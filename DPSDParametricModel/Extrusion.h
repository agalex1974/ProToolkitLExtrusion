#ifndef EXTRUSION_H
#define EXTRUSION_H

#include <ProElemId.h>
#include <ProFeatType.h>
#include <ProFeatForm.h>
#include <ProExtrude.h>
#include <ProStdSection.h>
#include <ProSection.h>
#include "HelperStructures.h"

class Extrusion
{
public:
	
protected:
	using SketchFunctionDefinition = ProError(*)(ProSection section, Parameter *params, int* sketchId, int* side_id, int* bot_id);
	using UpdateSketchFunctionDefinition = ProError(*)(ProSection section, Parameter *params, int* sketchId, int side_id, int bot_id);
	ElemTreeData tree[19] = {
		/* 0 */ {0, PRO_E_FEATURE_TREE, {(ProValueDataType)-1}},
		/* 1 */ {1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, PRO_FEAT_PROTRUSION}},
		/* 2 */ {1, PRO_E_FEATURE_FORM, {PRO_VALUE_TYPE_INT, PRO_EXTRUDE}},
		/* 3 */ {1, PRO_E_EXT_SURF_CUT_SOLID_TYPE, {PRO_VALUE_TYPE_INT, PRO_EXT_FEAT_TYPE_SOLID}},
		/* 4 */ {1, PRO_E_REMOVE_MATERIAL, {PRO_VALUE_TYPE_INT, PRO_EXT_MATERIAL_ADD}},
		/* 5 */ {1, PRO_E_STD_SECTION,  {(ProValueDataType)-1}},
		/* 6 */ {2, PRO_E_STD_SEC_SETUP_PLANE, {(ProValueDataType)-1}},
		/* 7 */ {3, PRO_E_STD_SEC_PLANE, {PRO_VALUE_TYPE_SELECTION}},
		/* 8 */ {3, PRO_E_STD_SEC_PLANE_VIEW_DIR, {PRO_VALUE_TYPE_INT, PRO_SEC_VIEW_DIR_SIDE_ONE}},
		/* 9 */ {3, PRO_E_STD_SEC_PLANE_ORIENT_DIR, {PRO_VALUE_TYPE_INT, PRO_SEC_ORIENT_DIR_RIGHT}},//here
		/* 10 */{3, PRO_E_STD_SEC_PLANE_ORIENT_REF, {PRO_VALUE_TYPE_SELECTION}},
		/* 11 */{1, PRO_E_FEAT_FORM_IS_THIN, {PRO_VALUE_TYPE_INT, PRO_EXT_FEAT_FORM_NO_THIN}},
		/* 12 */{1, PRO_E_STD_DIRECTION, {PRO_VALUE_TYPE_INT, PRO_EXT_CR_IN_SIDE_TWO}},//here
		/* 13 */{1, PRO_E_STD_EXT_DEPTH, {(ProValueDataType)-1}},
		/* 14 */{2, PRO_E_EXT_DEPTH_FROM, {(ProValueDataType)-1}},
		/* 15 */{3, PRO_E_EXT_DEPTH_FROM_TYPE, {PRO_VALUE_TYPE_INT, PRO_EXT_DEPTH_FROM_NONE}},
		/* 16 */{2, PRO_E_EXT_DEPTH_TO, {(ProValueDataType)-1}},
		/* 17 */{3, PRO_E_EXT_DEPTH_TO_TYPE, {PRO_VALUE_TYPE_INT, PRO_EXT_DEPTH_TO_BLIND}},
		/* 18 */{3, PRO_E_EXT_DEPTH_TO_VALUE, {PRO_VALUE_TYPE_DOUBLE}}
	};
	SketchFunctionDefinition sketchFunction;
	UpdateSketchFunctionDefinition updateSketchFunction;
	bool referencePlaneSet;
	bool orientationPlaneSet;
	bool sketchFunctionSet;
	ProFeature feature;
public:
	Extrusion();
	~Extrusion();
protected:
	int extrusionId;
	void updateExtrusionDepth(ProMdl model, double ExtrusionDepth);
	void setTheDrawingPlaneReference(ProMdl model, int surfaceId);
	void setTheDrawingPlaneOrientation(ProMdl model, int surfaceId);
	void setExtrusionDepth(double value);
	void setDrawingFunction(SketchFunctionDefinition sketchFunction);
	bool createExtrusion(ProMdl model, Parameter* params, int* sketchId, const char* featureName, int* side_id, int* bot_id);
	bool updateExtrusionSketch(ProMdl model, Parameter* params, int* sketchId, int side_id, int bot_id);
};

#endif
