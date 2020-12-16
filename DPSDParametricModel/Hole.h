// Developed by Alexander G. Agathos
// e-mail: alexander.agathos@gmail.com
// MIT license, see the license file in the Git repository.

#ifndef HOLE_H
#define HOLE_H

#include <ProElemId.h>
#include <ProFeatType.h>
#include <ProFeatForm.h>
#include <ProExtrude.h>
#include <ProStdSection.h>
#include <ProSection.h>
#include <ProDtmPln.h>
#include "HelperStructures.h"

class Hole
{
protected:

	// The feature tree of the hole. This feature can be fully defined by a reference plane on which
	// the whole circle will reside and of course an axis through which it will pass symmetrically.
	// In this example we follow this definition of a hole.
	ElemTreeData tree[15] = {
		/* 0*/ 	{0, PRO_E_FEATURE_TREE, {(ProValueDataType)-1}},
		/* 1*/	{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, PRO_FEAT_HOLE}},
		/* 2*/	{1, PRO_E_FEATURE_FORM, {PRO_VALUE_TYPE_INT, PRO_EXTRUDE}},
		/* 3*/	{1, PRO_E_HLE_COM, {(ProValueDataType)-1}},
		/* 4*/ 	{2, PRO_E_HLE_TYPE_NEW, {PRO_VALUE_TYPE_INT, 16}},
		/* 5*/ 	{2, PRO_E_DIAMETER, {PRO_VALUE_TYPE_DOUBLE, 4}},
		/* 6*/ 	{2, PRO_E_HOLE_STD_DEPTH, {(ProValueDataType)-1}},
		/* 7*/ 	{3, PRO_E_HOLE_DEPTH_TO, {(ProValueDataType)-1}},
		/* 8*/ 	{4, PRO_E_HOLE_DEPTH_TO_TYPE, {PRO_VALUE_TYPE_INT, 3}},
		/* 9*/	{3, PRO_E_HOLE_DEPTH_FROM, {(ProValueDataType)-1}},
		/* 10*/	{4, PRO_E_HOLE_DEPTH_FROM_TYPE, {PRO_VALUE_TYPE_INT, 3}},
		/* 11*/	{1, PRO_E_HLE_PLACEMENT, {(ProValueDataType)-1}},
		/* 12*/	{2, PRO_E_HLE_PRIM_REF, {PRO_VALUE_TYPE_SELECTION}},
		/* 13*/	{2, PRO_E_HLE_PL_TYPE, {PRO_VALUE_TYPE_INT, 5}},
		/* 14*/	{2, PRO_E_HLE_PLCMNT_PLANE, {PRO_VALUE_TYPE_SELECTION}},
	};

	int hole_id; /**< The hole id that Creo assigns to the feature hole */

	/**
	 * Set in the tree the parameters. All we need to define a hole is the axis through which it passes,
	 * the plane which will define its start position and of course the diameter of the hole.
	 *
	 * @param model Creo's part model session
	 * @param axis_id The axis id through which the hole will pass
	 * @param plane_id The starting plane id
	 * @param diameter The diameter of the hole
	 * @return The status of creation (needs to be fully defined in later versions)
	 */
	int setTreeReferences(ProMdl model, int axis_id, int plane_id, double diameter);

	/**
	 * Create the hole after defining the three references
	 *
	 * @param model Creo's part model session
	 * @param name The name of the hole
	 * @return The status of creation (needs to be fully defined in later versions) 
	 */
	int createHole(ProMdl model, const char* name);
	
public:

	/**
	 * Constructor of the hole
	 *
	 * @param model Creo's part model session
	 * @param axis_id The axis id through which the hole will pass
	 * @param plane_id The plane id from which the hole will pass
	 * @param diameter The diameter of the hole
	 * @param name The name of the hole as such will appear in Creo's UI
	 */
	Hole(ProMdl model, int axis_id, int plane_id, double diameter, const char* name);

	/**
	 * Update the hole diameter
	 *
	 * @param model Creo's part model session
	 * @param diameter The diameter of the hole
	 */
	void setHoleDiameter(ProMdl model, double diameter);
};

#endif
