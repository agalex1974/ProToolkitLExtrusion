#ifndef UTILITY_HELPER_CLASS
#define UTILITY_HELPER_CLASS

#include <ProMdl.h>
#include <ProElement.h>
#include "HelperStructures.h"

class UtilityHelperClass
{
public:
	static ProError ProUtilSelectionFromSurfaceId(ProMdl model, int surf_id, ProSelection *sel);
	static ProError ProUtilElemtreeCreate(ElemTreeData *elemarr, int n_elem, ProElement	base_tree,
		ProElement *elem_tree);
	static ProError ProUtilFeatureSetSectionDependentValues(ProElement elemtree, ElemTreeData* elemarr,
		int	n_elem);
	static ProError ProUtilModelitemNameSet(ProModelitem *modelitem, char *name);
	static ProError ProUtilModelitemByNameVisit(ProModelitem *modelitem, ProError err, ProAppData app_data);
	static int ProUtilStrwscmp(char *s1, wchar_t *ws2);
	static int ProUtilWstrcmp(wchar_t *ws1, wchar_t *ws2);
	static ProError ProUtilFeatsByName(ProSolid model, FeatByName *feats, int num_feats);
	static ProError ProUtilFeatFirstGeomitem(ProFeature *feature, ProType type, int  *item_id);
	static ProError ProUtilFeatFirstGeomitemVisit(ProGeomitem *geom_item, ProError err, ProAppData app_data);
	static ProError ProUtilFeatFirstDimension(ProFeature *feature, int  *item_id);
	static ProError ProUtilFeatFirstDimensionVisit(ProDimension *dim, ProError err, ProAppData app_data);
	static ProError ProUtilGeometryAtPointFind(ProPart part, Pro3dPnt point, ProModelitem *modelitem);
	static ProError ProUtilAxisGeomitem(ProFeature *feature_axis, ProType type, int  *featid);
	static ProError ProUtilAxisGeomitemVisit(ProGeomitem *geomitem, ProError err, ProAppData appdata);
};


#endif
