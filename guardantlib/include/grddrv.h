/////////////////////////////////////////////////////////////////////////////
// grddrv.h
//
// Definitions for Guardant Drivers Installation API.
/////////////////////////////////////////////////////////////////////////////

#if !defined GRD_DRVAPI_H__
#define GRD_DRVAPI_H__

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <windows.h>
#define GRDDRV_API          __stdcall
#else
#error "This Guardant API supports 32/x64-bit compilers only"
#endif

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/////////////////////////////////////////////////////////////////////////////
// Possible flags for GrdDrvInstall and GrdDrvUnInstall

typedef enum tagGRDDRV_INSTALLFLAGS
{
	GRDDRV_INSTALL_ONLY_LATEST   = 0,    // completely silent installation of latest version
	GRDDRV_INSTALL_OVERWRITE     = 1,    // completely silent installation with overwrite
	GRDDRV_INSTALL_UILEVEL_BASIC = 2     // simple progress and error handling
} TGRDDRV_INSTALLFLAGS;

/////////////////////////////////////////////////////////////////////////////
// Possible language ID for GrdDrvInstall

#define GRDDRV_INSTALL_LANGUAGE_RUSSIAN                 0
#define GRDDRV_INSTALL_LANGUAGE_ENGLISH                 1

//+--------------------------------------------------------------------------
// GrdDrvInstall -- Run installation process of Guardant Drivers package.
//
// Parameters:
//  [in]  pcszDriversPath - Path to Guardant Drivers package file. If
//        this parameter contain NULL value, current directory must contain
//        package file.
//  [in]  nFlags - install flags
//  [out] pnRebootRequired - reboot indication flag
//  [in]  nLanguageID - language ID
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvInstallA(LPCSTR pcszDriversPath, UINT nFlags, UINT *pnRebootRequired, UINT nLanguageID);
UINT GRDDRV_API GrdDrvInstallW(LPCWSTR pcszDriversPath, UINT nFlags, UINT *pnRebootRequired, UINT nLanguageID);

#ifdef UNICODE
#define GrdDrvInstall       GrdDrvInstallW
#else
#define GrdDrvInstall       GrdDrvInstallA
#endif // UNICODE

//+--------------------------------------------------------------------------
// GrdDrvUnInstall -- Run uninstallation process of Guardant Drivers
//  package.
//
// Parameters:
//  [in]  pcszDriversPath - Path to Guardant Drivers package file or NULL.
//  [in]  nFlags - uninstall flags [GRDDRV_INSTALL_UILEVEL_BASIC]
//  [out] pnRebootRequired - reboot indication flag
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvUnInstallA(LPCSTR pcszDriversPath, UINT *pnRebootRequired, UINT nFlags);
UINT GRDDRV_API GrdDrvUnInstallW(LPCWSTR pcszDriversPath, UINT *pnRebootRequired, UINT nFlags);

#ifdef UNICODE
#define GrdDrvUnInstall     GrdDrvUnInstallW
#else
#define GrdDrvUnInstall     GrdDrvUnInstallA
#endif // UNICODE

//+--------------------------------------------------------------------------
// GrdDrvIsInstalled -- Check for installed Guardant Drivers package.
//
// Parameters:
//  [out] pnIsInstalled - installed flag
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvIsInstalled(UINT *pnIsInstalled);

//+--------------------------------------------------------------------------
// GrdDrvGetVersion -- Return version of installed Guardant Drivers package.
//
// Parameters:
//  [out] pdwVersion - Guardant Drivers version number
//  [out] pdwBuild - Guardant Drivers build number
//        pReserved - reserved value
//
// Example:
//  Driver version: 05.50.85
//
// Returns:
//  Version = 0550 (MajorVersion = 5 (0x05 HEX), MinorVersion = 80 (0x50 HEX))
//  Build = 85 (0x55 HEX)
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvGetVersion(DWORD *pdwVersion, DWORD *pdwBuild, VOID *pReserved);

//+--------------------------------------------------------------------------
// GrdDrvGetVersionEx -- Return version of installed Guardant Drivers package.
//
// Parameters:
//  [out] pdwMajorVersion - Guardant Drivers major version number
//  [out] pdwMinorVersion - Guardant Drivers minor version number
//  [out] pdwBuild - Guardant Drivers build number
//
// Example:
//  Driver version: 5.50.85
//
// Returns:
//  MajorVersion = 5 (0x05 HEX) MinorVersion = 50 (0x32 HEX) Build = 85 (0x55 HEX)
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvGetVersionEx(DWORD *pdwMajorVersion, DWORD *pdwMinorVersion, DWORD *pdwBuild);

//+--------------------------------------------------------------------------
// GrdDrvSetPortUseState -- Enable or disable use of specified LPT port.
//
// Parameters:
//  [in] nPort - LPT port number
//  [in] nUseState - enable/disable flag
//       pReserved - reserved value
//       pReserved2 - reserved value
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvSetPortUseState(UINT nPort, UINT nUseState, VOID *pReserved, VOID *pReserved2);

