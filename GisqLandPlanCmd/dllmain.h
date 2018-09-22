/********************************************************************
*   Copyright @2016  浙江臻善科技有限公司.
*   All  right  reserved.
*   创建者：lidazhang 
*   创建日期：2016-09-18
*   简要描述：新规划系统(浮动窗口及命令模块)
*********************************************************************/  
// dllmain.h : 模块类的声明。

class CGisqLandPlanCmdModule : public CAtlDllModuleT< CGisqLandPlanCmdModule >
{
public :
	DECLARE_LIBID(LIBID_GisqLandPlanCmdLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_GISQLANDPLANCMD, "{4EC7A37C-35A5-40A3-9361-0C47357C80B8}")
};

extern class CGisqLandPlanCmdModule _AtlModule;
