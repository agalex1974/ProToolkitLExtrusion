#include <ProUI.h>
#include <ProUIDialog.h>
#include <ProUIInputpanel.h>
#include <ProUIPushbutton.h>
#include "Extrusion.h"
#include "Lextrusion.h"
#include "DatumPlane.h"
#include "Axis.h"
#include "Hole.h"
#include "ParametricModel.h"
#include "GuiCInterface.h"

const char* GuiCInterface::UI_GUI_NAME = "masterstudentcreo";
const char* GuiCInterface::UI_INPUT_BASE_LENGTH = "InputPanelBaseLength";
const char* GuiCInterface::UI_INPUT_VERTICAL_LENGTH = "InputPanelVerticalLength";
const char* GuiCInterface::UI_INPUT_EXTRUSION_LENGTH = "InputPanelExtrusionLength";
const char* GuiCInterface::UI_INPUT_THICKNESS = "InputPanelThickness";
const char* GuiCInterface::UI_INPUT_DST_FRST_REF = "InputPanelDstFrstRef";
const char* GuiCInterface::UI_INPUT_DST_SCND_REF = "InputPanelDstScndRef";
const char* GuiCInterface::UI_INPUT_DIAMETER = "InputPanelDimeter";
const char* GuiCInterface::UI_BUTTON_CREATE = "PushButtonCreate";

extern ParametricModel* parametricModel;

GuiCInterface::GuiCInterface(ParametricModel* parametricModel)
{
}

int GuiCInterface::GuiActivate()
{
	ProError err;
	err = ProUIDialogCreate(const_cast<char*>(UI_GUI_NAME), const_cast<char*>(UI_GUI_NAME));
	err = ProUIDialogCloseActionSet(const_cast<char*>(UI_GUI_NAME),
		static_cast<ProUIAction>(closeAction), nullptr);
	err = ProUIPushbuttonActivateActionSet(const_cast<char*>(UI_GUI_NAME), 
		const_cast<char*>(UI_BUTTON_CREATE),(ProUIAction)on_push_button_create, nullptr);

	int ui_status;
	setCurrentModelValues(parametricModel);
	err = ProUIDialogActivate(const_cast<char*>(UI_GUI_NAME), &ui_status);
	err = ProUIDialogDestroy(const_cast<char*>(UI_GUI_NAME));
	return 0;
}

void GuiCInterface::initialize_ui()
{
}

void GuiCInterface::closeAction(char* dialog, char *component, ProAppData appdata)
{
	ProUIDialogExit(dialog, 0);
}

GuiCInterface::~GuiCInterface()
{
	
}

ProError GuiCInterface::setBaseLength(double value)
{
	ProError err;
	err = ProUIInputpanelDoubleSet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_BASE_LENGTH), value);
	return err;
}

double GuiCInterface::getBaseLength(ProError& status)
{
	double value;
	status = ProUIInputpanelDoubleGet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_BASE_LENGTH), &value);
	return value;
}


ProError GuiCInterface::setDstFrstRef(double value)
{
	ProError err;
	err = ProUIInputpanelDoubleSet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_DST_FRST_REF), value);
	return err;
}


double GuiCInterface::getDstFrstRef(ProError& status)
{
	double value;
	status = ProUIInputpanelDoubleGet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_DST_FRST_REF), &value);
	return value;
}

ProError GuiCInterface::setDstScndRef(double value)
{
	ProError err;
	err = ProUIInputpanelDoubleSet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_DST_SCND_REF), value);
	return err;
}

double GuiCInterface::getDstScndRef(ProError& status)
{
	double value;
	status = ProUIInputpanelDoubleGet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_DST_SCND_REF), &value);
	return value;
}

ProError GuiCInterface::setDiameter(double value)
{
	ProError err;
	err = ProUIInputpanelDoubleSet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_DIAMETER), value);
	return err;
}

double GuiCInterface::getDiameter(ProError& status)
{
	double value;
	status = ProUIInputpanelDoubleGet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_DIAMETER), &value);
	return value;
}

ProError GuiCInterface::setExtrusionLength(double value)
{
	ProError err;
	err = ProUIInputpanelDoubleSet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_EXTRUSION_LENGTH), value);
	return err;
}

double GuiCInterface::getExtrusionLength(ProError& status)
{
	double value;
	status = ProUIInputpanelDoubleGet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_EXTRUSION_LENGTH), &value);
	return value;
}

ProError GuiCInterface::setThickness(double value)
{
	ProError err;
	err = ProUIInputpanelDoubleSet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_THICKNESS), value);
	return err;
}

double GuiCInterface::getThickness(ProError& status)
{
	double value;
	status = ProUIInputpanelDoubleGet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_THICKNESS), &value);
	return value;
}

ProError GuiCInterface::setVerticalLength(double value)
{
	ProError err;
	err = ProUIInputpanelDoubleSet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_VERTICAL_LENGTH), value);
	return err;
}

double GuiCInterface::getVerticalLength(ProError& status)
{
	double value;
	status = ProUIInputpanelDoubleGet(const_cast<char*>(UI_GUI_NAME),
		const_cast<char*>(UI_INPUT_VERTICAL_LENGTH), &value);
	return value;
}

void GuiCInterface::on_push_button_create(char* dialog, char *component, ProAppData appdata)
{
	getCurrentModelValues(parametricModel);
	parametricModel->createLExtrusion();
}

void GuiCInterface::setCurrentModelValues(ParametricModel* parametricModelInput)
{
	setBaseLength(parametricModelInput->getBaseLength());
	setVerticalLength(parametricModelInput->getVerticalLength());
	setDiameter(parametricModelInput->getDiameter());
	setDstFrstRef(parametricModelInput->getDistFrmFrstRef());
	setDstScndRef(parametricModelInput->getDistFrmScndRef());
	setExtrusionLength(parametricModelInput->getExtrusionLength());
	setDiameter(parametricModelInput->getDiameter());
	setThickness(parametricModelInput->getThickness());
}

void GuiCInterface::getCurrentModelValues(ParametricModel* parametricModelInput)
{
	ProError Error;
	parametricModelInput->setBaseLength(getBaseLength(Error));
	parametricModelInput->setVerticalLength(getVerticalLength(Error));
	parametricModelInput->setDiameter(getDiameter(Error));
	parametricModelInput->setDistFrmFrstRef(getDstFrstRef(Error));
	parametricModelInput->setDistFrmScndRef(getDstScndRef(Error));
	parametricModelInput->setExtrusionLength(getExtrusionLength(Error));
	parametricModelInput->setDiameter(getDiameter(Error));
	parametricModelInput->setThickness(getThickness(Error));
}
