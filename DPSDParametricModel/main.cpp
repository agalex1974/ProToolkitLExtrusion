#include <ProUI.h>
#include <ProUICmd.h>
#include <ProMessage.h>
#include <ProMdl.h>
#include <ProMenuBar.h>
#include <ProUtil.h>
#include "log_define.h"
#include "Extrusion.h"
#include "Lextrusion.h"
#include "DatumPlane.h"
#include "Axis.h"
#include "Hole.h"
#include "ParametricModel.h"
#include "GuiCInterface.h"

#ifdef __cplusplus
extern "C" {
#endif
	int user_initialize(int	argc, char *argv[], char *version, char	*build, wchar_t errbuf[80]);
	void user_terminate();
#ifdef __cplusplus
}
#endif

bool created = false;
GuiCInterface* gui;
FILE* errlog_fp;
ParametricModel* parametricModel;
const wchar_t* MSGFIL = L"messages.txt";

/*=============================================================*\
  Function: 	ProTKGDAccess
  Purpose:		Determine if gear design should be accessible from menus
\*=============================================================*/
static uiCmdAccessState accessFunction(uiCmdAccessMode access_mode)
{
	ProMdl current;
	ProMdlType type;
	ProError status;

	status = ProMdlCurrentGet(&current);
	PT_TEST_LOG_SUCC("ProMdlCurrentGet...........uiCmdAccessState")

	if (status != PRO_TK_NO_ERROR)
		return ACCESS_INVISIBLE;

	status = ProMdlTypeGet(current, &type);
	PT_TEST_LOG_SUCC("ProMdlTypeGet")

	if (status != PRO_TK_NO_ERROR || type != PRO_MDL_PART)
		return ACCESS_INVISIBLE;
	if (!created) {
		parametricModel = new ParametricModel();
		gui = new GuiCInterface(parametricModel);
		created = true;
	}
	
	return ACCESS_AVAILABLE;
}

/*=============================================================*\
  Function: 	user_initialize
  Purpose:		Create menu button
\*=============================================================*/
int user_initialize(
	int		argc,			 /* Inp: Pro/E arg count */
	char	*argv[],		 /* Inp: Pro/E args	 */
	char	*version,		 /* Inp: Pro/E version	 */
	char	*build, 		 /* Inp: Pro/E build date code */
	wchar_t errbuf[80])		 /* Out: error message (opt)   */
{
	/* Declare external functions */
	int wchar_size;
	ProError status;
	uiCmdCmdId cmd_id;
	errlog_fp = fopen("C:\\Users\\agalex\\Desktop\\Coding\\DPSDParametricModel\\x64\\Release\\ParametricModel.log", "w");
	//errlog_fp = fopen("GearDesign.log", "w");
	
	if (ProWcharSizeVerify(sizeof(wchar_t), &wchar_size) != PRO_TK_NO_ERROR)
	{
		ProMessageDisplay(const_cast<wchar_t*>(MSGFIL), const_cast<char*>("Parametric Design %0s"), 
			"Incorrect size of 'wchar_t'.");
		return(-1);
	}

	status = ProCmdActionAdd(const_cast<char*>("ParametricModelDesign"),
		(uiCmdCmdActFn)(gui->GuiActivate),
		uiProe2ndImmediate, accessFunction,
		PRO_B_FALSE, PRO_B_FALSE, &cmd_id);
	PT_TEST_LOG_SUCC("ProCmdActionAdd.........user_initialize")

	status = ProMenubarmenuPushbuttonAdd(
		const_cast<char*>("Applications"), const_cast<char*>("-ParametricUI"), 
		const_cast<char*>("-ParametricUI"), const_cast<char*>("-ParametricUIHelp"),
		nullptr, PRO_B_TRUE, cmd_id, const_cast<wchar_t*>(MSGFIL));
	PT_TEST_LOG_SUCC("ProMenubarmenuPushbuttonAdd")

	return (0);
}

/*=============================================================*\
  Function: 	user_terminate
  Purpose:	To handle any termination actions
\*=============================================================*/
void user_terminate()
{
	delete gui;
	fclose(errlog_fp);
}