// Developed by Alexander G. Agathos
// e-mail: alexander.agathos@gmail.com
// MIT license, see the license file in the Git repository.

#ifndef AXIS_H
#define AXIS_H

#include <ProElemId.h>
#include <ProFeatType.h>
#include <ProFeatForm.h>
#include <ProExtrude.h>
#include <ProStdSection.h>
#include <ProSection.h>
#include <ProDtmPln.h>
#include "HelperStructures.h"

class Axis
{
protected:
	
	// Axis feature three used to define the axis.
	// In this case the axis will be defined by the intersection of two planes
	ElemTreeData axis_tree[9] = {
		/* 0 */ {0, PRO_E_FEATURE_TREE, {(ProValueDataType)-1}},
		/* 1 */ {1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, PRO_FEAT_DATUM_AXIS}},
		/* 2 */ {1, PRO_E_DTMAXIS_CONSTRAINTS, {(ProValueDataType)-1}},
		/* 3 */ {2, PRO_E_DTMAXIS_CONSTRAINT, {(ProValueDataType)-1}},
		/* 4 */ {3, PRO_E_DTMAXIS_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, 1}},
		/* 5 */ {3, PRO_E_DTMAXIS_CONSTR_REF, {PRO_VALUE_TYPE_SELECTION}},
		/* 6 */ {2, PRO_E_DTMAXIS_CONSTRAINT, {(ProValueDataType)-1}},
		/* 7 */ {3, PRO_E_DTMAXIS_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, 1}},
		/* 8 */ {3, PRO_E_DTMAXIS_CONSTR_REF, {PRO_VALUE_TYPE_SELECTION}}
	};
	
	int axis_id; /**< This is the axis id, throughout the example it should not change even
						when it is redefined by the dialog */

	/**
	 * Set the axis intersecting planes
	 * 
	 * @param model The input part session of the creo parametric environment
	 * @param planeIdFirst The first intersecting plane id
	 * @param planeIdSecond The second intersecting plane id
	 */
	void setInstsectionPlanes(ProMdl model, int planeIdFirst, int planeIdSecond);

	/**
	 * Create the axis after the two intersecting planes has been provided
	 * 
	 * @param model The input part session of the creo parametric environment
	 * @param name The name that the axis will have in creo.
	 * @return a status value
	 */
	int createAxis(ProMdl model, const char* name);

public:
	/**
	 * Constructor of the axis class
	 * 
	 * @param model The input part session of the creo parametric environment
	 * @param planeIdFirst The first intersecting plane id
	 * @param planeIdSecond The second intersecting plane id
	 */
	Axis(ProMdl model, int planeIdFirst, int planeIdSecond, const char* name);

	/**
	 * Get axis id.
	 *
	 * @return The axis id
	 */
	int getAxisid();
	
};

#endif