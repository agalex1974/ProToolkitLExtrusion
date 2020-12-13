#include <stdlib.h>
#include <stdio.h>
#include <ProFeature.h>
#include <ProSecdim.h>
#include "log_define.h"
#include "Extrusion.h"
#include "Lextrusion.h"
#include "UtilityHelperClass.h"

#define PI 3.1415926535897932384626433832795

extern FILE* errlog_fp;

Lextrusion::Lextrusion(ProMdl model)
{
	this->model = model;
}

ProError Lextrusion::ProUtilCreateSection(
	ProSection section,		
	Parameter *params,
	int* LidPolyLine,
	int* side_id, int* bot_id)
{
	int					brk = 0;
	ProError			err;
	ProWSecerror		errors;
	Pro2dEntdef			*ent;
	Pro2dLinedef		*line;
	ProError            status, solve_status;
	do /* Used for exit from the middle of block */
	{
		/*----------------------------------------------------------*\
		Create entities
		\*----------------------------------------------------------*/
		double BaseLength = params[0].d;
		double VerticalLength = params[1].d;
		double Thickness = params[2].d;

		Pro2dPointdef center;
		/* Align the X coordinate of the circle with the side datum plane */
		err = ProSectionEntityFromProjection(section, params[3].r, side_id);
		err = ProSectionEntityGet(section, *side_id, &ent);
		line = (Pro2dLinedef*)ent;
		center.pnt[0] = line->end1[0];
		

		/* Align the Y coordinate of the circle with the bottom datum plane */
		err = ProSectionEntityFromProjection(section, params[4].r, bot_id);
		err = ProSectionEntityGet(section, *bot_id, &ent);
		line = (Pro2dLinedef*)ent;
		center.pnt[1] = line->end1[1];
		
		Pro2dLinedef line1;
		line1.type = PRO_2D_LINE;
		line1.end1[0] = center.pnt[0];
		line1.end1[1] = center.pnt[1];
		line1.end2[0] = center.pnt[0];
		line1.end2[1] = center.pnt[1] + VerticalLength;
		err = ProSectionEntityAdd(section, (Pro2dEntdef*)&line1, &LidPolyLine[0]);
		
		Pro2dLinedef line2;
		line2.type = PRO_2D_LINE;
		line2.end1[0] = center.pnt[0];
		line2.end1[1] = center.pnt[1] + VerticalLength;
		line2.end2[0] = center.pnt[0] + Thickness;
		line2.end2[1] = center.pnt[1] + VerticalLength;
		err = ProSectionEntityAdd(section, (Pro2dEntdef*)&line2, &LidPolyLine[1]);

		Pro2dLinedef line3;
		line3.type = PRO_2D_LINE;
		line3.end1[0] = center.pnt[0] + Thickness;
		line3.end1[1] = center.pnt[1] + VerticalLength;
		line3.end2[0] = center.pnt[0] + Thickness;
		line3.end2[1] = center.pnt[1] + Thickness;
		err = ProSectionEntityAdd(section, (Pro2dEntdef*)&line3, &LidPolyLine[2]);

		Pro2dLinedef line4;
		line4.type = PRO_2D_LINE;
		line4.end1[0] = center.pnt[0] + Thickness;
		line4.end1[1] = center.pnt[1] + Thickness;
		line4.end2[0] = center.pnt[0] + BaseLength;
		line4.end2[1] = center.pnt[1] + Thickness;
		err = ProSectionEntityAdd(section, (Pro2dEntdef*)&line4, &LidPolyLine[3]);

		Pro2dLinedef line5;
		line5.type = PRO_2D_LINE;
		line5.end1[0] = center.pnt[0] + BaseLength;
		line5.end1[1] = center.pnt[1] + Thickness;
		line5.end2[0] = center.pnt[0] + BaseLength;
		line5.end2[1] = center.pnt[1];
		err = ProSectionEntityAdd(section, (Pro2dEntdef*)&line5, &LidPolyLine[4]);

		Pro2dLinedef line6;
		line6.type = PRO_2D_LINE;
		line6.end1[0] = center.pnt[0] + BaseLength;
		line6.end1[1] = center.pnt[1];
		line6.end2[0] = center.pnt[0];
		line6.end2[1] = center.pnt[1];
		err = ProSectionEntityAdd(section, (Pro2dEntdef*)&line6, &LidPolyLine[5]);

		/*----------------------------------------------------------*\
		Solve section
		\*----------------------------------------------------------*/
		err = ProSecerrorAlloc(&errors);
		err = ProSectionAutodim(section, &errors);
		err = ProSecerrorFree(&errors);
	} while (brk);
	return (err);
}

