// Developed by Alexander G. Agathos
// e-mail: alexander.agathos@gmail.com
// MIT license, see the license file in the Git repository.

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
	
	// The axis that is normal to the plane 
	enum AXIS_CONSTRAINT {
		AXIS_X = 0,
		AXIS_Y = 1,
		AXIS_Z = 2
	};
	
private:

	// It is good practice to define the feature tree like this as Creo is doing and have
	// a reader build the tree internally. Here is the feature tree definition the datum plane
	// the tree will be filled by the interface and will be given for creation.
	ElemTreeData def_tree[5] = {
	   {0, PRO_E_FEATURE_TREE, {(ProValueDataType)-1}},								// 0
	   {1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, PRO_FEAT_DATUM}},				// 1
	   {1, PRO_E_DTMPLN_CONSTRAINTS, {(ProValueDataType)-1}},							// 2
	   {2, PRO_E_DTMPLN_CONSTRAINT, {(ProValueDataType)-1}},							// 3
	   {3, PRO_E_DTMPLN_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, PRO_DTMPLN_DEF_X}}		// 4
	};

	// This is the offset definition that will translate the datum plane in the x-y-z direction according
	// to their normal orientation.
	ElemTreeData offs_tree[7] = {
		{0, PRO_E_FEATURE_TREE, {(ProValueDataType)-1}},								// 0
		{1, PRO_E_FEATURE_TYPE, {PRO_VALUE_TYPE_INT, PRO_FEAT_DATUM}},				// 1
		{1, PRO_E_DTMPLN_CONSTRAINTS, {(ProValueDataType)-1}},							// 2
		{2, PRO_E_DTMPLN_CONSTRAINT, {(ProValueDataType)-1}},							// 3
		{3, PRO_E_DTMPLN_CONSTR_TYPE, {PRO_VALUE_TYPE_INT, PRO_DTMPLN_OFFS}},		// 4
		{3, PRO_E_DTMPLN_CONSTR_REF, {PRO_VALUE_TYPE_SELECTION}},						// 5
		{3, PRO_E_DTMPLN_CONSTR_REF_OFFSET, {PRO_VALUE_TYPE_DOUBLE}}					// 6
	};

	// Creo's definition to the axes
	int axes[3] = {
		PRO_DTMPLN_DEF_X,
		PRO_DTMPLN_DEF_Y,
		PRO_DTMPLN_DEF_Z
	};

	int datum_id;										/**< The datum id */
	int dim_id;											/**< The dimension id */
	AXIS_CONSTRAINT axisConstraint;						/**< At which axes the plane has normal */
	double offset;										/**< The offset value */

	/**
	 * Internal creation of the datum plane. Once created it needs just to update it's offset
	 * according to the dialog values. This is why it is just internal
	 *
	 * @param model Creos part session
	 * @param name the name set for the datum and displayed in Creo's UI
	 * @return the status of creation.
	 */
	int createDatum(ProMdl model, const wchar_t* name);	
public:

	/**
	 * Get Creo's id assignment for the datum
	 *
	 * @return The id of the datum plane. 
	 */
	int getDatumId();

	/**
	 * Get Creo's id assignment for the datum offset.
	 *
	 * @return the id of the dimension.
	 */
	int getDatumDimId();

	/**
	 * Constructor of the datum plane
	 *
	 * @param model Creo's part session model
	 * @param constraint The axis that the plane is normal
	 * @param offset The offset from the origin of the coordinate system along the dimension.
	 * @param name The name of the datum plane displayed on Creo's UI
	 */
	DatumPlane(ProMdl model, AXIS_CONSTRAINT constraint, double offset, const wchar_t* name);

	/**
	 * set the offset of the datum
	 *
	 * @param model Creo's part session model
	 * @param offset the offset along the axis
	 */
	void setOffset(ProMdl model, double offset);
};

#endif
