/********************************************************************
*   Copyright @2016  浙江臻善科技有限公司.
*   All  right  reserved.
*   创建者：lidazhang 
*   创建日期：2016-09-18
*   简要描述：新规划系统(制图模块)
*********************************************************************/  
// dllmain.h : 模块类的声明。

class CGisqLandPlanPageLayOutCmdModule : public CAtlDllModuleT< CGisqLandPlanPageLayOutCmdModule >
{
public :
	DECLARE_LIBID(LIBID_GisqLandPlanPageLayOutCmdLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_GISQLANDPLANPAGELAYOUTCMD, "{D36167D4-5FE3-41DA-A95D-97F60236C823}")
};

extern class CGisqLandPlanPageLayOutCmdModule _AtlModule;
