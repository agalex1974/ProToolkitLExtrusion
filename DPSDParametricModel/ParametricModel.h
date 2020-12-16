// Developed by Alexander G. Agathos
// e-mail: alexander.agathos@gmail.com
// MIT license, see the license file in the Git repository.

#ifndef PARAMETRIC_MODEL_H
#define PARAMETRIC_MODEL_H

#include <ProPart.h>
#include <memory>

class ParametricModel
{
public:

	/**
	 * The constructor of the parametric model
	 */
	ParametricModel();

	/**
	 * Returns the base length of the L-extrusion
	 *
	 * @return The base length of the L-extrusion
	 */
	double getBaseLength();					

	/**
	 * Returns the vertical length of the L-extrusion
	 *
	 * @return The vertical length of the L-extrusion
	 */
	double getVerticalLength();

	/**
	 * Returns the extrusion length (depth) of the L-extrusion
	 *
	 * @return The extrusion length of the L-extrusion
	 */
	double getExtrusionLength();

	/**
	 * Returns the L-shape thickness
	 *
	 * @return The L-shape thickness
	 */
	double getThickness();

	/**
	 * Returns the distance of the hole from the first reference
	 *
	 * @return The distance of the hole from the first reference
	 */
	double getDistFrmFrstRef();

	/**
	 * Returns the distance of the hole from the second reference
	 *
	 * @return The distance of the hole from the second reference
	 */
	double getDistFrmScndRef();

	/**
	 * Returns the diameter of the hole
	 *
	 * @return The distance of the hole
	 */
	double getDiameter();

	/**
	 * Set the base length of the L-shape
	 *
	 * @param value The value of the base length of the L-shape
	 */
	void setBaseLength(double value);

	/**
	 * Set the vertical length of the L-shape
	 *
	 * @param value The value of the vertical length of the L-shape
	 */
	void setVerticalLength(double value);

	/**
	 * Set the extrusion length of the L-shape
	 *
	 * @param value The value of the extrusion length of the L-shape
	 */
	void setExtrusionLength(double value);

	/**
	 * Set the thickness of the L-shape
	 *
	 * @param value The value of the thickness of the L-shape
	 */
	void setThickness(double value);

	/**
	 * Set the distance of the hole from the first reference
	 *
	 * @param value The distance of the hole from the first reference
	 */
	void setDistFrmFrstRef(double value);

	/**
	 * Set the distance of the hole from the second reference
	 *
	 * @param value The distance of the hole from the second reference
	 */
	void setDistFrmScndRef(double value);

	/**
	 * Set the diameter of the hole 
	 *
	 * @param value The diameter of the hole
	 */
	void setDiameter(double value);

	/**
	 * create the LExtrusion from the dialog box input parameters
	 *
	 * @return success or failure of the operation
	 */
	bool createLExtrusion();

private:
	using pExtrusion = std::shared_ptr<Lextrusion>;
	std::shared_ptr<DatumPlane> frontPlane;						/**< the front datum plane */
	std::shared_ptr<DatumPlane> sidePlane;						/**< the side datum plane */
	std::shared_ptr<DatumPlane> bottomPlane;					/**< the bottom datum plane */
	std::shared_ptr<DatumPlane> firstAxisIntersectingPlane;		/**< the first intersecting plane to form the axis */
	std::shared_ptr<DatumPlane> secondAxisIntersectingPlane;	/**< the second intersecting plane to form the axis */
	std::shared_ptr<Axis> holeAxis;								/**< the axis of the hole */
	std::shared_ptr<Hole> hole;									/**< the hole feature */
	
	bool ModelCreated;											/**< True if the model is initially created */
	pExtrusion lextrusion;										/**< The l-extrusion */
	ProPart creoModel;											/**< This is the creo model */
	double BaseLength;											/**< The base length variable */
	double VerticalLength;										/**< The vertical length variable */
	double ExtrusionLength;										/**< The extrusion length variable */
	double ThickNess;											/**< The thickness of the L-shape */
	double DistFrmFrstRef;										/**< The distance from the first reference */
	double DistFrmScndRef;										/**< The distance from the second reference */
	double Diameter;											/**< The diameter of the hole */
};

#endif