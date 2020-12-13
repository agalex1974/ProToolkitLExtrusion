#include <stdlib.h>
#include <ProTKRunTime.h>
#include "UtilityHelperClass.h"

#include <ProUtil.h>
#include <ProModelitem.h>
#include <ProSolid.h>
#include <ProWstring.h>

ProError UtilityHelperClass::ProUtilSelectionFromSurfaceId(ProMdl model,  /* In : model */
                                                           int surf_id,	  /* In : surface id */
                                                           ProSelection *sel)
{
	ProError err;
	ProSurface surf;
	ProModelitem modelitem;

	err = ProSurfaceInit(model, surf_id, &surf);
	if (err != PRO_TK_NO_ERROR)
		return (err);
	err = ProSurfaceToGeomitem((ProSolid)model, surf, (ProGeomitem*)&modelitem);
	if (err != PRO_TK_NO_ERROR)
		return (err);
	err = ProSelectionAlloc(NULL, &modelitem, sel);
	if (err != PRO_TK_NO_ERROR)
		return (err);


	return (PRO_TK_NO_ERROR);
}

/*=============================================================*\
  Function: 	ProUtilElemtreeCreate
  Purpose:		Create element tree by the ElemTreeData structure
  Return :      PRO_TK_NO_ERROR if successfull,
\*=============================================================*/
ProError UtilityHelperClass::ProUtilElemtreeCreate(
	ElemTreeData		*elemarr,	    /* In : The array of ElemTreeData */
	int					n_elem,		    /* In : number of ElemTreeData in elemarr */
	ProElement			base_tree,	    /* In : the base tree */
	ProElement			*elem_tree)	    /* Out : element tree */
{
	ProElement			elem_level[10], elem;
	//ProValue			value;
	int					i, type;
	ProError			err;
	//ProValueDataType	data_type;
	//ProSelection		sel_1;
	ProReference		ref_1;

	if (base_tree != nullptr)
		elem_level[0] = base_tree;

	for (i = 0; i < n_elem; i++) {
		err = ProElementAlloc((ProElemId)elemarr[i].elem_id, &elem);
		//TEST_CALL_REPORT("ProElementAlloc()", "ProUtilElemtreeCreate()", err, err != PRO_TK_NO_ERROR);
		if (err != PRO_TK_NO_ERROR)
			return (err);
		if (elemarr[i].data.type != -1) {
			/*----------------------------------------------------------*\
			Add Element value
			\*----------------------------------------------------------*/
			type = elemarr[i].data.type;

			switch (type)
			{
			case PRO_VALUE_TYPE_INT:
			{
				ProElementIntegerSet(elem, elemarr[i].data.v.i);
				break;
			}
			case PRO_VALUE_TYPE_DOUBLE:
			{
				ProElementDoubleSet(elem, elemarr[i].data.v.d);
				break;
			}
			case PRO_VALUE_TYPE_POINTER:
			{
				ProElementSpecialvalueSet(elem, (ProAppData*)elemarr[i].data.v.p);
				break;
			}
			case PRO_VALUE_TYPE_STRING:
			{
				ProElementStringSet(elem, (char*)elemarr[i].data.v.s);
				break;
			}
			case PRO_VALUE_TYPE_WSTRING:
			{
				ProElementWstringSet(elem, (wchar_t*)elemarr[i].data.v.w);
				break;
			}
			case PRO_VALUE_TYPE_SELECTION:
			{
				ProSelectionToReference(elemarr[i].data.v.r, &ref_1);
				ProElementReferenceSet(elem, ref_1);
				break;
			}
			case PRO_VALUE_TYPE_TRANSFORM:
			{
				ProElementTransformSet(elem, (double(*)[4])elemarr[i].data.v.t);
				break;
			}
			case PRO_VALUE_TYPE_BOOLEAN:
			{
				ProElementBooleanSet(elem, elemarr[i].data.v.b);
				break;
			}
			}
		}
		/*----------------------------------------------------------*\
		Add Element to the tree
		\*----------------------------------------------------------*/
		if (elemarr[i].level != 0)
		{
			err = ProElemtreeElementAdd(elem_level[elemarr[i].level - 1], NULL, elem);
			//TEST_CALL_REPORT("ProElemtreeElementAdd()", "ProUtilElemtreeCreate()", err, err != PRO_TK_NO_ERROR);
		}
		elem_level[elemarr[i].level] = elem;
	}
	elem_tree[0] = elem_level[0];
	return (PRO_TK_NO_ERROR);
}

