#include "log_define.h"
#include <ProMdl.h>
#include <ProWindows.h>
#include "Extrusion.h"
#include <ProSolid.h>
#include "Lextrusion.h"
#include "DatumPlane.h"
#include "Axis.h"
#include "Hole.h"
#include "ParametricModel.h"

extern FILE* errlog_fp;

ParametricModel::ParametricModel():
	ModelCreated(false),
	BaseLength(20.0),
	VerticalLength(30.0),
	ExtrusionLength(48.0),
	ThickNess(8.0),
	DistFrmFrstRef(24.0),
	DistFrmScndRef(3.5),
	Diameter(2.7)
{	
	ProError err;
	ProMdl modelTemp;
	err = ProMdlCurrentGet(&modelTemp);
	creoModel = static_cast<ProPart>(modelTemp);
	lextrusion = std::make_shared<Lextrusion>(creoModel);
	frontPlane = std::make_shared<DatumPlane>(creoModel, DatumPlane::AXIS_Z, 0.0, L"FRONT_PLANE");
	sidePlane = std::make_shared<DatumPlane>(creoModel, DatumPlane::AXIS_X, 0.0, L"SIDE_PLANE");
	bottomPlane = std::make_shared<DatumPlane>(creoModel, DatumPlane::AXIS_Y, 0.0, L"BOTTOM_PLANE");
}

double ParametricModel::getBaseLength()
{
	return BaseLength;
}

double ParametricModel::getVerticalLength()
{
	return VerticalLength;
}

double ParametricModel::getExtrusionLength()
{
	return ExtrusionLength;
}

double ParametricModel::getThickness()
{
	return ThickNess;
}

double ParametricModel::getDistFrmFrstRef()
{
	return DistFrmFrstRef;
}

double ParametricModel::getDistFrmScndRef()
{
	return DistFrmScndRef;
}

double ParametricModel::getDiameter()
{
	return Diameter;
}


void ParametricModel::setBaseLength(double value)
{
	BaseLength = value;
}

void ParametricModel::setVerticalLength(double value)
{
	VerticalLength = value;
}

void ParametricModel::setExtrusionLength(double value)
{
	ExtrusionLength = value;
}

void ParametricModel::setThickness(double value)
{
	ThickNess = value;
}

void ParametricModel::setDistFrmFrstRef(double value)
{
	DistFrmFrstRef = value;
}

void ParametricModel::setDistFrmScndRef(double value)
{
	DistFrmScndRef = value;
}

void ParametricModel::setDiameter(double value)
{
	Diameter = value;
}

bool ParametricModel::createLExtrusion()
{
	if (!ModelCreated)
	{
		lextrusion->createLextrusion(BaseLength, VerticalLength, ExtrusionLength, ThickNess, frontPlane->getDatumId(), sidePlane->getDatumId(),
			bottomPlane->getDatumId());
		firstAxisIntersectingPlane = std::make_shared<DatumPlane>(creoModel, DatumPlane::AXIS_Y, ThickNess + DistFrmScndRef, L"FIRST_INTERSECTION_PLANE");
		secondAxisIntersectingPlane = std::make_shared<DatumPlane>(creoModel, DatumPlane::AXIS_Z, ExtrusionLength - DistFrmFrstRef, L"SECOND_INTERSECTION_PLANE");
		holeAxis = std::make_shared<Axis>(creoModel, firstAxisIntersectingPlane->getDatumId(), secondAxisIntersectingPlane->getDatumId(), "HOLE_AXIS");
		hole = std::make_shared<Hole>(creoModel, holeAxis->getAxisid(), lextrusion->GetSideSurfaceId(), Diameter, "PARAMETRIC_HOLE");
		ModelCreated = true;
		return ModelCreated;
	}
	lextrusion->updateLExtrusion(BaseLength, VerticalLength, ExtrusionLength, ThickNess, frontPlane->getDatumId(), sidePlane->getDatumId());
	firstAxisIntersectingPlane->setOffset(creoModel, ThickNess + DistFrmScndRef);
	secondAxisIntersectingPlane->setOffset(creoModel, ExtrusionLength - DistFrmFrstRef);
	hole->setHoleDiameter(creoModel, Diameter);
	
	ProError status = ProSolidRegenerate(creoModel, 0);
	PT_TEST_LOG_SUCC("ProSolidRegenerate");
	status = ProWindowRepaint(-1);
	PT_TEST_LOG_SUCC("ProWindowRepaint");

	return true;
}
