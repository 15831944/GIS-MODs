#pragma once
#include "..\include\tinyxml.h"
#include "..\include\Global.h"

//A0纸张大小，用于判断出图范围,单位：厘米
#define A0WIDTH 84.1
//#define A0HEIGHT 118.9
//经纬度到平面的转换,1:1时经纬度转平面，1cm对应的范围
#define HEIGHT_SCALE_80 11119487.219999390
#define WIDTH_SCALE_80 11119487.218852831
//纸面留边
#define PAGE_HEAD 8
#define PAGE_BUTTON 7
#define PAGE_LEFT_RIGHT 2.5
//出图范围小于A0的容差,单位cm
#define PAGE_ALLOW 5
//示意图和图例的覆盖容差,元素长度的百分比，1为允许全部覆盖
//#define PAGE_COVER 0.5
enum ElementPosition
{
	LeftBotton = 1,
	RightBotton = 2,
	LeftTop = 3,
	RightTop = 4
};
class CExportThemeMap
{
public:
	CExportThemeMap(CComQIPtr<GisqLib::_DGisqFrameworkCOM> ipGisqFramework);
	~CExportThemeMap();
	////设置专题图所需的数据集
	//void SetDatasetName(CStringArray* pszArrDatasetName);
	//导出专题图到布局视图
	//参数 ipXZWorkspace:土地现状工作空间（土地利用工作空间）
	//参数 ipGHWorkspace：土地规划工作空间（建设用地管制区工作空间）
	//参数 ipPageLayout：布局视图
	//参数 strXZQDM:树形视图中行政区代码
	//参数 strMapConfig：配置文件中“块”的“键-值”对，保存每个布局视图中的配置信息
	//参数 strArrDatasetName:专题图图层名称
	//说明：该函数目前已经废弃使用，使用ExportThemeMap2(...)代替ExportThemeMap(...)
	void ExportThemeMap(CComQIPtr<IWorkspace> ipXZWorkspace,
						CComQIPtr<IWorkspace> ipGHWorkspace,
						CString strXZQDM,
						CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
						CStringArray& strArrDatasetName);
	//add by lh 2016.09.18 begin
	//void SetDatasetName(CStringArray* pszArrDatasetName);
	//导出专题图到布局视图
	//参数 ipXZWorkspace:土地现状工作空间（土地利用工作空间）
	//参数 ipGHWorkspace：土地规划工作空间（建设用地管制区工作空间）
	//参数 ipPageLayout：布局视图
	//参数 strXZQDM:树形视图中行政区代码
	//参数 strConfigFile：布局视图配置文件全路径
	//参数 strRootName：布局配置文件中的根节点名称
	//参数 strArrDatasetName:专题图图层名称
	void ExportThemeMap2(CComQIPtr<IWorkspace> ipXZWorkspace,
		CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CString strConfigFile,
		CString strRootName,
		CStringArray& strArrDatasetName);
	//初始化专题图配置信息
	//参数 strConfigFile：布局视图配置文件全路径
	//参数 strRootName：配置文件中的根节点名称
	//参数 strMapConfig：配置文件中“块”的“键-值”对，保存每个布局视图中的配置信息
	BOOL InitViewConfig(CString strConfigFile,
						CString strRootName,
						CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	//add by lh 2016.09.18 end
	
	void ExportThemeMap3(CComQIPtr<IWorkspace> ipXZWorkspace,
		CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
		CStringArray& strArrDatasetName);

	//edit by lcc 2016.12.20 修改了制图输出配置和数据匹配逻辑，上面的几个历史版本都废弃
	void ExportThemeMap4(CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	void ExportJBTMap(CComQIPtr<IWorkspace> ipGHWorkspace,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	void ExportThemeMapTDZZ(CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CComQIPtr<ILayer> spZDQYLayer,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	//edit by lcc 2017.2.2 修改制图输出逻辑，自适应纸张，比例尺，图例位置，以上历史版本弃用
	void ExportThemeMap5(CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
		BOOL bIsZXCQ,
		BOOL bIsLockScale);
	void ExportThemeMapTDZZ2(CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CComQIPtr<ILayer> spZDQYLayer,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
		BOOL bIsZXCQ,
		BOOL bIsLockScale);
	//根据给定的四角坐标创建矩形
	HRESULT GetPolygonByPoint(double MaxX, double MaxY, double MinX, double MinY, CComQIPtr<IPolygon> &spPolygon);
	//根据比较给定图层与给定四角坐标形成的矩形是否有相交
	HRESULT CheckCover(CComQIPtr<IFeatureLayer> &spXZQLayer, double MaxX, double MaxY, double MinX, double MinY);
	//获取给定活动视图的图面四角坐标
	HRESULT GetNewActiveViewPoint(double *MaxX, double *MaxY, double *MinX, double *MinY,CComQIPtr<IActiveView> &spActiveView);
	//根据给定长宽，计算在活动视图某个角上的矩形四角坐标
	HRESULT GetPolygonPoint(CComQIPtr<IActiveView> &spActiveView, double Width, double Height, ElementPosition Position
							,double *PolygonMaxX, double *PolygonMaxY, double *PolygonMinX, double *PolygonMinY);
	//获取文字元素长宽
	HRESULT GetTextElementSize(CComQIPtr<IElement> &spElement, CComQIPtr<IActiveView> spActiveView, double *dbWidth, double *dbHeight);
	//获取图框纸面大小
	HRESULT GetFrameSize(CComQIPtr<IFeatureLayer> &spXZQFtLayer, double dbScale, double *dbWidth, double *dbHeight);
	//获取图面长度与纸面长度的比值
	HRESULT GetSizeScale(CComQIPtr<IMapFrame> &spMapFrame, double *dbScale);
	//根据给定长宽，计算元素应当放在左边还是右边
	HRESULT GetDistance(CComQIPtr<IFeatureLayer> &spXZQFtLayer, CComQIPtr<IGeometry> spGeometry, double *dbDistance);
	//移动图例
	HRESULT MoveLegendElement(CComQIPtr<IPageLayoutControl> spPageLayoutControl, CComQIPtr<IFeatureLayer> &spXZQXZFtLayer,CComQIPtr<IPoint> spActiveViewCenter);
	//下面用于创建带号，后续可能需要根据情况给图附上不同的投影
	CComQIPtr<ISpatialReference> m_spSpatialRederence;//保存带号
	void CreateSpatial(int nProjCS);

	//下面参数用于保存全局配置
	//例尺配置
	double dbXZScale,dbXScale,dbZXCQScale;
	//张范围配置
	double dbMaxPageWidth,dbMinPageWidth;
	//纸面留边
	double dbPageHead,dbPageButton,dbPage_LeftRight;
	//图框留白
	double dbFrameBlank;
	//单次调整元素的位移量
	double dbMoveLenth;

	//用于设置全图范围
	void SetFullExtent(CComQIPtr<IEnvelope> &spEnvelope, BOOL bIsJBT);
private:
	//CStringArray* m_pszArrDatasetName;
	CComQIPtr<GisqLib::_DGisqFrameworkCOM>	        m_ipGisqFramework;
};
