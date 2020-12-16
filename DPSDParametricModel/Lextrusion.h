// Developed by Alexander G. Agathos
// e-mail: alexander.agathos@gmail.com
// MIT license, see the license file in the Git repository.

#ifndef L_EXTRUSION_H
#define L_EXTRUSION_H

class Lextrusion : public Extrusion
{
public:

	/**
	 * Constructor for Lextrusion
	 *
	 * @param model The Creo's part session
	 */
	explicit Lextrusion(ProMdl model);

	/**
	 * Create the Lextrusion given its parameters
	 *
	 * @param BaseLength The base length of the L-shape
	 * @param VerticalLength The vertical length of the L-shape
	 * @param ExtrusionLength The extrusion length of the L-shape
	 * @param Thickness The thickness of the L-shape
	 * @param sketch_plane_id The sketch plane id
	 * @param sketch_orientation_id The orientation id plane
	 * @param cutPlaneId The plane id which creates an ortogonal system on the sketch with sketch_orientation_id
	 */
	void createLextrusion(double BaseLength, double VerticalLength, 
		double ExtrusionLength, double Thickness, int sketch_plane_id, int sketch_orientation_id, int cutPlaneId);

	/**
	 * Returns the side id of the plane used to reference the hole plane
	 *
	 * @return id of the side surface.
	 */
	int GetSideSurfaceId();

	/**
	 * Update the L extrusion
	 *
	 * @param BaseLength The base length of the L-shape
	 * @param VerticalLength The vertical length of the L-shape
	 * @param ExtrusionLength The extrusion length of the L-shape
	 * @param Thickness The thickness of the L-shape
	 * @param sketch_orientation_id The sketch orientation id
	 * @param cutPlaneId The id of the plane whose projection on the sketch will create an orthogonal system with the sketch orientation plane projection
	 */
	void updateLExtrusion(double BaseLength, double VerticalLength, double ExtrusionLength, double Thickness, int sketch_orientation_id, int cutPlaneId);
	
private:

	/**
	 * Update the L-shape on the sketch
	 *
	 * @param section The section of the sketch
	 * @param params The parameters of the sketch
	 * @param LidPolyLine The L-shape polyline as an array of line ids.
	 * @param side_id One of the projections used for drawing.
	 * @param bot_id One of the projections used for drawing.
	 */
	static ProError updateLshape(ProSection section, Parameter* params, int* LidPolyLine, int side_id, int bot_id);

	/**
	 * Update the L-shape on the sketch
     *
     * @param section The section of the sketch
     * @param params The parameters of the sketch
     * @param pLid The L-shape polyline as an array of line ids.
     * @param side_id One of the projections used for drawing id.
     * @param bot_id One of the projections used for drawing id.
     */
	static ProError ProUtilCreateSection(ProSection section, Parameter *params, int* pLid, int* side_id, int* bot_id);

	ProMdl model;		/**< The creo's part session model id. */
	int sideSurfaceId;	/**< The side surface id */
	int Lid[6];			/**< The polyline of the L-shape ids */
	int side_id;		/**< One of the projections used for drawing id. */
	int bot_id;			/**< One of the projections used for drawing id. */
};

#endif