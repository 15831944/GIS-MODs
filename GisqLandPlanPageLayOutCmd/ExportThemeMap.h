#pragma once
#include "..\include\tinyxml.h"
#include "..\include\Global.h"

//A0ֽ�Ŵ�С�������жϳ�ͼ��Χ,��λ������
#define A0WIDTH 84.1
//#define A0HEIGHT 118.9
//��γ�ȵ�ƽ���ת��,1:1ʱ��γ��תƽ�棬1cm��Ӧ�ķ�Χ
#define HEIGHT_SCALE_80 11119487.219999390
#define WIDTH_SCALE_80 11119487.218852831
//ֽ������
#define PAGE_HEAD 8
#define PAGE_BUTTON 7
#define PAGE_LEFT_RIGHT 2.5
//��ͼ��ΧС��A0���ݲ�,��λcm
#define PAGE_ALLOW 5
//ʾ��ͼ��ͼ���ĸ����ݲ�,Ԫ�س��ȵİٷֱȣ�1Ϊ����ȫ������
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
	////����ר��ͼ��������ݼ�
	//void SetDatasetName(CStringArray* pszArrDatasetName);
	//����ר��ͼ��������ͼ
	//���� ipXZWorkspace:������״�����ռ䣨�������ù����ռ䣩
	//���� ipGHWorkspace�����ع滮�����ռ䣨�����õع����������ռ䣩
	//���� ipPageLayout��������ͼ
	//���� strXZQDM:������ͼ������������
	//���� strMapConfig�������ļ��С��顱�ġ���-ֵ���ԣ�����ÿ��������ͼ�е�������Ϣ
	//���� strArrDatasetName:ר��ͼͼ������
	//˵�����ú���Ŀǰ�Ѿ�����ʹ�ã�ʹ��ExportThemeMap2(...)����ExportThemeMap(...)
	void ExportThemeMap(CComQIPtr<IWorkspace> ipXZWorkspace,
						CComQIPtr<IWorkspace> ipGHWorkspace,
						CString strXZQDM,
						CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
						CStringArray& strArrDatasetName);
	//add by lh 2016.09.18 begin
	//void SetDatasetName(CStringArray* pszArrDatasetName);
	//����ר��ͼ��������ͼ
	//���� ipXZWorkspace:������״�����ռ䣨�������ù����ռ䣩
	//���� ipGHWorkspace�����ع滮�����ռ䣨�����õع����������ռ䣩
	//���� ipPageLayout��������ͼ
	//���� strXZQDM:������ͼ������������
	//���� strConfigFile��������ͼ�����ļ�ȫ·��
	//���� strRootName�����������ļ��еĸ��ڵ�����
	//���� strArrDatasetName:ר��ͼͼ������
	void ExportThemeMap2(CComQIPtr<IWorkspace> ipXZWorkspace,
		CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CString strConfigFile,
		CString strRootName,
		CStringArray& strArrDatasetName);
	//��ʼ��ר��ͼ������Ϣ
	//���� strConfigFile��������ͼ�����ļ�ȫ·��
	//���� strRootName�������ļ��еĸ��ڵ�����
	//���� strMapConfig�������ļ��С��顱�ġ���-ֵ���ԣ�����ÿ��������ͼ�е�������Ϣ
	BOOL InitViewConfig(CString strConfigFile,
						CString strRootName,
						CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	//add by lh 2016.09.18 end
	
	void ExportThemeMap3(CComQIPtr<IWorkspace> ipXZWorkspace,
		CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
		CStringArray& strArrDatasetName);

	//edit by lcc 2016.12.20 �޸�����ͼ������ú�����ƥ���߼�������ļ�����ʷ�汾������
	void ExportThemeMap4(CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	void ExportJBTMap(CComQIPtr<IWorkspace> ipGHWorkspace,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	void ExportThemeMapTDZZ(CComQIPtr<IWorkspace> ipGHWorkspace,
		CString strXZQDM,
		CComQIPtr<ILayer> spZDQYLayer,
		CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	//edit by lcc 2017.2.2 �޸���ͼ����߼�������Ӧֽ�ţ������ߣ�ͼ��λ�ã�������ʷ�汾����
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
	//���ݸ������Ľ����괴������
	HRESULT GetPolygonByPoint(double MaxX, double MaxY, double MinX, double MinY, CComQIPtr<IPolygon> &spPolygon);
	//���ݱȽϸ���ͼ��������Ľ������γɵľ����Ƿ����ཻ
	HRESULT CheckCover(CComQIPtr<IFeatureLayer> &spXZQLayer, double MaxX, double MaxY, double MinX, double MinY);
	//��ȡ�������ͼ��ͼ���Ľ�����
	HRESULT GetNewActiveViewPoint(double *MaxX, double *MaxY, double *MinX, double *MinY,CComQIPtr<IActiveView> &spActiveView);
	//���ݸ������������ڻ��ͼĳ�����ϵľ����Ľ�����
	HRESULT GetPolygonPoint(CComQIPtr<IActiveView> &spActiveView, double Width, double Height, ElementPosition Position
							,double *PolygonMaxX, double *PolygonMaxY, double *PolygonMinX, double *PolygonMinY);
	//��ȡ����Ԫ�س���
	HRESULT GetTextElementSize(CComQIPtr<IElement> &spElement, CComQIPtr<IActiveView> spActiveView, double *dbWidth, double *dbHeight);
	//��ȡͼ��ֽ���С
	HRESULT GetFrameSize(CComQIPtr<IFeatureLayer> &spXZQFtLayer, double dbScale, double *dbWidth, double *dbHeight);
	//��ȡͼ�泤����ֽ�泤�ȵı�ֵ
	HRESULT GetSizeScale(CComQIPtr<IMapFrame> &spMapFrame, double *dbScale);
	//���ݸ�����������Ԫ��Ӧ��������߻����ұ�
	HRESULT GetDistance(CComQIPtr<IFeatureLayer> &spXZQFtLayer, CComQIPtr<IGeometry> spGeometry, double *dbDistance);
	//�ƶ�ͼ��
	HRESULT MoveLegendElement(CComQIPtr<IPageLayoutControl> spPageLayoutControl, CComQIPtr<IFeatureLayer> &spXZQXZFtLayer,CComQIPtr<IPoint> spActiveViewCenter);
	//�������ڴ������ţ�����������Ҫ���������ͼ���ϲ�ͬ��ͶӰ
	CComQIPtr<ISpatialReference> m_spSpatialRederence;//�������
	void CreateSpatial(int nProjCS);

	//����������ڱ���ȫ������
	//��������
	double dbXZScale,dbXScale,dbZXCQScale;
	//�ŷ�Χ����
	double dbMaxPageWidth,dbMinPageWidth;
	//ֽ������
	double dbPageHead,dbPageButton,dbPage_LeftRight;
	//ͼ������
	double dbFrameBlank;
	//���ε���Ԫ�ص�λ����
	double dbMoveLenth;

	//��������ȫͼ��Χ
	void SetFullExtent(CComQIPtr<IEnvelope> &spEnvelope, BOOL bIsJBT);
private:
	//CStringArray* m_pszArrDatasetName;
	CComQIPtr<GisqLib::_DGisqFrameworkCOM>	        m_ipGisqFramework;
};
