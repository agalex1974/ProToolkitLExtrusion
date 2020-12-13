#ifndef PARAMETRIC_MODEL_H
#define PARAMETRIC_MODEL_H
#include <ProPart.h>
#include <memory>


class ParametricModel
{
public:
	using pExtrusion = std::shared_ptr<Lextrusion>;
	std::shared_ptr<DatumPlane> frontPlane;
	std::shared_ptr<DatumPlane> sidePlane;
	std::shared_ptr<DatumPlane> bottomPlane;
	std::shared_ptr<DatumPlane> firstAxisIntersectingPlane;
	std::shared_ptr<DatumPlane> secondAxisIntersectingPlane;
	std::shared_ptr<Axis> holeAxis;
	std::shared_ptr<Hole> hole;
	ParametricModel();
	double getBaseLength();
	double getVerticalLength();
	double getExtrusionLength();
	double getThickness();
	double getDistFrmFrstRef();
	double getDistFrmScndRef();
	double getDiameter();

	void setBaseLength(double value);
	void setVerticalLength(double value);
	void setExtrusionLength(double value);
	void setThickness(double value);
	void setDistFrmFrstRef(double value);
	void setDistFrmScndRef(double value);
	void setDiameter(double value);

	bool createLExtrusion();
private:
	bool ModelCreated;
	pExtrusion lextrusion;
	ProPart creoModel;
	double BaseLength;
	double VerticalLength;
	double ExtrusionLength;
	double ThickNess;
	double DistFrmFrstRef;
	double DistFrmScndRef;
	double Diameter;
};

#endif

