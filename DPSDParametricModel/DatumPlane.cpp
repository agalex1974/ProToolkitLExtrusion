#include <ProFeature.h>
#include <ProModelitem.h>
#include "log_define.h"
#include "DatumPlane.h"
#include "UtilityHelperClass.h"

#define SIZEOFARR(a) (sizeof(a)/sizeof(a[0]))

extern FILE* errlog_fp;

DatumPlane::DatumPlane(ProMdl model, AXIS_CONSTRAINT constraint, double offset, const wchar_t* name):
	axisConstraint(constraint),
	offset(offset),
	datum_id(-1)
{
	createDatum(model, name);
}

void DatumPlane::setOffset(ProMdl model, double offset)
{
	ProDimension dim;
	ProError status = ProModelitemInit(model, dim_id, PRO_DIMENSION, &dim);
	PT_TEST_LOG_SUCC("ProModelitemInit");

	status = ProDimensionValueSet(&dim, offset);
	PT_TEST_LOG_SUCC("ProDimensionValueSet");
}

int DatumPlane::getDatumId()
{
	return datum_id;
}

int DatumPlane::getDatumDimId()
{
	return dim_id;
}

int DatumPlane::createDatum(ProMdl model, const wchar_t* name)
{
	ProSelection featsel;
	ProElement elem_tree;
	ProModelitem modelitem;
	ProFeature f;
	ProErrorlist errs;

	offs_tree[6].data.v.d = offset;


	/*-----------------------------------------------------------------*\
	Create Element Tree
	\*-----------------------------------------------------------------*/
	def_tree[4].data.v.i = axes[axisConstraint];
	ProError status;
	status = UtilityHelperClass::ProUtilElemtreeCreate(def_tree,SIZEOFARR(def_tree), nullptr, &elem_tree);
	PT_TEST_LOG_SUCC("ProUtilElemtreeCreate.......ProUserCreateDatum");

	/*-----------------------------------------------------------------*\
	Create Default Datum
	\*-----------------------------------------------------------------*/
	status = ProMdlToModelitem(model, &modelitem);
	PT_TEST_LOG_SUCC("ProMdlToModelitem....DATUM");

	status = ProSelectionAlloc(NULL, &modelitem, &featsel);
	PT_TEST_LOG_SUCC("ProSelectionAlloc");

	status = ProFeatureCreate(featsel, elem_tree, NULL, 0, &f, &errs);
	PT_TEST_LOG_SUCC("ProFeatureCreate");

	if (status != PRO_TK_NO_ERROR) {
		//ProUtilFeatErrsWrite("ProFeatureCreate", status, elem_tree, &errs);
		return (-1);
	}

	/*-----------------------------------------------------------------*\
	Find surface ID for datum plane feature
	\*-----------------------------------------------------------------*/
	status = UtilityHelperClass::ProUtilFeatFirstGeomitem(&f, PRO_SURFACE, &datum_id);
	PT_TEST_LOG_SUCC("ProUtilFeatFirstGeomitem");
	
	status = UtilityHelperClass::ProUtilSelectionFromSurfaceId(model, datum_id, &offs_tree[5].data.v.r);
	PT_TEST_LOG_SUCC("ProUtilSelectionFromSurfaceId");

	status = UtilityHelperClass::ProUtilElemtreeCreate(offs_tree, SIZEOFARR(offs_tree), NULL, &elem_tree);
	PT_TEST_LOG_SUCC("ProUtilElemtreeCreate");

	status = ProFeatureCreate(featsel, elem_tree, NULL, 0, &f, &errs);
	PT_TEST_LOG_SUCC("ProFeatureCreate");

	if (status != PRO_TK_NO_ERROR)
	{
		//ProUtilFeatErrsWrite("ProFeatureCreate", status, elem_tree, &errs);
		return (-1);
	}

	status = ProModelitemNameSet((ProModelitem*)&f, const_cast<wchar_t*>(name));
	PT_TEST_LOG_SUCC("ProModelitemNameSet");

	if (status != PRO_TK_NO_ERROR)
		return (-1);

	status = ProSelectionFree(&featsel);
	PT_TEST_LOG_SUCC("ProSelectionFree");

	status = UtilityHelperClass::ProUtilFeatFirstGeomitem(&f, PRO_SURFACE, &datum_id);
	PT_TEST_LOG_SUCC("ProUtilFeatFirstGeomitem");

	status = UtilityHelperClass::ProUtilFeatFirstDimension(&f, &dim_id);
	PT_TEST_LOG_SUCC("ProUtilFeatFirstDimension");
	return 0;
}