//+--------------------------------------------------------------------------
// GrdDrvGetPortUseState -- Retrieve enable/disable state of specified
//  LPT port.
//
// Parameters:
//  [in]  nPort - LPT port number
//  [out] pnUseState - enable/disable flag
//        pReserved - reserved value
//        pReserved2 - reserved value
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvGetPortUseState(UINT nPort, UINT *pnUseState, VOID *pReserved, VOID *pReserved2);

//+--------------------------------------------------------------------------
// GrdDrvSetPrnPortTimeOut -- Set printer port timeout value.
//
// Parameters:
//  [in]  nTimeOutValue - timeout value (in milliseconds)
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvSetPrnPortTimeOut(UINT nTimeOutValue);

//+--------------------------------------------------------------------------
// GrdDrvGetPrnPortTimeOut -- Retrieve printer port timeout value.
//
// Parameters:
//  [out] pnTimeOutValue - timeout value (in milliseconds)
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvGetPrnPortTimeOut(UINT *pnTimeOutValue);

/////////////////////////////////////////////////////////////////////////////
// Windows NT LPT port properties

#define GRDDRV_ALLOCATE_PROPERTY_NT                     0x00
#define GRDDRV_IRQLRAISE_PROPERTY_NT                    0x01

/////////////////////////////////////////////////////////////////////////////
// Windows 9X LPT port properties

#define GRDDRV_ALLOCATE_PROPERTY_9X                     0x0A
#define GRDDRV_IO_RANGE_PROPERTY_9X                     0x0B
#define GRDDRV_CRITICAL_SECTION_PROPERTY_9X             0x0C
#define GRDDRV_MASKING_INTERRUPTS_PROPERTY_9X           0x0D
#define GRDDRV_DISABLE_INTERRUPTS_PROPERTY_9X           0x0E

/////////////////////////////////////////////////////////////////////////////
// Possible values for all properties exclude GRDDRV_IRQLRAISE_PROPERTY_NT

#define GRDDRV_PROPERTY_DISABLE                         0x00
#define GRDDRV_PROPERTY_ENABLE                          0x01

/////////////////////////////////////////////////////////////////////////////
// Value for property GRDDRV_IRQLRAISE_PROPERTY_NT variable from 0 to 31,
// that indicate IRQL level

//+--------------------------------------------------------------------------
// GrdDrvSetPortProperties -- Set printer port properties.
//
// Parameters:
//  [in]  nPort - LPT port number 
//  [in]  nProperty - property
//  [in]  nPropertyValue - property value
//        pReserved - reserved value
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvSetPortProperties(UINT nPort, UINT nProperty, UINT nPropertyValue, VOID *pReserved);

//+--------------------------------------------------------------------------
// GrdDrvGetPortProperties -- Get printer port properties.
//
// Parameters:
//  [in]  nPort - LPT port number 
//  [in]  nProperty - property
//  [out] pnPropertyValue - property value
//        pReserved - reserved value
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvGetPortProperties(UINT nPort, UINT nProperty, UINT *pnPropertyValue, VOID *pReserved);

//+--------------------------------------------------------------------------
// GrdDrvSetPortPropertiesAuto -- Set all printer port properties to
//  their default value.
//
// Parameters:
//  [in]  nPort - LPT port number 
//        pReserved - reserved value
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvSetPortPropertiesAuto(UINT nPort, VOID *pReserved);

// maximum LPT port number supported by the operation system
#define GRDDRV_MAX_LPT_PORTS                            10

typedef enum tagWINDOWS_PLATFORM
{
	PLATFORM_9X,    //  Windows 95,98,ME
	PLATFORM_NT     //  Windows NT,2000,XP

} TWINDOWS_PLATFORM, *PWINDOWS_PLATFORM;

// structure returned by GrdDrvGetSystemInfo
#pragma pack(push,1)
typedef struct tagGRDDRV_SYSTEM_INFORMATION
{
	WORD                wNumLPTPorts;       //  Number LPT ports
	WORD                awLPTPortBase[GRDDRV_MAX_LPT_PORTS];    //  LPT port base
	BYTE                bUSBPresent;        //  Present USB flag
	TWINDOWS_PLATFORM   Platform;           //  Currently running windows platform
	BYTE                bReserved[64];      //  Reserved

} TGRDDRV_SYSTEM_INFORMATION, *PGRDDRV_SYSTEM_INFORMATION;
#pragma pack(pop)

//+--------------------------------------------------------------------------
// GrdDrvGetSystemInfo -- Retrieve system information.
//
// Parameters:
//  [out] pGrdSystemInfo - pointer to system information structure
//
// Returns:
//  NO_ERROR if the call executed successfully;
//  Failure code otherwise.
//---------------------------------------------------------------------------

UINT GRDDRV_API GrdDrvGetSystemInfo(TGRDDRV_SYSTEM_INFORMATION *pGrdSystemInfo);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !GRD_DRVAPI_H__
