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
	int hole_id;
	int setTreeReferences(ProMdl model, int axis_id, int plane_id, double diameter);
	int createHole(ProMdl model, const char* name);
public:
	Hole(ProMdl model, int axis_id, int plane_id, double diameter, const char* name);
	void setHoleDiameter(ProMdl model, double diameter);
};

#endif