ProError UtilityHelperClass::ProUtilFeatureSetSectionDependentValues(ProElement	elemtree,
	ElemTreeData*	elemarr,
	int				n_elem)
{
	ElemTreeData*	needed_elems = NULL;
	int				i_elem, i_value;
	int				size;

	ProElempath		elem_path;
	ProElempathItem path_item;
	ProElement		element;
	ProValue		value;

	for (i_elem = 0; i_elem < n_elem; i_elem++) {
		switch (elemarr[i_elem].elem_id) {
		case PRO_E_EXT_SURF_CUT_SOLID_TYPE: {
			ProArrayAlloc(0, sizeof(ElemTreeData), 1, (ProArray*)&needed_elems);
			break;
		}
		case PRO_E_STD_DIRECTION:
		case PRO_E_STD_MATRLSIDE:
		{
			if (needed_elems != NULL)
			{
				ProArrayObjectAdd((ProArray*)&needed_elems, -1, 1, &elemarr[i_elem]);
			}
			break;
		}
		default:
			break;
		}
	}
	if (needed_elems == NULL)
		return PRO_TK_NO_ERROR;

	ProError err = ProArraySizeGet(needed_elems, &size);
	if (err != PRO_TK_NO_ERROR)
		return PRO_TK_NO_ERROR;
	if (size == 0)
	{
		ProArrayFree((ProArray*)&needed_elems);
		return PRO_TK_NO_ERROR;
	}


	for (i_value = 0; i_value < size; i_value++)
	{
		switch (needed_elems[i_value].elem_id)
		{
		case PRO_E_STD_DIRECTION:
		case PRO_E_STD_MATRLSIDE:
		{
			ProElempathAlloc(&elem_path);
			path_item.type = PRO_ELEM_PATH_ITEM_TYPE_ID;
			path_item.path_item.elem_id = needed_elems[i_value].elem_id;

			ProElempathDataSet(elem_path, &path_item, 1);
			ProElemtreeElementGet(elemtree, elem_path, &element);

			/*----------------------------------------------------------*\
			Add Element value
			\*----------------------------------------------------------*/
			ProValueAlloc(&value);
			ProValueDataSet(value, &needed_elems[i_value].data);
			ProElementValueSet(element, value);
			ProElempathFree(&elem_path);

			break;
		}
		}
	}

	ProArrayFree((ProArray*)&needed_elems);
	return PRO_TK_NO_ERROR;
}

/*=============================================================*\
  Function: 	ProUtilModelitemNameSet
  Purpose:	Set new name for the model item
  Return :      as ProModelitemNameSet
\*=============================================================*/
ProError UtilityHelperClass::ProUtilModelitemNameSet(ProModelitem *modelitem, char *name)
{
	ProName w_name;
	ProError err;

	ProStringToWstring(w_name, name);
	err = ProModelitemNameSet(modelitem, w_name);
	return (err);
}

/*=============================================================*\
  Function: 	ProUtilFeatsByName
  Purpose:		to find solid features by name
  Return :      PRO_TK_NO_ERROR if successfull,
\*=============================================================*/
ProError UtilityHelperClass::ProUtilFeatsByName(
	ProSolid model,
	FeatByName *feats,
	int num_feats)
{
	FeatByNameFind	feat_by_name;
	ProError		err;
	int				i;

	feat_by_name.feats = feats;
	feat_by_name.num_feats = num_feats;
	for (i = 0; i < num_feats; i++)
		feats[i].id = -1;

	err = ProSolidFeatVisit((ProSolid)model,
		(ProFeatureVisitAction)ProUtilModelitemByNameVisit, NULL,
		(ProAppData)&feat_by_name);
	return (err);
}

