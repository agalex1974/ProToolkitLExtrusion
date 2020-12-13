#include <ProFeature.h>
#include <ProSolid.h>
#include "log_define.h"
#include "UtilityHelperClass.h"
#include "Axis.h"

#define SIZEOFARR(a) (sizeof(a)/sizeof(a[0]))

extern FILE* errlog_fp;

Axis::Axis(ProMdl model, int planeIdFirst, int planeIdSecond, const char* name)
{
	setInstsectionPlanes(model, planeIdFirst, planeIdSecond);
	createAxis(model, name);
}

int Axis::getAxisid()
{
	return axis_id;
}

void Axis::setInstsectionPlanes(ProMdl model, int planeIdFirst, int planeIdSecond)
{
	ProError status = UtilityHelperClass::ProUtilSelectionFromSurfaceId(model, planeIdFirst, &axis_tree[5].data.v.r);
	PT_TEST_LOG_SUCC("ProUtilSelectionFromSurfaceId...FIRST AXIS REFERENCE PLANE")
	status = UtilityHelperClass::ProUtilSelectionFromSurfaceId(model, planeIdSecond, &axis_tree[8].data.v.r);
	PT_TEST_LOG_SUCC("ProUtilSelectionFromSurfaceId...SECOND AXIS REFERENCE PLANE")
}

int Axis::createAxis(ProMdl model, const char* name)
{
	ProElement elem_axis_tree;
	ProSelection featsel_axis;
	ProFeature feature_axis;
	ProModelitem model_item;
	ProErrorlist axis_errs;
	
	ProFeatureCreateOptions opts[2], *cr_opts;
	opts[0] = PRO_FEAT_CR_NO_OPTS;

	ProError status = ProArrayAlloc(0, sizeof(ProFeatureCreateOptions), 1, (ProArray*)&cr_opts);
	PT_TEST_LOG_SUCC("ProArrayAlloc...CREATE AXIS")

	status = ProArrayObjectAdd((ProArray*)&cr_opts, PRO_VALUE_UNUSED, 1, (void*)&(opts[0]));
	PT_TEST_LOG_SUCC("ProArrayObjectAdd...CREATE_AXIS")

	status = UtilityHelperClass::ProUtilElemtreeCreate(axis_tree, SIZEOFARR(axis_tree), NULL, &elem_axis_tree);
	PT_TEST_LOG_SUCC("ProUtilElemtreeCreate...CREATE_AXIS");

	status = ProMdlToModelitem(model, &model_item);
	PT_TEST_LOG_SUCC("ProMdlToModelitem...CREATE_AXIS");

	status = ProSelectionAlloc(NULL, &model_item, &featsel_axis);
	PT_TEST_LOG_SUCC("ProSelectionAlloc...CREATE_AXIS");

	status = ProFeatureWithoptionsCreate(featsel_axis, elem_axis_tree, cr_opts, PRO_REGEN_FORCE_REGEN, &feature_axis, &axis_errs);
	PT_TEST_LOG_SUCC("ProFeatureWithoptionsCreate...CREATE_AXIS");

	if (status != PRO_TK_NO_ERROR)
	{
		//ProUtilFeatErrsWrite("ProFeatureCreate", status, elem_axis_tree, &axis_errs);

		return (-1);
	}

	status = UtilityHelperClass::ProUtilAxisGeomitem(&feature_axis, PRO_AXIS, &axis_id);
	PT_TEST_LOG_SUCC("ProUtilAxisGeomitem...CREATE_AXIS");

	if (status != PRO_TK_NO_ERROR)
		return(status);

	status = ProArrayFree((ProArray*)&cr_opts);
	PT_TEST_LOG_SUCC("ProArrayFree...CREATE_AXIS");

	status = ProSelectionFree(&featsel_axis);
	PT_TEST_LOG_SUCC("ProSelectionFree...CREATE_AXIS");

	status = ProSelectionFree(&axis_tree[5].data.v.r);
	PT_TEST_LOG_SUCC("ProSelectionFree...CREATE_AXIS");

	status = ProSelectionFree(&axis_tree[8].data.v.r);
	PT_TEST_LOG_SUCC("ProSelectionFree...CREATE_AXIS");

	/*-----------------------------------------------------------------*\
	Set datum axis name
	\*-----------------------------------------------------------------*/
	status = UtilityHelperClass::ProUtilModelitemNameSet((ProModelitem*)&feature_axis, const_cast<char*>(name));
	PT_TEST_LOG_SUCC("ProUtilModelitemNameSet");

	if (status != PRO_TK_NO_ERROR)
		return (-1);
	return 0;
}