void Lextrusion::updateLExtrusion(double BaseLength, double VerticalLength, double ExtrusionLength, double Thickness, int sketch_orientation_id, int cutPlaneId)
{
	updateExtrusionDepth(model, ExtrusionLength);
	Parameter params[5];
	params[0].d = BaseLength;
	params[1].d = VerticalLength;
	params[2].d = Thickness;
	params[3].r = tree[10].data.v.r;

	//Here you tell that the fature will reside on the bottom plane.
	UtilityHelperClass::ProUtilSelectionFromSurfaceId(model, cutPlaneId, &params[4].r);
	
	updateSketchFunction = updateLshape;
	updateExtrusionSketch(model, params, Lid, side_id, bot_id);

	ProError status = ProSelectionFree(&params[4].r);
}

ProError Lextrusion::updateLshape(ProSection section, Parameter *params, int* LidPolyLine, int side_id, int bot_id)
{
	Pro2dEntdef	*ent;
	Pro2dLinedef* line;
	int Lid;
	Pro2dPointdef center;
	ProError status;
	ProWSecerror sec_errors;

	Pro2dPnt place_pnt = { 0.0, 0.0 };
	center.pnt[0] = 0.0;
	center.pnt[1] = 0.0;

	double BaseLength = params[0].d;
	double VerticalLength = params[1].d;
	double Thickness = params[2].d;

	fprintf(errlog_fp, "BaseLength:%lf\n", BaseLength);
	fprintf(errlog_fp, "VerticalLength:%lf\n", VerticalLength);
	fprintf(errlog_fp, "Thickness:%lf\n", Thickness);

	int ent_ids[2];
	int dim_id;
	ProSectionPointType pnt_types[2];

	Pro2dPnt place_pnt2 = { Thickness / 3.0, VerticalLength + 1.0 };
	Pro2dLinedef line2;
	line2.type = PRO_2D_LINE;
	line2.end1[0] = center.pnt[0];
	line2.end1[1] = center.pnt[1] + VerticalLength;
	line2.end2[0] = center.pnt[0] + Thickness;
	line2.end2[1] = center.pnt[1] + VerticalLength;
	status = ProSectionEntityAdd(section, (Pro2dEntdef*)&line2, &Lid);
	status = ProSectionEntityReplace(section, LidPolyLine[1], Lid);
	ent_ids[0] = bot_id;
	ent_ids[1] = LidPolyLine[1];
	pnt_types[0] = PRO_ENT_WHOLE;
	pnt_types[1] = PRO_ENT_WHOLE;
	status = ProSecdimCreate(section, ent_ids, pnt_types, 2, PRO_TK_DIM_LINE_LINE, place_pnt2, &dim_id);
	
	Pro2dPnt place_pnt5 = { BaseLength + 1.0, Thickness / 3.0 };
	Pro2dLinedef line5;
	line5.type = PRO_2D_LINE;
	line5.end1[0] = center.pnt[0] + BaseLength;
	line5.end1[1] = center.pnt[1] + Thickness;
	line5.end2[0] = center.pnt[0] + BaseLength;
	line5.end2[1] = center.pnt[1];
	status = ProSectionEntityAdd(section, (Pro2dEntdef*)&line5, &Lid);
	status = ProSectionEntityReplace(section, LidPolyLine[4], Lid);
	ent_ids[0] = LidPolyLine[4];
	ent_ids[1] = side_id;
	pnt_types[0] = PRO_ENT_WHOLE;
	pnt_types[1] = PRO_ENT_WHOLE;
	status = ProSecdimCreate(section, ent_ids, pnt_types, 2, PRO_TK_DIM_LINE_LINE, place_pnt5, &dim_id);
	
	Pro2dPnt place_pnt4 = { (BaseLength + Thickness) / 2.0, Thickness + 1};
	Pro2dLinedef line4;
	line4.type = PRO_2D_LINE;
	line4.end1[0] = center.pnt[0] + Thickness;
	line4.end1[1] = center.pnt[1] + Thickness;
	line4.end2[0] = center.pnt[0] + BaseLength;
	line4.end2[1] = center.pnt[1] + Thickness;
	status = ProSectionEntityAdd(section, (Pro2dEntdef*)&line4, &Lid);
	status = ProSectionEntityReplace(section, LidPolyLine[3], Lid);
	ent_ids[0] = LidPolyLine[3];
	ent_ids[1] = bot_id;
	pnt_types[0] = PRO_ENT_WHOLE;
	pnt_types[1] = PRO_ENT_WHOLE;
	status = ProSecdimCreate(section, ent_ids, pnt_types, 2, PRO_TK_DIM_LINE_LINE, place_pnt4, &dim_id);

	Pro2dPnt place_pnt3 = { Thickness + 1.0, (Thickness + VerticalLength) / 2.0 };
	Pro2dLinedef line3;
	line3.type = PRO_2D_LINE;
	line3.end1[0] = center.pnt[0] + Thickness;
	line3.end1[1] = center.pnt[1] + VerticalLength;
	line3.end2[0] = center.pnt[0] + Thickness;
	line3.end2[1] = center.pnt[1] + Thickness;
	status = ProSectionEntityAdd(section, (Pro2dEntdef*)&line3, &Lid);
	status = ProSectionEntityReplace(section, LidPolyLine[2], Lid);
	ent_ids[0] = LidPolyLine[2];
	ent_ids[1] = side_id;
	pnt_types[0] = PRO_ENT_WHOLE;
	pnt_types[1] = PRO_ENT_WHOLE;
	//status = ProSecdimCreate(section, ent_ids, pnt_types, 2, PRO_TK_DIM_LINE_LINE, place_pnt3, &dim_id);
	
	Pro2dPnt place_pnt1 = { -1.0, VerticalLength / 2.0 };
	Pro2dLinedef line1;
	line1.type = PRO_2D_LINE;
	line1.end1[0] = center.pnt[0];
	line1.end1[1] = center.pnt[1];
	line1.end2[0] = center.pnt[0];
	line1.end2[1] = center.pnt[1] + VerticalLength;
	status = ProSectionEntityAdd(section, (Pro2dEntdef*)&line1, &Lid);
	status = ProSectionEntityReplace(section, LidPolyLine[0], Lid);
	ent_ids[0] = LidPolyLine[0];
	ent_ids[1] = side_id;
	pnt_types[0] = PRO_ENT_WHOLE;
	pnt_types[1] = PRO_ENT_WHOLE;
	//status = ProSecdimCreate(section, ent_ids, pnt_types, 2, PRO_TK_DIM_LINE_LINE, place_pnt1, &dim_id);
	
	Pro2dPnt place_pnt6 = { BaseLength / 2.0, -1.0 };
	Pro2dLinedef line6;
	line6.type = PRO_2D_LINE;
	line6.end1[0] = center.pnt[0] + BaseLength;
	line6.end1[1] = center.pnt[1];
	line6.end2[0] = center.pnt[0];
	line6.end2[1] = center.pnt[1];
	status = ProSectionEntityAdd(section, (Pro2dEntdef*)&line6, &Lid);
	status = ProSectionEntityReplace(section, LidPolyLine[5], Lid);
	ent_ids[0] = LidPolyLine[5];
	ent_ids[1] = bot_id;
	pnt_types[0] = PRO_ENT_WHOLE;
	pnt_types[1] = PRO_ENT_WHOLE;
	//status = ProSecdimCreate(section, ent_ids, pnt_types, 2, PRO_TK_DIM_LINE_LINE, place_pnt6, &dim_id);
	
	/* Allocate the error list */
	status = ProSecerrorAlloc(&sec_errors);
	/* Add all the missing dimensions */
	status = ProSectionSolve(section, &sec_errors);
	/* Regenerate the section */
	status = ProSectionRegenerate(section, &sec_errors);
	/* Release the error list */
	ProSecerrorFree(&sec_errors);

	return status;
}

