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
	int axis_id;
	void setInstsectionPlanes(ProMdl model, int planeIdFirst, int planeIdSecond);
	int createAxis(ProMdl model, const char* name);
public:
	Axis(ProMdl model, int planeIdFirst, int planeIdSecond, const char* name);
	int getAxisid();
	
};

#endif