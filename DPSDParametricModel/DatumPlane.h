#ifndef DATUM_SURFACE_X
#define DATUM_SURFACE_X

#include <ProElemId.h>
#include <ProFeatType.h>
#include <ProFeatForm.h>
#include <ProExtrude.h>
#include <ProStdSection.h>
#include <ProSection.h>
#include <ProDtmPln.h>
#include "HelperStructures.h"

class DatumPlane
{
public:
	enum AXIS_CONSTRAINT {
		AXIS_X = 0,
		AXIS_Y = 1,
		AXIS_Z = 2
	};
private:
	ElemTreeData def_tree[5] = {
	   {0, PRO_E_FEATURE_TREE, {(ProValueDataType)-1}},								// 0
	   {1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, PRO_FEAT_DATUM}},				// 1
	   {1, PRO_E_DTMPLN_CONSTRAINTS, {(ProValueDataType)-1}},							// 2
	   {2, PRO_E_DTMPLN_CONSTRAINT, {(ProValueDataType)-1}},							// 3
	   {3, PRO_E_DTMPLN_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, PRO_DTMPLN_DEF_X}}		// 4
	};

	ElemTreeData offs_tree[7] = {
		{0, PRO_E_FEATURE_TREE, {(ProValueDataType)-1}},								// 0
		{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, PRO_FEAT_DATUM}},				// 1
		{1, PRO_E_DTMPLN_CONSTRAINTS, {(ProValueDataType)-1}},							// 2
		{2, PRO_E_DTMPLN_CONSTRAINT, {(ProValueDataType)-1}},							// 3
		{3, PRO_E_DTMPLN_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, PRO_DTMPLN_OFFS}},		// 4
		{3, PRO_E_DTMPLN_CONSTR_REF, {PRO_VALUE_TYPE_SELECTION}},						// 5
		{3, PRO_E_DTMPLN_CONSTR_REF_OFFSET, {PRO_VALUE_TYPE_DOUBLE}}					// 6
	};

	int axes[3] = {
		PRO_DTMPLN_DEF_X,
		PRO_DTMPLN_DEF_Y,
		PRO_DTMPLN_DEF_Z
	};
	
	int datum_id;
	int dim_id;
	AXIS_CONSTRAINT axisConstraint;
	double offset;
	int createDatum(ProMdl model, const wchar_t* name);
public:
	int getDatumId();
	int getDatumDimId();
	DatumPlane(ProMdl model, AXIS_CONSTRAINT constraint, double offset, const wchar_t* name);
	void setOffset(ProMdl model, double offset);
};

#endif
