#include <ProElement.h>
#include <ProFeature.h>
#include "log_define.h"
#include "UtilityHelperClass.h"
#include "Extrusion.h"

#define SIZEOFARR(a) (sizeof(a)/sizeof(a[0]))

extern FILE* errlog_fp;

Extrusion::Extrusion():
	sketchFunction(nullptr),
	referencePlaneSet(false),
	orientationPlaneSet(false),
	sketchFunctionSet(false)
{}

void Extrusion::updateExtrusionDepth(ProMdl model, double ExtrusionDepth)
{
	ProFeature extrusion_feature;
	ProError status = ProFeatureInit(static_cast<ProSolid>(model), extrusionId, &extrusion_feature);
	PT_TEST_LOG_SUCC("ProFeatureInit...extrusion depth Set")
	ProElement elementTree;
	status = ProFeatureElemtreeExtract(&extrusion_feature, nullptr, PRO_FEAT_EXTRACT_NO_OPTS, &elementTree);
	PT_TEST_LOG_SUCC("ProFeatureElemtreeExtract...extrusion depth Set")

	static ProElempathItem path_items[] = {
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_STD_EXT_DEPTH},
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_EXT_DEPTH_TO},
				{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_EXT_DEPTH_TO_VALUE}
	};

	ProElempath	path;
	status = ProElempathAlloc(&path);
	status = ProElempathDataSet(path, path_items, 3);

	ProElement extrusion_element_depth;
	status = ProElemtreeElementGet(elementTree, path, &extrusion_element_depth);
	PT_TEST_LOG_SUCC("ProElemtreeElementGet...extrusion depth Set")

	ProValue value;
	status = ProElementValueGet(extrusion_element_depth, &value);
	PT_TEST_LOG_SUCC("ProElementValueGet...extrusion depth Set")

	ProValueData value_data;
	status = ProValueDataGet(value, &value_data);

	value_data.v.d = ExtrusionDepth;
	ProValueDataSet(value, &value_data);
	ProElementValueSet(extrusion_element_depth, value);

	ProFeatureCreateOptions opts[1];
	opts[0] = PRO_FEAT_CR_INCOMPLETE_FEAT;
	ProErrorlist errs;
	status = ProFeatureRedefine(NULL, &extrusion_feature, elementTree, opts, 1, &errs);
	status = ProElempathFree(&path);
}

void Extrusion::setExtrusionDepth(double value)
{
	tree[18].data.v.d = value;
}

void Extrusion::setTheDrawingPlaneReference(ProMdl model, int surfaceId)
{
	ProError status;
	status = UtilityHelperClass::ProUtilSelectionFromSurfaceId(model, surfaceId, &tree[7].data.v.r);
	PT_TEST_LOG_SUCC("ProUtilSelectionFromSurfaceId.........SetTheDrawingPlaneReference");
	referencePlaneSet = status != PRO_TK_NO_ERROR;
}

void Extrusion::setTheDrawingPlaneOrientation(ProMdl model, int surfaceId)
{
	ProError status;
	status = UtilityHelperClass::ProUtilSelectionFromSurfaceId(model, surfaceId, &tree[10].data.v.r);
	PT_TEST_LOG_SUCC("ProUtilSelectionFromSurfaceId.........setTheDrawingPlaneOrientation");
	orientationPlaneSet = status != PRO_TK_NO_ERROR;
}

void Extrusion::setDrawingFunction(SketchFunctionDefinition sketchFunction)
{
	this->sketchFunction = sketchFunction;
	sketchFunctionSet = true;
}

ProError Extrusion::updateExtrusionSketch(ProMdl model, Parameter* params, int* sketchId, int side_id, int bot_id)
{
	static ProElempathItem path_items[] = {
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_STD_SECTION},
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_SKETCHER}
	};

	ProFeature extrusion_feature;
	ProError status = ProFeatureInit(static_cast<ProSolid>(model), extrusionId, &extrusion_feature);
	PT_TEST_LOG_SUCC("ProFeatureInit...Update Extrusion")
	ProElement elementTree;
	status = ProFeatureElemtreeExtract(&extrusion_feature, nullptr, PRO_FEAT_EXTRACT_NO_OPTS, &elementTree);
	PT_TEST_LOG_SUCC("ProFeatureElemtreeExtract...Update Extrusion")

	ProElempath				path;
	ProValue				value;
	ProValueData			value_data;
	ProSection				section;
	ProElement				sketch_element;
	
	status = ProElempathAlloc(&path);
	status = ProElempathDataSet(path, path_items, 2);
	status = ProElemtreeElementGet(elementTree, path, &sketch_element);
	status = ProElementValueGet(sketch_element, &value);

	status = ProValueDataGet(value, &value_data);
	section = (ProSection)value_data.v.p;
	status = ProElempathFree(&path);
	//////////////////////////////////////////////////////////
	/*----------------------------------------------------------*\
	Create a section
	\*----------------------------------------------------------*/
	
	status = updateSketchFunction(section, params, sketchId, side_id, bot_id);
	
	/*------------------------------------------------------------*\
	Set section-dependent element values
	\*------------------------------------------------------------*/
	ProValueDataSet(value, &value_data);
	ProElementValueSet(sketch_element, value);

	UtilityHelperClass::ProUtilFeatureSetSectionDependentValues(elementTree, tree,
		SIZEOFARR(tree));
	
	ProFeatureCreateOptions opts[1];

	ProErrorlist errs;
	opts[0] = PRO_FEAT_CR_INCOMPLETE_FEAT;
	//opts[0] = PRO_FEAT_CR_NO_OPTS;
	
	status = ProFeatureRedefine(NULL, &extrusion_feature, elementTree, opts, 1, &errs);
	return status;
}