/*====================================================================*\
	FUNCTION :  ProUtilWstrcmp()
	PURPOSE  :  strcmp() but for wide strings, and case-sensitive.
\*====================================================================*/
int UtilityHelperClass::ProUtilWstrcmp(wchar_t *ws1, wchar_t *ws2)
{
	int result, status;
	status = ProWstringCompare(ws1, ws2, PRO_VALUE_UNUSED, &result);
	return (result);
}

/*====================================================================*\
	FUNCTION :	ProUtilStrwscmp()
	PURPOSE  :	Compare a string to a wide string
\*====================================================================*/
int UtilityHelperClass::ProUtilStrwscmp(
	char *s1,
	wchar_t *ws2)
{
	wchar_t* ws1;
	int result;
	int len = strlen(s1);

	ws1 = (wchar_t*)calloc(len + 1, sizeof(wchar_t));
	ProStringToWstring(ws1, s1);

	result = ProUtilWstrcmp(ws1, ws2);

	free(ws1);

	return (result);
}

/*=============================================================*\
  Function: 	ProUtilFeatByNameVisit
  Purpose:	visiting fucntion for ProSolidFeatVisit
  Return :      1 if feature with required name found, 0 otherwise
\*=============================================================*/
ProError UtilityHelperClass::ProUtilModelitemByNameVisit(
	ProModelitem *modelitem,
	ProError err,
	ProAppData app_data)
{
	FeatByNameFind *feat_by_name = (FeatByNameFind *)app_data;
	ProName   name;
	int i;
	ProBoolean vis;

	if (modelitem->type == PRO_FEATURE)
	{
		err = ProFeatureVisibilityGet((ProFeature*)modelitem, &vis);
		if (err != PRO_TK_NO_ERROR || vis == PRO_B_FALSE)
			return (PRO_TK_NO_ERROR);
	}

	err = ProModelitemNameGet(modelitem, name);
	if (err != PRO_TK_NO_ERROR)
		return (PRO_TK_NO_ERROR);

	for (i = 0; i < feat_by_name->num_feats; i++)
	{
		if (ProUtilStrwscmp(feat_by_name->feats[i].name, name) == 0)
			feat_by_name->feats[i].id = modelitem->id;
	}
	return (PRO_TK_NO_ERROR);
}

/*=============================================================*\
  Function: 	ProUtilFeatFirstGeomitem
  Purpose:	Found first GeomItem for the feature
  Return :      PRO_TK_NO_ERROR if successful
\*=============================================================*/
ProError UtilityHelperClass::ProUtilFeatFirstGeomitem(
	ProFeature *feature,	    /* In: the feature */
	ProType type,		    /* In: Geomitem type */
	int  *item_id)		    /*Out: Geomitem id, user's memory */
{
	ProModelitem model_item;
	ProError err;

	err = ProFeatureGeomitemVisit(feature, type,
		ProUtilFeatFirstGeomitemVisit, NULL, (ProAppData)&model_item);

	if (err == 1)
	{
		item_id[0] = model_item.id;
		return (PRO_TK_NO_ERROR);
	}
	return (PRO_TK_E_NOT_FOUND);
}

/*=============================================================*\
  Function: 	ProUtilFeatFirstGeomitemVisit
  Purpose:	Visit function for the ProFeatureGeomitemVisit
  Return :      1 (only first geom item visited)
\*=============================================================*/
ProError UtilityHelperClass::ProUtilFeatFirstGeomitemVisit(
	ProGeomitem *geom_item,
	ProError err,
	ProAppData app_data)
{
	ProGeomitem *surf = (ProGeomitem*)app_data;
	surf[0] = geom_item[0];
	return ((ProError)1);
}