void Lextrusion::createLextrusion(double BaseLength, double VerticalLength,
	double ExtrusionLength, double Thickness, int sketch_plane_id, int sketch_orientation_id, int cutPlaneId)
{
	setTheDrawingPlaneReference(model, sketch_plane_id);
	setTheDrawingPlaneOrientation(model, sketch_orientation_id);

	setExtrusionDepth(ExtrusionLength);
	setDrawingFunction(ProUtilCreateSection);
	Parameter params[5];
	params[0].d = BaseLength;
	params[1].d = VerticalLength;
	params[2].d = Thickness;
	params[3].r = tree[10].data.v.r;

	//Here you tell that the fature will reside on the bottom plane.
	UtilityHelperClass::ProUtilSelectionFromSurfaceId(model, cutPlaneId, &params[4].r);
	createExtrusion(model, params, Lid, "Lextrusion", &side_id, &bot_id);

	ProError status = ProSelectionFree(&params[4].r);
	PT_TEST_LOG_SUCC("ProSelectionFree CutPlane");
	
	Pro3dPnt pnt;
	pnt[0] = 0.0;
	pnt[1] = VerticalLength / 2.0;
	pnt[2] = ExtrusionLength / 2.0;

	ProModelitem modelitem;
	
	status = UtilityHelperClass::ProUtilGeometryAtPointFind((ProPart)model, pnt, &modelitem);
	PT_TEST_LOG_SUCC("ProUtilGeometryAtPointFind");

	sideSurfaceId = modelitem.id;

	status = UtilityHelperClass::ProUtilModelitemNameSet(&modelitem, const_cast<char*>("SIDE_SUFACE"));
	PT_TEST_LOG_SUCC("ProUtilModelitemNameSet...LEXtrusion SIDE SURFACE)")
}

int Lextrusion::GetSideSurfaceId()
{
	return sideSurfaceId;
}
