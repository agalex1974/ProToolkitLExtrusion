#ifndef L_EXTRUSION_H
#define L_EXTRUSION_H

class Lextrusion : public Extrusion
{
public:
	explicit Lextrusion(ProMdl model);
	void createLextrusion(double BaseLength, double VerticalLength, 
		double ExtrusionLength, double Thickness, int sketch_plane_id, int sketch_orientation_id, int cutPlaneId);
	int GetSideSurfaceId();
	void updateLExtrusion(double BaseLength, double VerticalLength, double ExtrusionLength, double Thickness, int sketch_orientation_id, int cutPlaneId);
	
private:
	static ProError updateLshape(ProSection section, Parameter *params, int* LidPolyLine, int side_id, int bot_id);
	static ProError ProUtilCreateSection(ProSection section, Parameter *params, int* pLid, int* side_id, int* bot_id);
	ProMdl model;
	int sideSurfaceId;
	int Lid[6];
	int side_id;
	int bot_id;
};

#endif