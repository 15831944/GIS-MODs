/********************************************************************
*   Copyright @2016  �㽭���ƿƼ����޹�˾.
*   All  right  reserved.
*   �����ߣ�lidazhang 
*   �������ڣ�2016-09-18
*   ��Ҫ�������¹滮ϵͳ(�������ڼ�����ģ��)
*********************************************************************/  
// dllmain.h : ģ�����������

class CGisqLandPlanCmdModule : public CAtlDllModuleT< CGisqLandPlanCmdModule >
{
public :
	DECLARE_LIBID(LIBID_GisqLandPlanCmdLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_GISQLANDPLANCMD, "{4EC7A37C-35A5-40A3-9361-0C47357C80B8}")
};

extern class CGisqLandPlanCmdModule _AtlModule;
