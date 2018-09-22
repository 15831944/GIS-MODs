#pragma once
#define DEFAULT_DATA_FOLDER    _T("\\data\\")
#include "map"
using namespace std;

//定义区县等级
#define DISTRICT_TREE_SHENG_ICON	    0
#define DISTRICT_TREE_SHI_ICON	            1
#define DISTRICT_TREE_XIAN_ICON		    2
#define DISTRICT_TREE_QU_ICON		        3
#define DISTRICT_TREE_XIANG_ICON		4
#define DISTRICT_TREE_ZHEN_ICON				5
#define DISTRICT_TREE_JIEDAO_IOC			5
#define DISTRICT_TREE_CUN_ICON		    6

#define ICON_INDEX_RULE				1
//图标
#define ICON_INDEX_CATEGORY         0
#define ICON_INDEX_FOLDER           1
#define ICON_INDEX_MXD              2
#define ICON_INDEX_PDF              3
#define ICON_INDEX_TXET             4
#define ICON_INDEX_WORD             5
#define ICON_INDEX_EXCEL            6
#define ICON_INDEX_ACCESS           7
#define ICON_INDEX_JPG              8
#define ICON_INDEX_BMP              9
#define ICON_INDEX_TIF              10
#define ICON_INDEX_PNG              11
#define ICON_INDEX_HTM              12
#define ICON_INDEX_CHM              13
#define ICON_INDEX_AVI              14
#define ICON_INDEX_POWERPOINT       15
#define ICON_INDEX_PROJECT          16
#define ICON_INDEX_VISIO            17
#define ICON_INDEX_LAYER            18
#define ICON_INDEX_NNN              19
#define ICON_INDEX_RAR              20
#define ICON_INDEX_MAPS             21
#define ICON_INDEX_DATABASE         22
#define ICON_INDEX_DATASET          23
#define ICON_INDEX_TABLE            24
#define ICON_INDEX_CHECKED          25
#define ICON_INDEX_UNCHECKED        26
#define ICON_INDEX_TOPO             27
#define ICON_INDEX_RASTER           28
#define ICON_INDEX_EDIT             29
#define ICON_INDEX_XML             30
#define ICON_INDEX_TASK            61


//系统配置表相对位置
#define   DATACHECK_CONFIG_PATH     _T("Data\\Task.mdb")
#define   DATACHECK_RULEFILE_PATH   _T("Data\\Rule.mdb")

//表定义区
//行政区编码表
#define   TABLE_REGION_CODE              _T("RegionCode")
#define   FIELD_REGIONCODE_XZQBM   _T("XZQBM")
#define   FIELD_REGIONCODE_XZQMC   _T("XZQMC")
#define   FIELD_REGIONCODE_PARENTID    _T("Parent_ID")
#define   FIELD_REGIONCODE_VALIDATE     _T("Validate")
#define   FIELD_REGIONCODE_LEVEL           _T("Level")

//行政区结构
typedef struct _tagDistrictItem
{
	CString   strName;      //行政区名称
	CString   strCode;            //行政区代码
	CString   strParentID;  //父节点代码
	int          nLevel;          //行政区等级

	_tagDistrictItem()
	{
		strCode    = _T("");
		strName   = _T("");
		nLevel      = 0; 
		strParentID= _T("");
	}
} tagDistrictItem;

//文件的枚举类型
typedef enum
{
  FT_Mdb  = 0,
  FT_Gdb   = 1,
  FT_E00	= 2,
  FT_Shp	= 3,
  FT_Other	= 4,
}enumFileType;