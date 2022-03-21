
#include <cstddef>
#include <cstring>
#include "grddongle.h"

// Constructor
CGrdDongle::CGrdDongle(
		// parameters for call GrdStartup() & GrdSetFindMode
		DWORD   dwRemoteMode,
		// parameters for call SetFindMode()
		DWORD   dwFlags,                            // Combination of GrdFM_XXXXX flags
		DWORD   dwProg,
		DWORD   dwID,
		DWORD   dwSN,
		DWORD   dwVer,
		DWORD   dwMask,
		DWORD   dwType,
		DWORD   dwModels,                           // Possible dongle model bits. See GrdFMM_GSX definition
		DWORD   dwInterfaces)                       // Possible dongle interface bits. See GrdFMI_XXX definition
{
	m_hGrd = NULL;
	// parameters for call GrdStartup() & GrdSetFindMode
	m_dwRemoteMode      = dwRemoteMode;
	// parameters for call SetFindMode()
	m_dwFlags           = dwFlags;              // Combination of GrdFM_XXXXX flags
	m_dwProg            = dwProg;
	m_dwVer             = dwVer;
	m_dwMask            = dwMask;
	m_dwType            = dwType;
	m_dwModels          = dwModels;             // Possible dongle model bits. See GrdFMM_GSX definition
	m_dwInterfaces      = dwInterfaces;         // Possible dongle interface bits. See GrdFMI_XXX definition
}

// Create
int CGrdDongle::Create(
		// parameters for call SetAccessCodes()
		DWORD   dwPublic,
		DWORD   dwPrivateRD,
		DWORD   dwPrivateWR,
		DWORD   dwPrivateMST,
		// parameters for call SetWorkMode()
		DWORD   dwFlagsWork,                    // combination of GrdWM_XXX flags
		DWORD   dwFlagsMode)                    // combination of GrdWMFM_XXX flags
{
	// Initialize this copy of GrdAPI. GrdStartup() must be called once before first GrdAPI call at application startup
	static InitializeGrdDongle InitGrdDongle(m_dwRemoteMode);

	int nRet;

	if (m_hGrd == NULL)
	{
		// Create Grd protected object
		m_hGrd = GrdCreateHandle(m_abyGrdContainer, GrdCHM_MultiThread, NULL);
		if (m_hGrd == NULL)
			return GrdE_Internal;
	}

	// Store dongle codes in Guardant protected container
	nRet = GrdSetAccessCodes(m_hGrd, dwPublic, dwPrivateRD, dwPrivateWR, dwPrivateMST);
	if (nRet != GrdE_OK)
		return  nRet;
	// Set System Address Mode (SAM) as default mode
	nRet = GrdSetWorkMode(m_hGrd, dwFlagsWork, dwFlagsMode);
	if (nRet != GrdE_OK)
		return  nRet;
	// Set dongle search parameters
	return  GrdSetFindMode(m_hGrd, m_dwRemoteMode, m_dwFlags, m_dwProg, 0, 0, m_dwVer, m_dwMask, m_dwType, m_dwModels, m_dwInterfaces);
}

// Destructor
CGrdDongle::~CGrdDongle()
{
	int nRet;

	// Close hGrd handle. If need log out from dongle/server & free allocated memory
	if (m_hGrd)
	{
		nRet = GrdCloseHandle(m_hGrd);
		if (nRet == GrdE_OK)
			m_hGrd = NULL;
	}
}

