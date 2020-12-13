#include <ProModelitem.h>
#include "log_define.h"
#include "UtilityHelperClass.h"
#include "Hole.h"

#include <xlocmon>

#define SIZEOFARR(a) (sizeof(a)/sizeof(a[0]))

extern FILE* errlog_fp;

Hole::Hole(ProMdl model, int axis_id, int plane_id, double diameter, const char* name)
{
	setTreeReferences(model, axis_id, plane_id, diameter);
	createHole(model, name);
}

int Hole::setTreeReferences(ProMdl model, int axis_id, int plane_id, double diameter)
{
	ProModelitem modelitem;
	ProError status = ProModelitemInit(model, axis_id, PRO_AXIS, &modelitem);
	PT_TEST_LOG_SUCC("ProModelitemInit...HOLE")

		if (status != PRO_TK_NO_ERROR)
			return (status);

	// The hole references this axis 
	status = ProSelectionAlloc(NULL, &modelitem, &tree[12].data.v.r);
	PT_TEST_LOG_SUCC("ProSelectionAlloc...HOLE")

		// This is the plane the hole will be put at  
		status = UtilityHelperClass::ProUtilSelectionFromSurfaceId(model, plane_id, &tree[14].data.v.r);
	PT_TEST_LOG_SUCC("ProUtilSelectionFromSurfaceId")

	// get the hole diamgeter.
	tree[5].data.v.d = diameter;
	return 0;
}

void Hole::setHoleDiameter(ProMdl model, double diameter)
{
	ProFeature hole_feature;
	ProError status = ProFeatureInit(static_cast<ProSolid>(model), hole_id, &hole_feature);
	PT_TEST_LOG_SUCC("ProFeatureInit...Hole Diameter Set")
	ProElement elementTree;
	status = ProFeatureElemtreeExtract(&hole_feature, nullptr, PRO_FEAT_EXTRACT_NO_OPTS, &elementTree);
	PT_TEST_LOG_SUCC("ProFeatureElemtreeExtract...Hole Diameter Set")

	static ProElempathItem path_items[] = {
			{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_HLE_COM},
			{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_DIAMETER}
	};
	
	ProElempath	path;
	status = ProElempathAlloc(&path);
	status = ProElempathDataSet(path, path_items, 2);
	
	ProElement hole_element_diameter;
	status = ProElemtreeElementGet(elementTree, path, &hole_element_diameter);
	PT_TEST_LOG_SUCC("ProElemtreeElementGet...Hole Diameter Set")

	ProValue value;
	status = ProElementValueGet(hole_element_diameter, &value);
	PT_TEST_LOG_SUCC("ProElementValueGet...Hole Diameter Set")

	ProValueData value_data;
	status = ProValueDataGet(value, &value_data);
	fprintf(errlog_fp, "");
	
	value_data.v.d = diameter;
	ProValueDataSet(value, &value_data);
	ProElementValueSet(hole_element_diameter, value);
	
	ProFeatureCreateOptions opts[1];
	opts[0] = PRO_FEAT_CR_INCOMPLETE_FEAT;
	ProErrorlist errs;
	status = ProFeatureRedefine(NULL, &hole_feature, elementTree, opts, 1, &errs);
	status = ProElempathFree(&path);
}

int Hole::createHole(ProMdl model, const char* name)
{
	ProElement		elem_tree;
	ProSelection	featsel;
	ProModelitem	modelitem;
	ProFeature		f;
	ProErrorlist	errs;
	/*-----------------------------------------------------------------*\
	Create new feature
	\*-----------------------------------------------------------------*/
	// Create the element tree for the hole
	ProError status = UtilityHelperClass::ProUtilElemtreeCreate(tree, SIZEOFARR(tree), NULL, &elem_tree);
	PT_TEST_LOG_SUCC("ProUtilElemtreeCreate");

	// Get the model item of the gear disk
	status = ProMdlToModelitem(model, &modelitem);
	PT_TEST_LOG_SUCC("ProUtilFeatsByName");

	// Get the gear model item as a selection
	status = ProSelectionAlloc(NULL, &modelitem, &featsel);
	PT_TEST_LOG_SUCC("ProSelectionAlloc");

	// Create the feature defined on the model tree on the   
	status = ProFeatureCreate(featsel, elem_tree, NULL, 0, &f, &errs);
	PT_TEST_LOG_SUCC("ProFeatureCreate");
	hole_id = f.id;

	//ProFeature
	status = ProSelectionFree(&featsel);
	PT_TEST_LOG_SUCC("ProSelectionFree");

	if (status != PRO_TK_NO_ERROR) {
		//ProUtilFeatErrsWrite("ProFeatureCreate", status, elem_tree, &errs);
		return (-1);
	}
	else {
		/*-----------------------------------------------------------------*\
		Set feature name
		\*-----------------------------------------------------------------*/
		status = UtilityHelperClass::ProUtilModelitemNameSet((ProModelitem*)&f, const_cast<char*>(name));
		PT_TEST_LOG_SUCC("ProUtilModelitemNameSet");

		if (status != PRO_TK_NO_ERROR)
			return (-1);
	}
	/*-----------------------------------------------------------------*\
		Free allocated memory
	\*-----------------------------------------------------------------*/
	status = ProSelectionFree(&tree[12].data.v.r);
	PT_TEST_LOG_SUCC("ProSelectionFree");

	status = ProSelectionFree(&tree[14].data.v.r);
	PT_TEST_LOG_SUCC("ProSelectionFree");

	return (status);

}
