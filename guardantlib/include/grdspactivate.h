#ifndef __GRDSKACTIVATE_H__
#define __GRDSKACTIVATE_H__

#include <stdint.h>

#define GrdVD_OK                                  0
#define GrdVD_InvalidSerialNumber                 1 //Serial number is invalid
#define GrdVD_KeyResourceExhaust                  2 //Dongle activation resource is exhausted
#define GrdVD_SerialNumberBlocked                 3 //Serial number is blocekd by vendor
#define GrdVD_AlreadyActivated                    4 //License file has been already activated
#define GrdVD_InvalidVendor                       5 //Specified serial number is registetred for another vendor
#define GrdVD_InvalidProgramNumber                6 //Specified serial number is registetred for another product
#define GrdVD_SerialNumberLenWrong              101 //Invalid serial number length
#define GrdVD_InvalidArg                        102 //Invalid parameter specified in API function
#define GrdVD_InvalidFileFormat                 112 //Invalid license file format
#define GrdVD_SystemError                       123 //System error
#define GrdVD_GetPCNError                       124 //Unable to get unique PCN
#define GrdVD_InvalidBufferSize                 125 //Invalid buffer size
#define GrdVD_ActivationError                   126 //Unable to activate container
#define GrdVD_InvalidVendorPassword             127 //Invalid access code
#define GrdVD_GuardantDongleError               128 //Guardant dongle error
#define GrdVD_ActivationRequestFailed           201 //Unable to send activation request
#define GrdVD_ActivationResponseFailed          202 //Unable to receive activation response
#define GrdVD_ActivationStatusNotFound          203 //Unable to find activation server
#define GrdVD_ActivationException               210 //Activation server internnal error
#define GrdSKE_InvalidCmdLineParameter          300 // Invalid command line parameter
#define GrdSKE_ErrorLoadLicenseIntermediateFile 301 // Unable to load license intermediate file
#define GrdSKE_SerialNumberIsNotSpecified       302 // Serial number is not specefied
#define GrdSKE_ErrorIniCurlLibrary              303 // Error initalizing curl library
#define GrdSKE_ErrorFileOperation               304 // Error during an operation with a text file 
#define GrdSKE_LicenseFileNotFound              305 // License file not found
#define GrdSKE_ExtractLicenseInfo               306 // Unable to extract license info from license file
#define GrdSKE_InvalidLicenseFile               307 // License file is invalid or corrupted
#define GrdSKE_InvalidLicenseDataFromResponse   308 // Unable to extract license data from response
#define GrdSKE_SPDongleDoesNotExist                             309 // Guardant SP Dongle with specified ID does not exist


/////////////////////////////////////////////////////////////////////
// Online mode

// Make whole activation process
//  szGrdSPLicenseFileName  - License file name
//  szURL                       - Server URL (optional, may be NULL). Address by default is https://activation.guardant.ru/activationservice.svc
//  szSerialNumber  - License serial number
//  szProxyName     - Proxy name (optional, may be NULL)
//  szProxyPassword - Proxy password (optional, may be NULL)
__declspec(dllexport)
int __stdcall ActivateGuardantSPDongle(const char* szGrdSPLicenseFileName,
		const char* szURL,
		const char* szSerialNumber,
		const char* szProxyName,
		const char* szProxyPassword);

/////////////////////////////////////////////////////////////////////
// Offline mode

// Step 1: Prepare license file on target workstation to send it to a server
//  szGrdSPLicenseFileName  - License file name
//  szSerialNumber                      - License serial number
//  szOutFileName                       - Name of the license file to be sent to a server (Optional, may be NULL). By default szGrdSPLicenseFileName + ".toserver"
__declspec(dllexport)
int __stdcall PrepareSPLicenseToSend(const char* szGrdSPLicenseFileName,
		const char* szSerialNumber,
		const char* szOutFileName);

// Step 2: Send license file to a server on workstation with WAN
//  szGrdSPLicenseFileNameToServer  - File name of license prepared to be sent to a server (with an additional '.toserver' extension)
//  szURL                 - Server URL (Optional, may be NULL). Address by default is https://activation.guardant.ru/activationservice.svc
//  szProxyName           - Proxy name (Optional, may be NULL)
//  szProxyPassword       - Proxy password (Optional, may be NULL)
//  szOutFileName         - Name of the license file to activate (Optional, may be NULL). By default szGrdSPLicenseFileName + ".fromserver"
__declspec(dllexport)
int __stdcall SendSPLicenseFileToServer(const char* szGrdSPLicenseFileNameToServer,
		const char* szURL,
		const char* szProxyName,
		const char* szProxyPassword,
		const char* szOutFileName);

// Step 3: Activate license file received from a server on target workstation
//  szGrdSPLicenseFileNameFromServer  - File name of license received from a server
__declspec(dllexport)
int __stdcall ActivateSPLicenseFileFromServer(const char* szGrdSPLicenseFileNameFromServer);

// Get dongle ID from serial number string
// szSerialNumber               - [in] serial number string
// dongleID                             - [out] dongle id
__declspec(dllexport)
int __stdcall GetSPDongleIdFromSerialNumber(const char* szSerialNumber, uint32_t* dongleID);

// Delete dongle by serial number or string with hex ID
// szSerialNumber               - [in] serial number string
__declspec(dllexport)
int __stdcall DeleteSPDongle(const char* szSerialNumber);

// Delete dongle by serial number or string with hex ID
// dongleID                             - [in] dongle id
__declspec(dllexport)
int __stdcall DeleteSPDongleByID(uint32_t dongleID);

// Backup working dongle to a file
// dongleID                             - [in] dongle id
// szBackupFileName             - [in] output file name of stored dongle
__declspec(dllexport)
int __stdcall BackupSPDongle(uint32_t dongleID, const char* szBackupFileName);

// Restore dongle from a file
// szBackupFileName             - [in] file name of stored dongle
__declspec(dllexport)
int __stdcall RestoreSPDongle(const char* szBackupFileName);

// Get the time of dongle's first actviation
// dongleID                             - [in] dongle id
__declspec(dllexport)
int __stdcall GetSPDongleFirstActivationDate(uint32_t dongleID, uint64_t* fisrtActivationTime);

// Check serial number for validness
//  szSerialNumber      - License serial number
// if returns 0 serial number is valid
__declspec(dllexport)
int __stdcall CheckSPDongleSerialNumber(const char* szSerialNumber);


#endif//__GRDSKACTIVATE_H__