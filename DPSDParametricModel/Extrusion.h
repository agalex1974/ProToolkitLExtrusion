// Developed by Alexander G. Agathos
// e-mail: alexander.agathos@gmail.com
// MIT license, see the license file in the Git repository.

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

	// Definition of the initial sketch function
	using SketchFunctionDefinition = ProError(*)(ProSection section, Parameter *params, int* sketchId, int* side_id, int* bot_id);
	// Definition of the function to update the sketch
	using UpdateSketchFunctionDefinition = ProError(*)(ProSection section, Parameter *params, int* sketchId, int side_id, int bot_id);

	// Feature tree of the Extrusion definition.
	// The class which will inherit this general definition need first to define the sketch plane on which the drawing will happen
	// and the orientation of the sketch plane, then the tree will be created by partial definition of the extrusion feature
	// this will create the sketch section on which we will grab and do the sketch painting on it. When finished the feature will be
	// redefined and it will become fully defined in creo parametric. 
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

	SketchFunctionDefinition sketchFunction;				/**< The sketch function definition */
	UpdateSketchFunctionDefinition updateSketchFunction;	/**< The update sketch function definition */
	bool referencePlaneSet;									/**< True if the sketching plane is created */
	bool orientationPlaneSet;								/**< True if the orientation plane is created */
	bool sketchFunctionSet;									/**< True if the sketching function is set */
public:
	
	/**
	 * The Extrusion constructor
	 */
	Extrusion();
	
	/** 
	 * The extrusion destructor
	 */
	~Extrusion();
protected:
	int extrusionId;										/**< The extrusion id */
	/**
	 * Function to update the extrusion depth after created.
	 *
	 * @param model Creo's part model session
	 * @param ExtrusionDepth The new extrusion depth of the feature
	 */
	void updateExtrusionDepth(ProMdl model, double ExtrusionDepth);

	/**
	 * Set the drawing sketch plane
	 *
	 * @param model Creo's part model session
	 * @param surfaceId The id of the sketching plane
	 */
	void setTheDrawingPlaneReference(ProMdl model, int surfaceId);

	/**
	 * Set the drawing sketch plane
	 *
	 * @param model Creo's part model session
	 * @param surfaceId The id of the orientation plane
	 */
	void setTheDrawingPlaneOrientation(ProMdl model, int surfaceId);

	/**
	 * Set the extrusion depth in the initialization phase
	 *
	 * @param value the initial depth value
	 */
	void setExtrusionDepth(double value);

	/**
	 * Set the drawing function for initial object sketching
	 *
	 * @param sketchFunction The initial sketching function 
	 */
	void setDrawingFunction(SketchFunctionDefinition sketchFunction);

	/**
	 * Create the extrusion after setting, 1) The sketching plane, 2) The orientation plane, 3) The sketching function
	 *
	 * @param model Creo's part model session
	 * @param params The parameters of the function
	 * @param sketchId the sketch Id. This should be replaced by a void pointer in later versions.
	 * @param featureName the feature name of the extrusion appearing in Creo
	 * @param side_id The id of one of one of the projections on the plane, this sets also the dimensionality
	 * @param bot_id The id of one of the projections on the plane, this sets also the dimensionality
	 * @return if the operation was successful. It needs to be further improved in later versions. 
	 */
	ProError createExtrusion(ProMdl model, Parameter* params, int* sketchId, const char* featureName, int* side_id, int* bot_id);

	/**
	 * Update the extrusion after setting the update function
	 *
	 * @param model Creo's part model session
	 * @param params The parameters of the function
	 * @param sketchId the sketch Id. This should be replaced by a void pointer in later versions.
	 * @param side_id The id of one of one of the projections on the plane, this sets also the dimensionality
	 * @param bot_id The id of one of the projections on the plane, this sets also the dimensionality
	 * @return if the operation was successful. It needs to be further improved in later versions.
	 */
	ProError updateExtrusionSketch(ProMdl model, Parameter* params, int* sketchId, int side_id, int bot_id);
};

#endif