ProError Extrusion::createExtrusion(ProMdl model, Parameter* params, int* sketchId, const char* featureName, int* side_id, int* bot_id)
{
	ProElement				elem_tree;
	ProElement				created_elemtree, sketch_element;

	ProError				err;
	ProModelitem			model_item;
	ProSelection			model_sel;

	ProFeatureCreateOptions opts[1];
	ProErrorlist			errs;
	ProElempath				path;
	ProValue				value;
	ProValueData			value_data;
	ProSection				section;

	int brk = 0;
	
	static ProElempathItem path_items[] = {
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_STD_SECTION},
		{PRO_ELEM_PATH_ITEM_TYPE_ID, PRO_E_SKETCHER}
	};

	do /* Used for exit from middle of block */
	{
		/*----------------------------------------------------------*\
		Create Element Tree
		\*----------------------------------------------------------*/
		err = UtilityHelperClass::ProUtilElemtreeCreate(tree, SIZEOFARR(tree), NULL, &elem_tree);
		if (err != PRO_TK_NO_ERROR)
			break;
		fprintf(errlog_fp, "creation of tree_passed\n");
		/*----------------------------------------------------------*\
		Create the incomplete protrusion in the current model
		\*----------------------------------------------------------*/
		err = ProMdlToModelitem(model, &model_item);
		err = ProSelectionAlloc(NULL, &model_item, &model_sel);

		ProFeature feature;
		
		opts[0] = PRO_FEAT_CR_INCOMPLETE_FEAT;
		err = ProFeatureCreate(model_sel, elem_tree, opts, 1, &feature, &errs);
		if (err != PRO_TK_NO_ERROR)
		{
			//ProUtilFeatErrsWrite("ProFeatureCreate", err, elem_tree, &errs);
			break;
		}
		err = ProSelectionFree(&model_sel);

		/*----------------------------------------------------------*\
		Get the initialized section element from the database
		\*----------------------------------------------------------*/
		// Details can be found on page 989 of user manual.
		err = ProElempathAlloc(&path);
		err = ProElempathDataSet(path, path_items, 2);

		err = ProFeatureElemtreeCreate(&feature, &created_elemtree);
		err = ProElemtreeElementGet(created_elemtree, path, &sketch_element);
		err = ProElementValueGet(sketch_element, &value);

		err = ProValueDataGet(value, &value_data);
		section = (ProSection)value_data.v.p;
		err = ProElempathFree(&path);
		//////////////////////////////////////////////////////////
		/*----------------------------------------------------------*\
		Create a section
		\*----------------------------------------------------------*/
		fprintf(errlog_fp, "Doing the drawing...\n");
		err = sketchFunction(section, params, sketchId, side_id, bot_id);
		if (err != PRO_TK_NO_ERROR)
			break;
		fprintf(errlog_fp, "Done the drawing...\n");
		/*------------------------------------------------------------*\
		Set section-dependent element values
		\*------------------------------------------------------------*/

		UtilityHelperClass::ProUtilFeatureSetSectionDependentValues(created_elemtree, tree,
			SIZEOFARR(tree));

		opts[0] = PRO_FEAT_CR_INCOMPLETE_FEAT;
		err = ProFeatureRedefine(NULL, &feature, created_elemtree, opts, 1, &errs);

		extrusionId = feature.id;

		if (err != PRO_TK_NO_ERROR)
		{
			//ProUtilFeatErrsWrite("ProFeatureRedefine", err, elem_tree, &errs);
			break;
		}
		err = ProElementFree(&elem_tree);
		/*----------------------------------------------------------*\
		Set feature name
		\*----------------------------------------------------------*/
		UtilityHelperClass::ProUtilModelitemNameSet(&feature, const_cast<char*>(featureName));
	} while (brk);

	return (err);
}

Extrusion::~Extrusion()
{
	ProError status = ProSelectionFree(&tree[7].data.v.r);
	PT_TEST_LOG_SUCC("ProSelectionFree");

	status = ProSelectionFree(&tree[10].data.v.r);
	PT_TEST_LOG_SUCC("ProSelectionFree");
}

