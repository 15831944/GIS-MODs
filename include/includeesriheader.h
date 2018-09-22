#ifndef __ESRI__ARCSDK_COM_SDK_WINDOWS_H__
#define __ESRI__ARCSDK_COM_SDK_WINDOWS_H__

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#pragma warning(disable: 4146)

//#define  VERIFYDISKINFOR  //是否验证硬盘序列号

#define  ARCENGINE102     //10 or 9.3 or 10.1

#ifdef ARCENGINE102

#import "..\lib\com10.2\esriSystem.olb"                 raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_COLOR", "OLE_HANDLE", "VARTYPE")
#import "..\lib\com10.2\esriSystemUI.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("IProgressDialog", "esriIProgressDialog"),rename("ICommand","esriICommand")
#import "..\lib\com10.2\esriGeometry.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("ISegment","esriISegment")
#import "..\lib\com10.2\esriDisplay.olb"                raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriServer.olb"                 raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriOutput.olb"                 raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriGeoDatabase.olb"            raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("IRow","esriIRow"),rename("ICursor","esriICursor"),rename("IRelationship","esriIRelationship")

#import "..\lib\com10.2\esriGISClient.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriDataSourcesFile.olb"        raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriDataSourcesGDB.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriDataSourcesOleDB.olb"       raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriDataSourcesRaster.olb"      raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriGeoDatabaseDistributed.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids

#import "..\lib\com10.2\esriCarto.olb"                  raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("UINT_PTR"),rename("ITableDefinition","esriITableDefinition"),rename("ICursor","esriICursor"),rename("IRow","esriIRow")

#import "..\lib\com10.2\esriLocation.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriNetworkAnalysis.olb"        raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriNetworkAnalyst.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriGeoAnalyst.olb"             raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esri3DAnalyst.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriGlobeCore.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriSpatialAnalyst.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10.2\esriGeoStatisticalAnalyst.olb"  raw_interfaces_only, raw_native_types, no_namespace, named_guids

#import "..\lib\com10.2\esriControls.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("ICommand","esriICommand")
#import "..\lib\com10.2\esriGeoprocessing.olb"          raw_interfaces_only, raw_native_types, no_namespace, named_guids

//arcgis 许可
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" no_namespace raw_interfaces_only no_implementation rename("esriProductCode", "esriVersionProductCode")

#endif

#ifdef ARCENGINE10

#import "..\lib\com10\esriSystem.olb"                 raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_COLOR", "OLE_HANDLE", "VARTYPE")
#import "..\lib\com10\esriSystemUI.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("IProgressDialog", "esriIProgressDialog"),rename("ICommand","esriICommand")
#import "..\lib\com10\esriGeometry.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("ISegment","esriISegment")
#import "..\lib\com10\esriDisplay.olb"                raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriServer.olb"                 raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriOutput.olb"                 raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriGeoDatabase.olb"            raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("IRow","esriIRow"),rename("ICursor","esriICursor"),rename("IRelationship","esriIRelationship")

#import "..\lib\com10\esriGISClient.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriDataSourcesFile.olb"        raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriDataSourcesGDB.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriDataSourcesOleDB.olb"       raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriDataSourcesRaster.olb"      raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriGeoDatabaseDistributed.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids

#import "..\lib\com10\esriCarto.olb"                  raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("UINT_PTR"),rename("ITableDefinition","esriITableDefinition"),rename("ICursor","esriICursor"),rename("IRow","esriIRow")

#import "..\lib\com10\esriLocation.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriNetworkAnalysis.olb"        raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriNetworkAnalyst.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriGeoAnalyst.olb"             raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esri3DAnalyst.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriGlobeCore.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriSpatialAnalyst.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com10\esriGeoStatisticalAnalyst.olb"  raw_interfaces_only, raw_native_types, no_namespace, named_guids

#import "..\lib\com10\esriControls.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("ICommand","esriICommand")
#import "..\lib\com10\esriGeoprocessing.olb"          raw_interfaces_only, raw_native_types, no_namespace, named_guids
//arcgis 许可
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" no_namespace raw_interfaces_only no_implementation rename("esriProductCode", "esriVersionProductCode")
#endif

#ifdef ARCENGINE93

#import "..\lib\com93\esriSystem.olb"                 raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("OLE_COLOR", "OLE_HANDLE", "VARTYPE")
#import "..\lib\com93\esriSystemUI.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("IProgressDialog", "esriIProgressDialog"),rename("ICommand","esriICommand")
#import "..\lib\com93\esriGeometry.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("ISegment","esriISegment")
#import "..\lib\com93\esriDisplay.olb"                raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriServer.olb"                 raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriOutput.olb"                 raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriGeoDatabase.olb"            raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("IRow","esriIRow"),rename("ICursor","esriICursor"),rename("IRelationship","esriIRelationship")

#import "..\lib\com93\esriGISClient.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriDataSourcesFile.olb"        raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriDataSourcesGDB.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriDataSourcesOleDB.olb"       raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriDataSourcesRaster.olb"      raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriGeoDatabaseDistributed.olb" raw_interfaces_only, raw_native_types, no_namespace, named_guids

#import "..\lib\com93\esriCarto.olb"                  raw_interfaces_only, raw_native_types, no_namespace, named_guids, exclude("UINT_PTR"),rename("ITableDefinition","esriITableDefinition"),rename("ICursor","esriICursor"),rename("IRow","esriIRow")

#import "..\lib\com93\esriLocation.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriNetworkAnalysis.olb"        raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriNetworkAnalyst.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriGeoAnalyst.olb"             raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esri3DAnalyst.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriGlobeCore.olb"              raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriSpatialAnalyst.olb"         raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "..\lib\com93\esriGeoStatisticalAnalyst.olb"  raw_interfaces_only, raw_native_types, no_namespace, named_guids

#import "..\lib\com93\esriControls.olb"               raw_interfaces_only, raw_native_types, no_namespace, named_guids,rename("ICommand","esriICommand")
#import "..\lib\com93\esriGeoprocessing.olb"          raw_interfaces_only, raw_native_types, no_namespace, named_guids

#endif

#define  ICommandPtr          esriICommandPtr
#define  ICommand             esriICommand
#define  IProgressDialogPtr   esriIProgressDialogPtr
#define  IProgressDialog      esriIProgressDialog
#define  IRowPtr              esriIRowPtr
#define  IRow                 esriIRow
#define  ICursorPtr           esriICursorPtr
#define  ICursor              esriICursor
#define  ITableDefinitionPtr  esriITableDefinitionPtr
#define  ITableDefinition     esriITableDefinition
#define  strcpy               strcpy_s

//#define  MessageBox           BCGPMessageBox
//#define  AfxMessageBox        BCGPMessageBox

#pragma warning(pop)

#endif /* __ESRI__ARCSDK_COM_SDK_WINDOWS_H__ */    