/*=============================================================*\
  Function: 	ProUtilFeatFirstDimensionVisit
  Purpose:	Visit function for the ProFeatureDimensionVisit
  Return :      1 (only first dim visited)
\*=============================================================*/
ProError UtilityHelperClass::ProUtilFeatFirstDimensionVisit(
	ProDimension *dim,
	ProError err,
	ProAppData app_data)
{
	ProDimension *found = (ProDimension*)app_data;
	found[0] = dim[0];
	return ((ProError)1);
}

/*=============================================================*\
  Function: 	ProUtilFeatFirstDimension
  Purpose:	Found first GeomItem for the feature
  Return :      PRO_TK_NO_ERROR if successful
\*=============================================================*/
ProError UtilityHelperClass::ProUtilFeatFirstDimension(
	ProFeature *feature,	    /* In: the feature */
	int  *item_id)		    /*Out: Dimension id, user's memory */
{
	ProModelitem model_item;
	ProError err;

	err = ProFeatureDimensionVisit(feature,
		ProUtilFeatFirstDimensionVisit, NULL, (ProAppData)&model_item);

	if (err == 1)
	{
		item_id[0] = model_item.id;
		return (PRO_TK_NO_ERROR);
	}
	return (PRO_TK_E_NOT_FOUND);
}
/*=============================================================*\
  Function: 	ProUtilGeometryAtPointFind
  Purpose:	to find surface at point
  Return :      PRO_TK_NO_ERROR if successfull,
		PRO_TK_E_NOT_FOUND otherwise
\*=============================================================*/
ProError UtilityHelperClass::ProUtilGeometryAtPointFind(
	ProPart part,
	Pro3dPnt point,
	ProModelitem *modelitem)
{
	int n_sel, i;
	ProSelection *p_sel;
	ProType type;
	ProError err;

	err = ProGeometryAtPointFind(part, point, &p_sel, &n_sel);

	if (err != PRO_TK_NO_ERROR)
		return (PRO_TK_E_NOT_FOUND);

	err = ProSelectionModelitemGet(p_sel[0], modelitem);

	/* Free memory allocated by ProGeometryAtPointFind */
	for (i = 0; i < n_sel; i++)
		ProSelectionFree(&p_sel[i]);

	ProArrayFree((ProArray *)&p_sel);
	return (PRO_TK_NO_ERROR);
}

/*=============================================================*\
  Function: 	ProUtilAxisGeomitem
  Purpose:	Found Axis GeomItem for the axis feature (PRO_FEAT_DATUM_AXIS)
  Return :      PRO_TK_NO_ERROR if successful
\*=============================================================*/
ProError UtilityHelperClass::ProUtilAxisGeomitem(
	ProFeature *feature_axis,	    /* In: the feature */
	ProType type,		    /* In: Geomitem type */
	int  *featid)		    /*Out: Geomitem id  */
{
	ProModelitem modelitem;
	ProError err;

	err = ProFeatureGeomitemVisit(feature_axis, type,
		ProUtilAxisGeomitemVisit, NULL,
		(ProAppData)&modelitem);

	if (err == PRO_TK_E_FOUND)
	{
		featid[0] = modelitem.id;
		return (PRO_TK_NO_ERROR);
	}
	return (PRO_TK_E_NOT_FOUND);
}

/*=============================================================*\
  Function: 	ProUtilAxisGeomitemVisit
  Purpose:	Visit function for the ProFeatureGeomitemVisit
  Return :      only one axis as feature it self is axis
\*=============================================================*/
ProError UtilityHelperClass::ProUtilAxisGeomitemVisit(
	ProGeomitem *geomitem,
	ProError err,
	ProAppData appdata)
{
	ProGeomitem *axis = (ProGeomitem*)appdata;
	axis[0] = geomitem[0];
	err = PRO_TK_E_FOUND;
	return (PRO_TK_E_FOUND);
}