#ifndef PARAMETRIC_GUI_C_INTERFACE_H
#define PARAMETRIC_GUI_C_INTERFACE_H

class GuiCInterface
{
	// Declare user interface constants to tie them with retrieving the
	static const char* UI_GUI_NAME;
	static const char* UI_INPUT_BASE_LENGTH;
	static const char* UI_INPUT_VERTICAL_LENGTH;
	static const char* UI_INPUT_EXTRUSION_LENGTH;
	static const char* UI_INPUT_THICKNESS;
	static const char* UI_INPUT_DST_FRST_REF;
	static const char* UI_INPUT_DST_SCND_REF;
	static const char* UI_INPUT_DIAMETER;
	static const char* UI_BUTTON_CREATE;
	void initialize_ui();
	static void closeAction(char* dialog, char *component, ProAppData appdata);
	static void setCurrentModelValues(ParametricModel* parametricModel);
	static void getCurrentModelValues(ParametricModel* parametricModel);
public:
	GuiCInterface(ParametricModel* parametricModel);
	~GuiCInterface();
	static int GuiActivate();
	static void on_push_button_create(char* dialog, char *component, ProAppData appdata);
	static ProError setBaseLength(double value);
	static double getBaseLength(ProError& status);
	static ProError setVerticalLength(double value);
	static double getVerticalLength(ProError& status);
	static ProError setExtrusionLength(double value);
	static double getExtrusionLength(ProError& status);
	static ProError setThickness(double value);
	static double getThickness(ProError& status);
	static ProError setDstFrstRef(double value);
	static double getDstFrstRef(ProError& status);
	static ProError setDstScndRef(double value);
	static double getDstScndRef(ProError& status);
	static ProError setDiameter(double value);
	static double getDiameter(ProError& status);
};

#endif