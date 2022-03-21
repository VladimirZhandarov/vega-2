#include "core.h"

#include <fstream>
#include <string>
#include <iostream>
#include <cstdio>
#include <memory>

#include <unistd.h>

#include <iANPRinterface.h>

#include "GPIOClass.h"

#ifdef USE_PIFACE_DIGITAL
#include <pifacedigital.h>
#endif

#ifndef NO_AES
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#endif

#ifndef NO_GUARDANT
#include <boost/crc.hpp>
#define PUB_KEY_ECC_SIZE 40
#define VERIFY_START 0
#define VERIFY_CHECK 1
#define VERIFY_MAX_COUNT 1000
#define VERIFY_SEC 30
#endif

#include <boost/lexical_cast.hpp>

#define BOOST_NO_SCOPED_ENUMS
#include <boost/filesystem.hpp>

#include "platereadyvisitor.h"
#include "tcpcommandserver.h"
#include "settings.h"
#include "vegacommandfactory.h"
#include "vegatagfactory.h"

#include "vegacommand25051_startdigitaloutput.h"
#include "vegacommand25052_stopdigitaloutput.h"
#include "vegacommand25134_readdigitalinputoutput.h"
#include "vegacommand25166_getsingleimage.h"
#include "vegacommand26000_resetdevice.h"
#include "vegacommand26001_readfile.h"
#include "vegacommand26002_writefile.h"
#include "vegacommand26003_deletefile.h"
#include "vegacommand35000_reloadlista.h"
#include "vegacommand35001_reloadlistb.h"
#include "vegapacket.h"
#include "platetimedata.h"

#include "vegatag.h"
#include "vegadatatag.h"
#include "vegapointtag.h"
#include "vegadatetimetag.h"
#include "vegabancotag.h"
#include "dbinterface.h"

#define IANPR_PLATE_RESULT_SIZE 100

#define VEGA2APP_HOME "/opt/cerberus"

std::string Core::listAFile = std::string(VEGA2APP_HOME) + "/List_A.txt";
std::string Core::listBFile = std::string(VEGA2APP_HOME) + "/List_B.txt";
std::string Core::listImeiFile = std::string(VEGA2APP_HOME) + "/List_IMEI.txt";
std::string Core::iniFile = std::string(VEGA2APP_HOME) + std::string("/settings.ini");
std::string Core::bakIniFile = std::string(VEGA2APP_HOME) + std::string("/settings.bak");
std::string Core::logFile = std::string(VEGA2APP_HOME) + std::string("/log.txt");
std::string Core::oldLogFile = std::string(VEGA2APP_HOME) + std::string("/log_old.txt");

std::string Core::base64Chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string Core::portAString = "port A";
std::string Core::portNAString = "port NA";
std::string Core::portBString = "port B";
std::string Core::portNBString = "port NB";
char* Core::ver17 = "1.7";

void videoCaptureWiringPi()
{
	Core::getInstance()->setWiringPiNeedVideoCapture();
}

Core* Core::m_instance = 0;

Core::Core() : VegaCommandExecutor()
{
	struct stat st = {0};
	if(stat(VEGA2APP_HOME, &st) == -1)
	{
		mkdir(VEGA2APP_HOME, 0777);
	}

	m_srcImageReadyForPlateRecognize = false;

	m_plateReadyVisitor = 0;
	m_settings = new Settings(iniFile);

	m_logLevel = m_settings->getLogLevel();

	m_minPlateSequence = m_settings->getMinPlateSequence();
	m_plateCount = 0;

	m_fps = m_settings->getFps();
	if(m_fps <= 0)
	{
		m_fps = 30;
	}
	m_logMaxRowNumber = m_settings->getLogMaxRowNumber();
	m_logCurrentRowNumber = 0;
	m_minPlateCountInMap = m_settings->getMinPlateCountInMap();
	m_maxPlateCountTime = m_settings->getMaxPlateCountTime();

	if(m_minPlateSequence < 0 && m_minPlateCountInMap < 0)
	{
		m_minPlateSequence = 1;
	}

	m_anprOption.Detect_Mode = m_settings->getAnprDetectMode();
	m_anprOption.min_plate_size = m_settings->getAnprMinPlateSize();
	m_anprOption.max_plate_size = m_settings->getAnprMaxPlateSize();
	m_anprOption.max_text_size = m_settings->getAnprMaxTextSize();
	m_anprOption.type_number = 7;	//< russian
	m_anprOption.flags = 0;
	m_anprOption.vers = ver17;

	m_settingsRect = m_settings->getRect();

	m_messageTimeLiveSec = m_settings->getMessageTimeToLiveSec();
	m_messageSendTimeoutSec = m_settings->getMessageSendTimeoutSec();
	m_storeNoSendedPlate = m_settings->getStoreNoSendedPlate();

	m_workRelayWaitTimeMilliSec = m_settings->getWorkRelayWaitTimeMilliSec();
	m_workRelayNumber = m_settings->getWorkRelayNumber();
	m_portAAutoOpenRelayNumber = m_settings->getPortAAutoOpenRelayNumber();
	m_portBAutoOpenRelayNumber = m_settings->getPortBAutoOpenRelayNumber();
	m_openRelayTimeoutSec = m_settings->getOpenRelayTimeoutSec();
	m_openRelay0Time = 0;
	m_openRelay1Time = 0;

	m_sendPlateRect = m_settings->getSendPlateRect();
	m_sendPlateImage = m_settings->getSendPlateImage();

	m_checkDirFlag = false;
	m_checkDirUp = m_settings->getCheckDirUp();
	m_checkDirDown = m_settings->getCheckDirDown();
	m_checkDirLeft = m_settings->getCheckDirLeft();
	m_checkDirRight = m_settings->getCheckDirRight();
	m_checkDirRectSizeGoToLarge = m_settings->getCheckDirRectSizeGoToLarge();
	m_checkDirRectSizeGoToSmall = m_settings->getCheckDirRectSizeGoToSmall();
	m_checkDirBound = m_settings->getCheckDirBound();
	m_checkDirDataLiveTimeout = m_settings->getCheckDirDataLiveTimeout();
	if(m_checkDirDataLiveTimeout < 0)
	{
		m_checkDirDataLiveTimeout = 0;
	}
	if(m_checkDirBound > 0 && (m_checkDirUp > 0 || m_checkDirDown > 0 || m_checkDirLeft > 0 || m_checkDirRight > 0 || m_checkDirRectSizeGoToLarge > 0 ||
		m_checkDirRectSizeGoToSmall > 0))
	{
		m_checkDirFlag = true;
	}

	m_wiringPiPinWorkRelay0 = m_settings->getWiringPiPinWorkRelay0();
	m_wiringPiPinCheckRelay0 = m_settings->getWiringPiPinCheckRelay0();
	m_wiringPiPinWorkRelay1 = m_settings->getWiringPiPinWorkRelay1();
	m_wiringPiPinCheckRelay1 = m_settings->getWiringPiPinCheckRelay1();

	m_wiringPiPinNumberVideoCaptureFromLowToHigh = m_settings->getWiringPiPinNumberVideoCaptureFromLowToHigh();
	m_wiringPiPinNumberVideoCaptureFromHighToLow = m_settings->getWiringPiPinNumberVideoCaptureFromHighToLow();
	m_wiringPiEmptyPlateVideoCapture = m_settings->getWiringPiEmptyPlateVideoCapture();
	m_wiringPiVideoCaptureDelaySec = m_settings->getWiringPiVideoCaptureDelaySec();
	m_wiringPiNeedVideoCapture = false;

	std::ifstream countLogFile(logFile);
	m_logCurrentRowNumber = std::count(std::istreambuf_iterator<char>(countLogFile), std::istreambuf_iterator<char>(), '\n');

	m_logFile.open(logFile, std::ofstream::out | std::ofstream::app);

	addLogMessage("start work");

	m_iANPRKey = 0;

#ifndef NO_AES
	std::string licenceInfo = getLicenceInfo();
	if(licenceInfo.size() > 0)
	{
		addLogMessage(licenceInfo, false);
	}
	else
	{
		addLogMessage("error licence", false);
		exit(-1);
	}
#endif

	m_testPlateString = "";

#ifndef NO_GUARDANT
	m_grdDongle = 0;
	m_pubEccKeyByteArray = 0;
	m_pubEccKeyByteArrayLen = 0;
	std::string pubEccKeyString = std::string("579BCBFC51C14A1D9AF483C59EE496D24BCB3439D0B9EA1962CB9B1C4C4FD6BD77CCDEFA4FD40C6B");
	unsigned char* pubEccKeyCharArray = new unsigned char[pubEccKeyString.length() + 1];
	memcpy(pubEccKeyCharArray, pubEccKeyString.c_str(), pubEccKeyString.length());
	m_pubEccKeyByteArray = new unsigned char[GrdECC160_DIGEST_SIZE + 1];
	pubEccKeyCharArray[pubEccKeyString.length()] = 0;
	m_pubEccKeyByteArrayLen = hex2byte(pubEccKeyCharArray, m_pubEccKeyByteArray);
	delete [] pubEccKeyCharArray;

	if(m_pubEccKeyByteArrayLen < GrdECC160_DIGEST_SIZE)
	{
		addLogMessage("error: wrong guardant pub key len", false);
		exit(-1);
	}
	std::string guardantLicenceInfo = getGuardantLicenceInfo();
	if(guardantLicenceInfo.size() > 0)
	{
		addLogMessage(guardantLicenceInfo, false);
	}
	else
	{
		addLogMessage("error guardant licence", false);
		exit(-1);
	}
	m_verifyState = VERIFY_START;
	m_dongleVerifyCount = VERIFY_MAX_COUNT;
	m_alg0Activated = false;
#endif

	if(m_iANPRKey == 0)
	{
		FILE* f = fopen("/opt/cerberus/ooosmit.key", "rb");
		if(f != NULL)
		{
			fseek(f, 0L, SEEK_END);
			long fileSize  = ftell(f);
			fseek(f, 0L, SEEK_SET);
			m_iANPRKey = new char[fileSize + 1];
			fread((void*)m_iANPRKey, fileSize, 1, f);
			m_iANPRKey[fileSize] = '\0';
			fclose(f);
		}
	}
	if(m_iANPRKey)
	{
		LicenseValue(m_iANPRKey);
		delete [] m_iANPRKey;
	}
	else
	{
		addLogMessage("No recognize license", false);
	}

	m_wiringPiWorkRelay0 = 0;
	m_wiringPiCheckRelay0 = 0;
	m_wiringPiWorkRelay1 = 0;
	m_wiringPiCheckRelay1 = 0;


#ifdef USE_GPIOD
	addLogMessage("GPIOD started", false);
	if(m_wiringPiPinWorkRelay0 >= 0)
	{
		if(m_wiringPiPinWorkRelay0 == m_wiringPiPinCheckRelay0 ||
			m_wiringPiPinWorkRelay0 == m_wiringPiPinCheckRelay1)
		{
			addLogMessage(std::string("Unable to setup OUTPUT and INPUT Mode for WIRING PI WORK RELAY 0"), false);
			exit(-1);
		}
		char numBuffer[128];
		sprintf(numBuffer, "%d", m_wiringPiPinWorkRelay0);
		m_wiringPiWorkRelay0 = new GPIOClass(std::string(numBuffer));
		m_wiringPiWorkRelay0->setdir_gpio("out");
	}
	if(m_wiringPiPinCheckRelay0 >= 0)
	{
		char numBuffer[128];
		sprintf(numBuffer, "%d", m_wiringPiPinCheckRelay0);
		m_wiringPiCheckRelay0 = new GPIOClass(std::string(numBuffer));
		m_wiringPiCheckRelay0->setdir_gpio("in");
	}
	if(m_wiringPiPinWorkRelay1 >= 0)
	{
		if(m_wiringPiPinWorkRelay1 == m_wiringPiPinCheckRelay0 ||
			m_wiringPiPinWorkRelay1 == m_wiringPiPinCheckRelay1)
		{
			addLogMessage(std::string("Unable to setup OUTPUT and INPUT Mode for WIRING PI WORK RELAY 1"), false);
			exit(-1);
		}
		char numBuffer[128];
		sprintf(numBuffer, "%d", m_wiringPiPinWorkRelay1);
		m_wiringPiWorkRelay1 = new GPIOClass(std::string(numBuffer));
		m_wiringPiWorkRelay1->setdir_gpio("out");
	}
	if(m_wiringPiPinCheckRelay1 >= 0)
	{
		char numBuffer[128];
		sprintf(numBuffer, "%d", m_wiringPiPinCheckRelay1);
		m_wiringPiCheckRelay1 = new GPIOClass(std::string(numBuffer));
		m_wiringPiCheckRelay1->setdir_gpio("in");
	}
	addLogMessage("GPIOD inited", false);
#endif

#ifdef USE_PIFACE_DIGITAL
	m_piFaceHwAddr = 0;
	if(pifacedigital_open(m_piFaceHwAddr) == -1)
	{
		addLogMessage(std::string("ERROR OPEN PIFACE DIGITAL - HARDWARE NOT INSTALLED"), false);
	}

	pifacedigital_write_bit(0, 0, OUTPUT, m_piFaceHwAddr);
	pifacedigital_write_bit(0, 1, OUTPUT, m_piFaceHwAddr);
	pifacedigital_write_bit(1, 2, OUTPUT, m_piFaceHwAddr);
	pifacedigital_write_bit(1, 3, OUTPUT, m_piFaceHwAddr);
#endif

#ifdef USE_MONGODB
	m_dbInterface = 0;
	m_dbInited = false;
	if(m_storeNoSendedPlate > 0)
	{
		std::vector<std::string> portNameVector;
		portNameVector.push_back(portAString);
		portNameVector.push_back(portNAString);
		portNameVector.push_back(portBString);
		portNameVector.push_back(portNBString);
		m_dbInterface = new DbInterface(this, m_storeNoSendedPlate, m_logLevel, "mongodb://localhost:27017", "vega2Db", portNameVector);
		if(!m_dbInterface->ping())
		{
			addLogMessage("MongoDB connection ERROR. Please start mongod server.", false);
		}
		else
		{
			addLogMessage("MongoDB connection OK", false);
			initDb();
		}
	}
#endif

	if(m_settings->getPortControl() != 0)
	{
		addLogMessage("TcpCommandServer started", false);
		m_tcpCommandServer = new TcpCommandServer(1, this, m_settings->getPortControl(), m_settings->getLogLevel());
		m_tcpCommandServer->start();
	}
	else
	{
		m_tcpCommandServer = 0;
	}
	if(m_settings->getPortApkControl() != 0)
	{
		addLogMessage("TcpAPkCommandServer started", false);
		m_tcpApkCommandServer = new TcpCommandServer(2, this, m_settings->getPortApkControl(), m_settings->getLogLevel());
		m_tcpApkCommandServer->start();
	}
	else
	{
		m_tcpApkCommandServer = 0;
	}
	if(m_tcpCommandServer == 0 && m_tcpApkCommandServer == 0)
	{
		addLogMessage("NO CONTROL PORTS", false);
	}

	m_aListReinited = false;
	m_bListReinited = false;
	reloadAList();
	reloadBList();

	m_imeiList = reloadList(listImeiFile);
	char imeiListMsgBuffer[1000];
	sprintf(imeiListMsgBuffer, "imeiList size = %d", m_imeiList.size());
	addLogMessage(std::string(imeiListMsgBuffer));

	VegaCommandFactory& vegaCommandFactory = VegaCommandFactory::getInstance();
	vegaCommandFactory.add<VegaCommand25051_StartDigitalOutput>();
	vegaCommandFactory.add<VegaCommand25052_StopDigitalOutput>();
	vegaCommandFactory.add<VegaCommand25134_ReadDigitalInputOutput>();
	vegaCommandFactory.add<VegaCommand25166_GetSingleImage>();
	vegaCommandFactory.add<VegaCommand26000_ResetDevice>();
	vegaCommandFactory.add<VegaCommand26001_ReadFile>();
	vegaCommandFactory.add<VegaCommand26002_WriteFile>();
	vegaCommandFactory.add<VegaCommand26003_DeleteFile>();
	vegaCommandFactory.add<VegaCommand35000_ReloadListA>();
	vegaCommandFactory.add<VegaCommand35001_ReloadListB>();

	VegaTagFactory& vegaTagFactory = VegaTagFactory::getInstance();
	vegaTagFactory.add<VegaTag>(VegaTag::FILE_NAME_TAG);
	vegaTagFactory.add<VegaTag>(VegaTag::FILE_DATA_TAG);

	vegaTagFactory.add<VegaTag>(VegaTag::DATE_TAG);
	vegaTagFactory.add<VegaTag>(VegaTag::TIME_TAG);
	vegaTagFactory.add<VegaTag>(VegaTag::PLATE_STRING_TAG);
	vegaTagFactory.add<VegaTag>(VegaTag::PLATE_COUNTRY_TAG);
	vegaTagFactory.add<VegaTag>(VegaTag::IMAGE_BW_TAG);
	vegaTagFactory.add<VegaTag>(VegaTag::IMEI_TAG);

	vegaTagFactory.add<VegaDataTag>(VegaDataTag::DIGITAL_INPUT_NUMBER);
	vegaTagFactory.add<VegaDataTag>(VegaDataTag::DIGITAL_OUTPUT_NUMBER);
	vegaTagFactory.add<VegaDataTag>(VegaDataTag::DIGITAL_INPUT_STATUS);
	vegaTagFactory.add<VegaDataTag>(VegaDataTag::DIGITAL_OUTPUT_STATUS);
	vegaTagFactory.add<VegaDataTag>(VegaDataTag::DIGITAL_OUTPUT_SEL_NUMBER);
	vegaTagFactory.add<VegaDataTag>(VegaDataTag::JPEG_QUALITY);

	vegaTagFactory.add<VegaPointTag>(VegaPointTag::POINT);
	vegaTagFactory.add<VegaDateTimeTag>(VegaDateTimeTag::IMAGE_TIME);
	vegaTagFactory.add<VegaBancoTag>(VegaBancoTag::IMAGE);
}

Core* Core::getInstance()
{
	if(!m_instance)
	{
		m_instance = new Core();
	}
	return m_instance;
}

void Core::releaseInstance()
{
	delete m_instance;
	m_instance = 0;
}

Core::~Core()
{
	addLogMessage("stop work");

#ifdef USE_PIFACE_DIGITAL
	pifacedigital_close(m_piFaceHwAddr);
#endif

	addLogMessage("delete TcpCommandServer");
	delete m_tcpCommandServer;
	addLogMessage("delete TcpApkCommandServer");
	delete m_tcpApkCommandServer;

	m_finishAllThreads = true;

	m_videoCaptureThread.join();
	if(m_videoCapture.isOpened())
	{
		m_videoCapture.release();
	}
	for(int i = 0; i < m_plateRecognizeThreadVector.size(); i++)
	{
		m_plateRecognizeThreadVector[i].join();
	}
	for(int i = 0; i < m_sendVegaPacketThreadVector.size(); i++)
	{
		m_sendVegaPacketThreadVector[i].join();
	}
	for(int i = 0; i < m_messageQueueVector.size() && i < m_messageQueueMutexVector.size(); i++)
	{
		m_messageQueueMutexVector[i]->lock();
		bool emptyMessageQueue = m_messageQueueVector[i].empty();
		m_messageQueueMutexVector[i]->unlock();
		while(!emptyMessageQueue)
		{
			m_messageQueueMutexVector[i]->lock();
			VegaPacket* packet = m_messageQueueVector[i].front();
			m_messageQueueVector[i].pop();
			m_messageQueueMutexVector[i]->unlock();
			delete packet;
			m_messageQueueMutexVector[i]->lock();
			emptyMessageQueue = m_messageQueueVector[i].empty();
			m_messageQueueMutexVector[i]->unlock();
		}
	}
	for(int i = 0; i < m_messageQueueMutexVector.size(); i++)
	{
		delete m_messageQueueMutexVector[i];
		m_messageQueueMutexVector[i] = 0;
	}
	delete m_settings;

#ifdef USE_MONGODB
	delete m_dbInterface;
	m_dbInterface = 0;
	m_dbInited = false;
#endif

#ifndef NO_GUARDANT
	delete m_grdDongle;
	delete [] m_pubEccKeyByteArray;
#endif

	delete m_wiringPiWorkRelay0;
	delete m_wiringPiCheckRelay0;
	delete m_wiringPiWorkRelay1;
	delete m_wiringPiCheckRelay1;

	m_logFile.close();
}

#ifndef NO_GUARDANT

int Core::guardantErrorHandling(CGrdDongle* grdDongle, int errorCode)
{
	char errorMsg[0x100];
	int ret = grdDongle->FormatMessage(errorCode, GrdLng_ENG, errorMsg, sizeof(errorMsg));
	if(errorCode != GrdE_OK)
	{
		addLogMessage(std::string("dongle: error ") + std::string(errorMsg));
		exit(-1);
	}
	return ret;
}

bool Core::guardantDemix(unsigned char* inArray, size_t inArrayLen,
	unsigned char* outArray1, size_t outArray1Len,
	unsigned char* outArray2, size_t outArray2Len)
{
	if(inArray == 0 || outArray1 == 0 || outArray2 == 0 ||
		inArrayLen != outArray1Len + outArray2Len)
	{
		return false;
	}
	for(int i = 0, j = 0; i < inArrayLen; j++)
	{
		if(j < outArray1Len)
		{
			outArray1[j] = inArray[i++];
		}
		if(j < outArray2Len)
		{
			outArray2[j] = inArray[i++];
		}
	}
	return true;
}

void Core::guardantRotateCharArray(unsigned char* array, size_t arrayLen)
{
	for(size_t i = 0; i < (arrayLen / 2); i++)
	{
		unsigned char tmpCh = array[i];
		array[i] = array[arrayLen - 1 - i];
		array[arrayLen - 1 - i] = tmpCh;
	}
}

#endif

std::string Core::getGuardantLicenceInfo()
{
	std::string result = "";

#ifndef NO_GUARDANT

	m_dongleId = 0;
	FILE* f = fopen("/opt/cerberus/licence.lic", "rb");
	unsigned char* hexCh = 0;
	unsigned char* byteCh = 0;
	size_t byteChLen = 0;
	if(f != NULL)
	{
		fseek(f, 0L, SEEK_END);
		long fileSize  = ftell(f);
		fseek(f, 0L, SEEK_SET);
		hexCh = new unsigned char[fileSize + 1];
		byteChLen = fileSize / 2;
		byteCh = new unsigned char[byteChLen];
		fread((void*)hexCh, fileSize, 1, f);
		hexCh[fileSize] = '\0';
		fclose(f);
	}
	else
	{
		addLogMessage("error: can not read /opt/cerberus/licence.lic", false);
		return "";
	}
	byteChLen = hex2byte(hexCh, byteCh);
	if(byteChLen < PUB_KEY_ECC_SIZE + 8 + 4 + 8 + 1)
	{
		addLogMessage("error: wrong guardant lic len", false);
		return "";
	}
	unsigned char* arrayCh1 = new unsigned char[byteChLen - 4];
	unsigned char* arrayCh2 = new unsigned char[4];
	if(!guardantDemix(byteCh, byteChLen, arrayCh1, byteChLen - 4, arrayCh2, 4))
	{
		delete [] arrayCh1;
		delete [] arrayCh2;
		return "";
	}
	m_grdRdKey = 0;
	guardantRotateCharArray(arrayCh2, 4);
	memcpy(&m_grdRdKey, arrayCh2, 4);

	m_grdDongle = new CGrdDongle(GrdFMR_Local);
	guardantErrorHandling(m_grdDongle, GrdE_OK);
	int ret = m_grdDongle->Create(CryptPU + C2, CryptRD + C2, 0, 0);
	guardantErrorHandling(m_grdDongle, ret);
	ret = m_grdDongle->SetFindMode(GrdFMR_Local, GrdFM_NProg, 1, 0, 0, 0, 0, GrdDT_GSII64, GrdFMM_GS3S | GrdFMM_GS3);
	guardantErrorHandling(m_grdDongle, m_grdDongle->GetLastError());
	DWORD id;
	TGrdFindInfo grdFindInfo;
	ret = m_grdDongle->Find(GrdF_First, &id, &grdFindInfo);
	guardantErrorHandling(m_grdDongle, ret);
	size_t workArrayLen = byteChLen - 4;
	bool resultExist = false;
	bool dongleExist = false;
	int index = 1;
	while(ret == GrdE_OK)
	{
		dongleExist = true;
		DWORD lms = -1;
		ret = m_grdDongle->Login(lms);
		guardantErrorHandling(m_grdDongle, ret);
		ret = m_grdDongle->PI_Activate(0x2, m_grdRdKey);
		guardantErrorHandling(m_grdDongle, ret);
		char iv[16];
		ret = m_grdDongle->PI_Read(0x3, 0, 16, iv, m_grdRdKey);
		guardantErrorHandling(m_grdDongle, ret);
		ret = m_grdDongle->CryptEx(0x2, workArrayLen, arrayCh1, GrdAM_Decode + GrdAM_OFB, 16, iv);
		guardantErrorHandling(m_grdDongle, ret);

		unsigned char* arrayCh3 = new unsigned char[workArrayLen - 8];
		unsigned char* arrayCh4 = new unsigned char[8];
		if(guardantDemix(arrayCh1, workArrayLen, arrayCh3, workArrayLen - 8, arrayCh4, 8))
		{
			boost::crc_32_type crc32;
			crc32.process_bytes(arrayCh3, workArrayLen - 8);
			long nCrc = 0;
			guardantRotateCharArray(arrayCh4, 8);
			memcpy(&nCrc, arrayCh4, 8);
			if(crc32.checksum() == nCrc)
			{
				workArrayLen = workArrayLen - 8;
				unsigned char* arrayCh5 = new unsigned char[workArrayLen - PUB_KEY_ECC_SIZE];
				unsigned char* arrayCh6 = new unsigned char[PUB_KEY_ECC_SIZE];
				if(guardantDemix(arrayCh3, workArrayLen, arrayCh5, workArrayLen - PUB_KEY_ECC_SIZE, arrayCh6, PUB_KEY_ECC_SIZE))
				{
					result = parseLicenceString(arrayCh5, workArrayLen - PUB_KEY_ECC_SIZE);
					resultExist = true;
					addLogMessage(std::string("use token with serial number ") + std::to_string(grdFindInfo.wSN), false);
					m_dongleId = id;
				}
				delete [] arrayCh5;
				delete [] arrayCh6;
			}
			else
			{
				addLogMessage(std::string("error: wrong guardant crc number ") + std::to_string(index), false);
			}
		}
		delete [] arrayCh3;
		delete [] arrayCh4;

		if(resultExist)
		{
			break;
		}
		ret = m_grdDongle->Logout();
		guardantErrorHandling(m_grdDongle, ret);
		ret = m_grdDongle->Find(GrdF_Next, &id, &grdFindInfo);
		guardantErrorHandling(m_grdDongle, ret);
		index++;
	}
	if(!dongleExist)
	{
		addLogMessage("error: no right dongle connected", false);
	}
	delete [] arrayCh1;
	delete [] arrayCh2;

#endif

	return result;
}

std::string Core::getLicenceInfo()
{

#ifndef NO_AES
	std::string cmd = "MACHINE_TYPE=`uname -m`\n";
	cmd += "if [ ${MACHINE_TYPE} = 'x86_64' ] ; then\n";
	cmd += "dmidecode\n";
	cmd += "elif [ ${MACHINE_TYPE} = 'i686' ] ; then\n";
	cmd += "dmidecode\n";
	cmd += "else\n";
	cmd += "lsusb -v | grep \"iSerial                 3\"\n";
	cmd += "fi\n";

	std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
	if(!pipe)
	{
		return "";
	}
	char buffer[128];
	std::string systemInfo = "";
	while(!feof(pipe.get()))
	{
		if(fgets(buffer, 128, pipe.get()) != NULL)
		{
			systemInfo += buffer;
		}
	}
	char passwordChar[65];
	char* systemInfoCh = new char[systemInfo.size() + 1];
	std::copy(systemInfo.begin(), systemInfo.end(), systemInfoCh);
	systemInfoCh[systemInfo.size()] = '\0';
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, systemInfoCh, strlen(systemInfoCh));
	SHA256_Final(hash, &sha256);
	delete [] systemInfoCh;
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		sprintf(passwordChar + (i * 2), "%02X", hash[i]);
	}
	passwordChar[64] = 0;

	FILE* f = fopen("/opt/cerberus/licence.lic", "rb");
	char* base64Ch = 0;
	if(f != NULL)
	{
		fseek(f, 0L, SEEK_END);
		long fileSize  = ftell(f);
		fseek(f, 0L, SEEK_SET);
		base64Ch = new char[fileSize + 1];
		fread((void*)base64Ch, fileSize, 1, f);
		base64Ch[fileSize] = '\0';
		fclose(f);
	}
	else
	{
		addLogMessage("error: can not read /opt/cerberus/licence.lic", false);
		return "";
	}
	std::string base64(base64Ch);
	std::vector<char> binVector = base64Decode(base64);
	delete [] base64Ch;

	int binDataSize = binVector.size();
	if(binDataSize > 0)
	{
		unsigned char* binArray = new unsigned char[binDataSize];
		memcpy(binArray, binVector.data(), binDataSize * sizeof(char));
		int aesKeyLength = binArray[0] * 8;
		int SALT_LENGTH = 16; // in bytes
		int AUTH_KEY_LENGTH = 8; // in bytes
		int IV_LENGTH = 16; // in bytes
		int keyLength = aesKeyLength + AUTH_KEY_LENGTH * 8;
		if(binDataSize > 1 + SALT_LENGTH + AUTH_KEY_LENGTH + IV_LENGTH)
		{
			unsigned char saltChar[SALT_LENGTH];
			memcpy(saltChar, &binArray[1], SALT_LENGTH);
			unsigned char readedAuthKeyChar[AUTH_KEY_LENGTH];
			memcpy(readedAuthKeyChar, &binArray[1 + SALT_LENGTH], AUTH_KEY_LENGTH);
			unsigned char ivChar[IV_LENGTH];
			memcpy(ivChar, &binArray[1 + SALT_LENGTH + AUTH_KEY_LENGTH], IV_LENGTH);
			int encDataIndex = 1 + SALT_LENGTH + AUTH_KEY_LENGTH + IV_LENGTH;
			int encDataSize = binDataSize - encDataIndex;
			int iterations = 32768;
			unsigned char* keyChar = (unsigned char *)malloc(sizeof(unsigned char) * keyLength);
			if(PKCS5_PBKDF2_HMAC_SHA1(passwordChar, strlen(passwordChar), saltChar, sizeof(saltChar), iterations, keyLength, keyChar) == 0)
			{
				return "";
			}
			unsigned char authKeyChar[AUTH_KEY_LENGTH];
			memcpy(authKeyChar, keyChar, AUTH_KEY_LENGTH);
			unsigned char decKeyChar[aesKeyLength / 8];
			memcpy(decKeyChar, &keyChar[AUTH_KEY_LENGTH], aesKeyLength / 8);
			delete [] keyChar;
			if(memcmp(authKeyChar, readedAuthKeyChar, AUTH_KEY_LENGTH) != 0)
			{
				return "";
			}
			AES_KEY decKey;
			AES_set_decrypt_key(decKeyChar, aesKeyLength, &decKey);
			unsigned char* decOut = new unsigned char[encDataSize];
			memset(decOut, 0, encDataSize);
			AES_cbc_encrypt(&binArray[encDataIndex], decOut, encDataSize, &decKey, ivChar, AES_DECRYPT);
			std::string result = parseLicenceString(decOut, encDataSize);
			delete [] decOut;
			return result;
		}
	}
#endif

	return "";
}

std::string Core::parseLicenceString(unsigned char* data, size_t dataLen)
{
	std::vector<std::string> fieldNameVector;
	fieldNameVector.push_back("Vendor:        ");
	fieldNameVector.push_back("Customer:      ");
	fieldNameVector.push_back("CustomerFio:   ");
	fieldNameVector.push_back("CustomerPhone: ");
	fieldNameVector.push_back("CustomerEmail: ");
	fieldNameVector.push_back("Product:       ");
	fieldNameVector.push_back("Version:       ");
	fieldNameVector.push_back("Date:          ");
	fieldNameVector.push_back("Serial:        ");
	std::string result = "";
	int32_t fieldSize = 0;
	char* fieldSizeCh = (char*)&fieldSize;
	int decIndex = 0;
	for(int i = 0; i < fieldNameVector.size(); i++)
	{
		fieldSizeCh[0] = data[decIndex + 3];
		fieldSizeCh[1] = data[decIndex + 2];
		fieldSizeCh[2] = data[decIndex + 1];
		fieldSizeCh[3] = data[decIndex];
		decIndex += sizeof(int32_t);
		if(fieldSize + decIndex <= dataLen)
		{
			char* fieldValueChar = new char[fieldSize + 1];
			memcpy(fieldValueChar, &data[decIndex], fieldSize);
			fieldValueChar[fieldSize] = '\0';
			decIndex += fieldSize;
			result += fieldNameVector[i] + std::string(fieldValueChar) + std::string("\n");
			delete [] fieldValueChar;
		}
	}
	int32_t iANPRKeySize = 0;
	char* iANPRKeySizeCh = (char*)&iANPRKeySize;
	iANPRKeySizeCh[0] = data[decIndex + 3];
	iANPRKeySizeCh[1] = data[decIndex + 2];
	iANPRKeySizeCh[2] = data[decIndex + 1];
	iANPRKeySizeCh[3] = data[decIndex];
	decIndex += sizeof(int32_t);
	if(iANPRKeySize > 0 && iANPRKeySize + decIndex <= dataLen)
	{
		m_iANPRKey = new char[iANPRKeySize + 1];
		memcpy(m_iANPRKey, &data[decIndex], iANPRKeySize);
		m_iANPRKey[iANPRKeySize] = '\0';
	}
	return result;
}

int Core::char2int(unsigned char input) const
{
	if(input >= '0' && input <= '9')
	{
		return input - '0';
	}
 	if(input >= 'A' && input <= 'F')
 	{
		return input - 'A' + 10;
	}
	if(input >= 'a' && input <= 'f')
	{
		return input - 'a' + 10;
	}
	return -1;
}

int Core::hex2byte(const unsigned char* src, unsigned char* dst) const
{
	int result=0;
	while(*src && src[1])
	{
		if(*src == '\r' || *src == '\n')
		{
			src += 1;
			continue;
		}
		int num1 = char2int(*src);
		int num2 = char2int(src[1]);
		if(num1 >= 0 && num2 >= 0)
		{
			*(dst++) = num1 * 16 + num2;
			src += 2;
			result++;
		}
		else
		{
			break;
		}
	}
	return result;
}

bool Core::isBase64(unsigned char c) const
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::vector<char> Core::base64Decode(const std::string& encodedString) const
{
	int inLen = encodedString.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char charArray4[4], charArray3[3];
	std::vector<char> result;

	while(inLen-- && (encodedString[in_] != '=') && isBase64(encodedString[in_]))
	{
		charArray4[i++] = encodedString[in_];
		in_++;
		if(i == 4)
		{
			for(i = 0; i < 4; i++)
			{
				charArray4[i] = base64Chars.find(charArray4[i]);
			}
			charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
			charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
			charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];
			for(i = 0; i < 3; i++)
			{
				result.push_back(charArray3[i]);
			}
			i = 0;
		}
	}
	if(i)
	{
		for(j = i; j < 4; j++)
		{
			charArray4[j] = 0;
		}
		for(j = 0; j < 4; j++)
		{
			charArray4[j] = base64Chars.find(charArray4[j]);
		}
		charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
		charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
		charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

		for(j = 0; j < i - 1; j++)
		{
			result.push_back(charArray3[j]);
		}
	}
	return result;
}

void Core::setPlateReadyVisitor(PlateReadyVisitor* plateReadyVisitor)
{
	m_plateReadyVisitor = plateReadyVisitor;
}

void Core::openFile(const std::string& fileName)
{
	if(fileName.empty())
	{
		return;
	}
	try
	{
 		if(m_videoCapture.isOpened())
		{
			m_videoCapture.release();
		}
		m_videoCaptureThread.join();
		m_srcImageMutex.lock();
		m_srcImage = cv::imread(fileName);
		m_srcImageMutex.unlock();
		if(m_plateReadyVisitor)
		{
			m_plateReadyVisitor->imageReady();
		}
		cv::Mat image = m_srcImage.clone();
		plateRecognize(image);
		image.release();
	}
	catch(const cv::Exception& exc)
	{

	}
	if(!m_srcImage.empty())
	{
		m_srcImage.release();
	}
}

void Core::startCapture()
{
	m_finishAllThreads = false;
	if(m_settings)
	{
		m_videoCaptureThread = boost::thread(&Core::processVideoCapture, this);
		int size = m_settings->getRecognizeThreadNumber();
		for(int i = 0; i < size; i++)
		{
			m_plateRecognizeThreadVector.push_back(boost::thread(&Core::processPlateRecognize, this));
		}
		testForAddSendVegaPacketThread(m_settings->getHostAddressA(), m_settings->getPortA(), 0);
		testForAddSendVegaPacketThread(m_settings->getHostAddressNA(), m_settings->getPortNA(), 1);
		testForAddSendVegaPacketThread(m_settings->getHostAddressB(), m_settings->getPortB(), 2);
		testForAddSendVegaPacketThread(m_settings->getHostAddressNB(), m_settings->getPortNB(), 3);
	}
}

void Core::joinVideoCaptureThread()
{
	m_videoCaptureThread.join();
}

bool Core::startCaptureStream()
{
	bool result = false;
	if(m_settings)
	{
		try
		{
			result = m_videoCapture.open(m_settings->getIpCameraUrl());
		}
		catch(cv::Exception& e)
		{
			addLogMessage(std::string("Exception caught from video capture.open: ") + std::string(e.what()));
		}
	}
	return result;
}

bool Core::startCaptureWebCam()
{
	int number = m_settings->getWebCameraNumber();
	if(number < 0)
	{
		return false;
	}
	return m_videoCapture.open(number);
}

const CvRect& Core::getSettingsRect() const
{
	return m_settingsRect;
}

DigitalInputOutputStatus Core::startDigitalOutput(uint32_t number)
{
	DigitalInputOutputStatus result;
	result.digitalInputNumber = 0;
	result.digitalInputStatus = 0;
	result.digitalOutputNumber = 0;
	result.digitalOutputStatus = 0;

	int workRelayNumber = number + 1;
	if(m_workRelayNumber > 0 && m_workRelayNumber <= 3)
	{
		workRelayNumber = m_workRelayNumber;
	}

	std::time_t currentDateTime;
	time(&currentDateTime);

#ifdef USE_PIFACE_DIGITAL
	result.digitalOutputNumber = 2;
	if(workRelayNumber & 1)
	{
		pifacedigital_write_bit(1, 0, OUTPUT, m_piFaceHwAddr);
		pifacedigital_write_bit(0, 2, OUTPUT, m_piFaceHwAddr);
		addLogMessage("open relay 0");
		m_openRelay0Time = currentDateTime;
	}
	if(workRelayNumber & 2)
	{
		pifacedigital_write_bit(1, 1, OUTPUT, m_piFaceHwAddr);
		pifacedigital_write_bit(0, 3, OUTPUT, m_piFaceHwAddr);
		addLogMessage("open relay 1");
		m_openRelay1Time = currentDateTime;
	}
	if(m_workRelayWaitTimeMilliSec >= 0)
	{
		usleep(m_workRelayWaitTimeMilliSec);
	}
	if(pifacedigital_read_bit(0, INPUT, m_piFaceHwAddr) == 0)
	{
		if(workRelayNumber & 1)
		{
			addLogMessage("error open relay 0");
		}
	}
	else
	{
		result.digitalOutputStatus |= 0x00000001;
	}
	if(pifacedigital_read_bit(1, INPUT, m_piFaceHwAddr) == 0)
	{
		if(workRelayNumber & 2)
		{
			addLogMessage("error open relay 1");
		}
	}
	else
	{
		result.digitalOutputStatus |= 0x00000002;
	}
#endif

#ifdef USE_GPIOD
	result.digitalOutputNumber = 2;

	if((workRelayNumber & 1) && m_wiringPiPinWorkRelay0 >= 0 && m_wiringPiWorkRelay0 != 0)
	{
		m_wiringPiWorkRelay0->setval_gpio("1");
		addLogMessage("open wiring pi relay 0");
		m_openRelay0Time = currentDateTime;
	}
	if((workRelayNumber & 2) && m_wiringPiPinWorkRelay1 >= 0 && m_wiringPiWorkRelay1 != 0)
	{
		m_wiringPiWorkRelay1->setval_gpio("1");
		addLogMessage("open wiring pi relay 1");
		m_openRelay1Time = currentDateTime;
	}

	if(m_workRelayWaitTimeMilliSec >= 0)
	{
		usleep(m_workRelayWaitTimeMilliSec);
	}

	if(m_wiringPiPinCheckRelay0 >= 0 && m_wiringPiCheckRelay0 != 0)
	{
		std::string val = "";
		m_wiringPiCheckRelay0->getval_gpio(val);
		if(val != "1")
		{
			if(workRelayNumber & 1)
			{
				addLogMessage("error open wiring pi relay 0");
			}
		}
		else
		{
			result.digitalOutputStatus |= 0x00000001;
		}
	}
	if(m_wiringPiPinCheckRelay1 >= 0 && m_wiringPiCheckRelay1 != 0)
	{
		std::string val = "";
		m_wiringPiCheckRelay1->getval_gpio(val);
		if(val != "1")
		{
			if(workRelayNumber & 2)
			{
				addLogMessage("error open wiring pi relay 1");
			}
		}
		else
		{
			result.digitalOutputStatus |= 0x00000002;
		}
	}
#endif

	return result;
}

DigitalInputOutputStatus Core::stopDigitalOutput(uint32_t number)
{
	DigitalInputOutputStatus result;
	result.digitalInputNumber = 0;
	result.digitalInputStatus = 0;
	result.digitalOutputNumber = 0;
	result.digitalOutputStatus = 3;

	int workRelayNumber = number + 1;
	if(m_workRelayNumber > 0 && m_workRelayNumber <= 3)
	{
		workRelayNumber = m_workRelayNumber;
	}

#ifdef USE_PIFACE_DIGITAL
	result.digitalOutputNumber = 2;

	if(workRelayNumber & 1)
	{
		pifacedigital_write_bit(0, 0, OUTPUT, m_piFaceHwAddr);
		pifacedigital_write_bit(1, 2, OUTPUT, m_piFaceHwAddr);
		addLogMessage("close relay 0");
	}
	if(workRelayNumber & 2)
	{
		pifacedigital_write_bit(0, 1, OUTPUT, m_piFaceHwAddr);
		pifacedigital_write_bit(1, 3, OUTPUT, m_piFaceHwAddr);
		addLogMessage("close relay 1");
	}
	if(m_workRelayWaitTimeMilliSec >= 0)
	{
		usleep(m_workRelayWaitTimeMilliSec);
	}
	if(pifacedigital_read_bit(0, INPUT, m_piFaceHwAddr) == 1)
	{
		if(workRelayNumber & 1)
		{
			addLogMessage("error close relay 0");
		}
	}
	else
	{
		result.digitalOutputStatus ^= 0x00000001;
	}
	if(pifacedigital_read_bit(1, INPUT, m_piFaceHwAddr) == 1)
	{
		if(workRelayNumber & 2)
		{
			addLogMessage("error close relay 1");
		}
	}
	else
	{
		result.digitalOutputStatus ^= 0x00000002;
	}
#endif

#ifdef USE_GPIOD
	result.digitalOutputNumber = 2;

	if((workRelayNumber & 1) && m_wiringPiPinWorkRelay0 >= 0 && m_wiringPiWorkRelay0 != 0)
	{
		m_wiringPiWorkRelay0->setval_gpio("0");
		addLogMessage("close wiring pi relay 0");
	}
	if((workRelayNumber & 2) && m_wiringPiPinWorkRelay1 >= 0 && m_wiringPiWorkRelay1 != 0)
	{
		m_wiringPiWorkRelay1->setval_gpio("0");
		addLogMessage("close wiring pi relay 1");
	}
	if(m_workRelayWaitTimeMilliSec >= 0)
	{
		usleep(m_workRelayWaitTimeMilliSec);
	}
	if(m_wiringPiPinCheckRelay0 >= 0 && m_wiringPiCheckRelay0 != 0)
	{
		std::string val = "";
		m_wiringPiCheckRelay0->getval_gpio(val);
		if(val != "0")
		{
			if(workRelayNumber & 1)
			{
				addLogMessage("error close wiring pi relay 0");
			}
		}
		else
		{
			result.digitalOutputStatus ^= 0x00000001;
		}
	}
	if(m_wiringPiPinCheckRelay1 >= 0 && m_wiringPiCheckRelay1 != 0)
	{
		std::string val = "";
		m_wiringPiCheckRelay1->getval_gpio(val);
		if(val != "0")
		{
			if(workRelayNumber & 2)
			{
				addLogMessage("error close wiring pi relay 1");
			}
		}
		else
		{
			result.digitalOutputStatus ^= 0x00000002;
		}
	}
#endif

	return result;
}

DigitalInputOutputStatus Core::readDigitalInputOutput()
{
	addLogMessage("read relay status");

	DigitalInputOutputStatus result;
	result.digitalInputNumber = 0;
	result.digitalInputStatus = 0;
	result.digitalOutputNumber = 0;
	result.digitalOutputStatus = 0;

#ifdef USE_PIFACE_DIGITAL
	result.digitalOutputNumber = 2;
	if(pifacedigital_read_bit(0, OUTPUT, m_piFaceHwAddr))
	{
		result.digitalOutputStatus |= 0x00000001;
	}
	if(pifacedigital_read_bit(1, OUTPUT, m_piFaceHwAddr))
	{
		result.digitalOutputStatus |= 0x00000002;
	}
#endif

#ifdef USE_GPIOD
	result.digitalOutputNumber = 2;
	if(m_wiringPiPinCheckRelay0 >= 0 && m_wiringPiCheckRelay0 != 0)
	{
		std::string val = "";
		m_wiringPiCheckRelay0->getval_gpio(val);
		if(val == "1")
		{
			result.digitalOutputStatus |= 0x00000001;
		}
	}
	if(m_wiringPiPinCheckRelay1 >= 0 && m_wiringPiCheckRelay1 != 0)
	{
		std::string val = "";
		m_wiringPiCheckRelay1->getval_gpio(val);
		if(val == "1")
		{
			result.digitalOutputStatus |= 0x00000002;
		}
	}
#endif

	return result;
}

SingleImage Core::getSingleImage()
{
	SingleImage result;
	m_srcImageMutex.lock();
	result.imageDateTime = m_srcImageDateTime;
	result.image = m_srcImage.clone();
	addLogMessage("get single image");
	m_srcImageMutex.unlock();
	return result;
}

void Core::resetDevice()
{
	m_finishAllThreads = true;
	m_videoCaptureThread.join();
	if(m_videoCapture.isOpened())
	{
		m_videoCapture.release();
	}
	//TODO
	//boost::filesystem::copy_file(bakIniFile, bakIniFile, boost::filesystem::copy_option::overwrite_if_exists);
}

std::string Core::getHomeDir() const
{
	return VEGA2APP_HOME;
}

void Core::reloadAList()
{
	m_aListMutex.lock();
	m_aListTemp = reloadList(listAFile);

	char msgBuffer[1000];
	sprintf(msgBuffer, "reload list A %d", m_aListTemp.size());
	addLogMessage(std::string(msgBuffer));

	m_aListReinited = true;
	m_aListMutex.unlock();
}

void Core::reloadBList()
{
	m_bListMutex.lock();
	m_bListTemp = reloadList(listBFile);

	char msgBuffer[1000];
	sprintf(msgBuffer, "reload list B %d", m_bListTemp.size());
	addLogMessage(std::string(msgBuffer));

	m_bListReinited = true;
	m_bListMutex.unlock();
}

void Core::addLogMessage(const std::string& message, bool withTime)
{
	m_logFileMutex.lock();

	try
	{
		if(withTime)
		{
			std::time_t currentDateTime;
			time(&currentDateTime);
			char buffer[64];
			std::tm* dateTimeTM = std::localtime(&currentDateTime);
			std::strftime(buffer, 64, "%Y-%m-%d-%H-%M-%S-000: ", dateTimeTM);
			m_logFile << std::string(buffer);
		}
		m_logFile << message << std::endl;
		m_logFile.flush();
		if(m_logMaxRowNumber > 0)
		{
			m_logCurrentRowNumber++;
			if(m_logCurrentRowNumber >= m_logMaxRowNumber)
			{
				m_logCurrentRowNumber = 0;
				m_logFile.close();
				boost::filesystem::rename(logFile, oldLogFile);
				m_logFile.open(logFile, std::ofstream::out);
			}
		}
	}
	catch(std::exception& e)
	{
		m_logFile.open(logFile, std::ofstream::out);
	}
	catch(...)
	{
		m_logFile.open(logFile, std::ofstream::out);
	}
	m_logFileMutex.unlock();
}

bool Core::checkImei(const std::string& imei)
{
	if(std::find(m_imeiList.begin(), m_imeiList.end(), imei) != m_imeiList.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Core::processVideoCapture()
{
	bool started = false;
	bool afterStopCapture = false;
	for(;;)
	{
		try
		{
			checkOpenRelayTimeoutAndClose();
			if(m_settings)
			{
				std::string ipCameraUrl = m_settings->getIpCameraUrl();
				if(ipCameraUrl.empty())
				{
					if(m_finishAllThreads)
					{
						addLogMessage("processVideoCapture finishAllThreads");
						break;
					}
					usleep(1000000);
					continue;
				}
			}
			if(m_testPlateString.empty())
			{
				started = startCaptureWebCam();
				if(!started)
				{
					started = startCaptureStream();
				}
				m_srcImageReadyForPlateRecognize = false;
				if(started)
				{
					addLogMessage("start capture");
					while(m_videoCapture.isOpened())
					{
						if(m_finishAllThreads)
						{
							break;
						}
						if(m_videoCapture.grab())
						{
							if(!m_srcImageReadyForPlateRecognize)
							{
								m_srcImageMutex.lock();
								bool res = m_videoCapture.retrieve(m_srcImage);
								time(&m_srcImageDateTime);
								m_srcImageReadyForPlateRecognize = true;
								m_srcImageMutex.unlock();
								if(res)
								{
									if(m_plateReadyVisitor)
									{
										m_plateReadyVisitor->imageReady();
									}
								}
								else
								{
									break;
								}
							}
						}
						else
						{
							break;
						}
						checkOpenRelayTimeoutAndClose();
					}
				}
				addLogMessage("stop capture");
			}
			else
			{
				m_srcImageReadyForPlateRecognize = true;
			}
			if(m_finishAllThreads)
			{
				addLogMessage("processVideoCapture finishAllThreads");
				m_videoCapture.release();
				break;
			}
			m_videoCapture.release();
			usleep(1000000 / m_fps);
		}
		catch(boost::thread_interrupted& e)
		{
			addLogMessage("Video thread interrupted (and ended)");
			m_videoCapture.release();
			return;
		}
		catch(std::exception& e)
		{
			addLogMessage(std::string("Exception caught from video thread: ") + std::string(e.what()));
			m_videoCapture.release();
		}
		catch(...)
		{
			addLogMessage("Unknown exception caught from video thread");
			m_videoCapture.release();
		}
	}
}

bool Core::plateRecognize(const cv::Mat& image)
{
	std::time_t startDateTime;
	time(&startDateTime);
	CvRect rects[IANPR_PLATE_RESULT_SIZE];
	int all = IANPR_PLATE_RESULT_SIZE;
	char** res = new char*[all];
	for(int i = 0; i < all; i++)
	{
		res[i] = new char[m_anprOption.max_text_size + 2];
	}

	CvRect usedRect = m_settingsRect;

	if(image.rows < usedRect.y + usedRect.height)
	{
		usedRect.height = image.rows - usedRect.y;
	}
	if(image.cols < usedRect.x + usedRect.width)
	{
		usedRect.width = image.cols - usedRect.x;
	}
	int anprError = -9999;
	if(m_anprOption.type_number == 4 || m_anprOption.type_number == 7)
	{
		IplImage iplImage = image;
		if(usedRect.width <= 0 || usedRect.height <= 0)
		{
			anprError = anprPlate(&iplImage, m_anprOption, &all, rects, res);
		}
		else
		{
			anprError = anprPlateRect(&iplImage, usedRect, m_anprOption, &all, rects, res);
		}
	}
	else
	{
		cv::Mat grayImage;
		if(image.channels() == 1)
		{
			grayImage = image;
		}
		else if(image.channels() == 3)
		{
			cv::cvtColor(image, grayImage, CV_BGR2GRAY);
		}
		else if(image.channels() == 4)
		{
			cv::cvtColor(image, grayImage, CV_BGRA2GRAY);
		}
		IplImage iplImage = grayImage;
		if(usedRect.width <= 0 || usedRect.height <= 0)
		{
			anprError = anprPlate(&iplImage, m_anprOption, &all, rects, res);
		}
		else
		{
			anprError = anprPlateRect(&iplImage, usedRect, m_anprOption, &all, rects, res);
		}
	}
	std::time_t elapsedDateTime;
	time(&elapsedDateTime);
	int elapsed = elapsedDateTime - startDateTime;

	if(m_maxPlateCountTime > 0 && elapsedDateTime - m_createPlateMapDateTime >= m_maxPlateCountTime)
	{
		m_plateCount = 0;
		m_plateCountMap.clear();
		m_plateCountMapMaxValueKey = "";
	}

	bool result = false;
	if(anprError == 0)
	{
		m_plateMutex.lock();
		if(all > 0)
		{
			result = true;
		}
		for(int i = 0; i < all; i++)
		{
			std::string plateString = std::string(res[i]);

			addLogMessage(std::string("plate ") + plateString + std::string(" ready"));

			CvRect plateRect = rects[i];

			if(usedRect.width > 0 && usedRect.height > 0)
			{
				plateRect.x += usedRect.x;
				plateRect.y += usedRect.y;
			}
			
			bool fixRect = false;
			if(plateRect.width > image.cols)
			{
				plateRect.width = image.cols;
				fixRect = true;
			}
			if(plateRect.height > image.rows)
			{
				plateRect.height = image.rows;
				fixRect = true;
			}
			if(plateRect.x < 0)
			{
				plateRect.x = 0;
				fixRect = true;
			}
			if(plateRect.y < 0)
			{
				plateRect.y = 0;
				fixRect = true;
			};
			if(plateRect.x + plateRect.width > image.cols)
			{
				plateRect.x = image.cols - plateRect.width;
				fixRect = true;
			}
			if(plateRect.y + plateRect.height > image.rows)
			{
				plateRect.y = image.rows - plateRect.height;
				fixRect = true;
			}

			if(m_logLevel == 1)
			{			
				cv::Mat plateImage = cv::Mat(image, plateRect);
				if(!plateImage.empty())
				{
					imwrite("/opt/cerberus/plate0.jpg", plateImage);
				}
			}
			
			std::string text = plateString + std::string(" (") + boost::lexical_cast<std::string>(elapsed) + std::string(" sec)");
			if(m_plateReadyVisitor)
			{
				m_plateReadyVisitor->plateReady(plateString, text);
			}
			if(m_lastPlateString != plateString) //Исключение дублирования
			{
				m_plateCount = 1;
			}
			else
			{
				m_plateCount++;
			}
			m_lastPlateString = plateString;

			std::time_t currentDateTime;
			time(&currentDateTime);
			if(m_checkDirFlag)
			{
				updateDirection(currentDateTime, plateString, plateRect);
			}

			if(m_minPlateCountInMap > 0)
			{
				if(m_plateCountMap.empty())
				{
					time(&m_createPlateMapDateTime);
				}
				if(m_plateCountMap.find(plateString) == m_plateCountMap.end())
				{
					m_plateCountMap[plateString] = 1;
				}
				else
				{
					m_plateCountMap[plateString] = m_plateCountMap[plateString] + 1;
				}
				if(!m_plateCountMapMaxValueKey.empty())
				{
					if(m_plateCountMap[plateString] > m_plateCountMap[m_plateCountMapMaxValueKey])
					{
						m_plateCountMapMaxValueKey = plateString;
					}
				}
				else
				{
					m_plateCountMapMaxValueKey = plateString;
				}
				plateString = m_plateCountMapMaxValueKey;
			}
			addLogMessage("plate recognized: " + plateString);
			int elapsedMap = elapsedDateTime - m_createPlateMapDateTime;
			if(m_plateCount >= m_minPlateSequence || (m_minPlateCountInMap > 0 && m_plateCountMap[m_plateCountMapMaxValueKey] >= m_minPlateCountInMap))
			{
				VegaCommand40000_DataMessage::DirectionType direction = VegaCommand40000_DataMessage::NoDirectionType;
				if(!m_checkDirFlag)
				{
					testForListsAndCreateMessage(plateString, direction, plateRect, image);
				}
				else
				{
					if(m_directionMap.find(plateString) != m_directionMap.end())
					{
						direction = m_directionMap[plateString].direction;
					}
					if(direction == VegaCommand40000_DataMessage::AproachDirectionType)
					{
						testForListsAndCreateMessage(plateString, direction, plateRect, image);
					}
				}
				m_plateCount = 0;
				m_plateCountMap.clear();
				m_plateCountMapMaxValueKey = "";
			}
			if(m_maxPlateCountTime > 0 && elapsedMap >= m_maxPlateCountTime)
			{
				m_plateCount = 0;
				m_plateCountMap.clear();
				m_plateCountMapMaxValueKey = "";
			}
		}
		m_plateMutex.unlock();
	}
	else
	{
		if(m_plateReadyVisitor)
		{
			m_plateReadyVisitor->plateReady("", "Error");
		}
	}
	for(int i = 0; i < IANPR_PLATE_RESULT_SIZE; i++)
	{
		delete [] res[i];
	}
	delete [] res;
	return result;
}

#ifndef NO_GUARDANT
void Core::verifySign()
{
	m_dongleVerifyCount--;
	if(m_dongleVerifyCount <= 0)
	{
		bool dongleExist = false;
		if(!m_grdDongle)
		{
			addLogMessage("error: no dongle connected");
			exit(-1);
		}
		if(!m_alg0Activated)
		{
			int ret = m_grdDongle->PI_Activate(0x0, m_grdRdKey);
			guardantErrorHandling(m_grdDongle, ret);
			m_alg0Activated = true;
		}
		if(m_verifyState == VERIFY_START)
		{
			for(int i = 0; i < GrdECC160_MESSAGE_SIZE; i++)
			{
				m_dongleMessage[i] = (char)(rand() % 256);
			}
			int ret = m_grdDongle->Sign(0x0, sizeof(m_dongleMessage), m_dongleMessage, sizeof(m_dongleDigSignMessage), m_dongleDigSignMessage, NULL);
			guardantErrorHandling(m_grdDongle, ret);
			dongleExist = true;
			m_verifyState = VERIFY_CHECK;
		}
		else if(m_verifyState == VERIFY_CHECK)
		{
			int ret = m_grdDongle->VerifySign(GrdVSC_ECC160, m_pubEccKeyByteArrayLen, m_pubEccKeyByteArray, sizeof(m_dongleMessage), m_dongleMessage,
				sizeof(m_dongleDigSignMessage), m_dongleDigSignMessage, NULL);
			guardantErrorHandling(m_grdDongle, ret);
			dongleExist = true;
			m_verifyState = VERIFY_START;
		}
		if(!dongleExist)
		{
			addLogMessage("error: no dongle connected");
			exit(-1);
		}
		m_dongleVerifyCount = (VERIFY_MAX_COUNT / 2) + (rand() % (VERIFY_MAX_COUNT + 1));
	}
}
#endif

void Core::processPlateRecognize()
{
	for(;;)
	{
		try
		{
			m_srcImageMutex.lock();
			if(m_srcImageReadyForPlateRecognize)
			{
				cv::Mat image;
				try
				{
					if(!m_srcImage.empty())
					{
						image = m_srcImage.clone();
					}
				}
				catch(std::exception& e)
				{
					addLogMessage(std::string("Exception caught from recognize thread in srcImage.clone: ") + std::string(e.what()));
					if(!image.empty())
					{
						image.release();
					}
				}
				catch(...)
				{
					addLogMessage("Unknown exception caught from recognize thread in srcImage.clone");
					if(!image.empty())
					{
						image.release();
					}
				}
				m_srcImageReadyForPlateRecognize = false;
				m_srcImageMutex.unlock();

#ifndef NO_GUARDANT
				m_dongleVerifyMutex.lock();
				m_verifySignThread = boost::thread(&Core::verifySign, this);
//#if BOOST_VERSION < 105000
				// Use of Boost.DateTime in Boost.Thread is deprecated as of 1.50
				boost::posix_time::ptime deadline = boost::posix_time::microsec_clock::local_time() + boost::posix_time::seconds(VERIFY_SEC);
//#else
//				boost::chrono::system_clock::time_point deadline = boost::chrono::system_clock::now() + boost::chrono::seconds(VERIFY_SEC);
//#endif
				if(!m_verifySignThread.timed_join(deadline))
				{
					exit(-2);
				}
				m_dongleVerifyMutex.unlock();
#endif

				if(m_testPlateString.empty())
				{
					bool plateFinded = plateRecognize(image);
					if(m_wiringPiNeedVideoCapture)
					{
						if(plateFinded)
						{
							m_wiringPiNeedVideoCapture = false;
						}
						else
						{
							std::time_t currentDateTime;
							time(&currentDateTime);
							if(m_wiringPiCommandTime + m_wiringPiVideoCaptureDelaySec <= currentDateTime)
							{
								m_wiringPiNeedVideoCapture = false;
								testForListsAndCreateMessage(m_wiringPiEmptyPlateVideoCapture, VegaCommand40000_DataMessage::NoDirectionType, CvRect(), image);
								addLogMessage("Send empty plate by wiringPi event.");
							}
						}
					}
				}
				else
				{
					testForListsAndCreateMessage(m_testPlateString, VegaCommand40000_DataMessage::NoDirectionType, m_testPlateRect, cv::Mat());
				}
				if(!image.empty())
				{
					image.release();
				}
			}
			else
			{
				m_srcImageMutex.unlock();
			}
			if(m_finishAllThreads)
			{
				addLogMessage("processPlateRecognize finishAllThreads");
				break;
			}
			usleep(1000000 / m_fps);
		}
		catch(boost::thread_interrupted& e)
		{
			addLogMessage("Recognize thread interrupted (and ended)");
			return;
		}
		catch(std::exception& e)
		{
			addLogMessage(std::string("Exception caught from recognize thread: ") + std::string(e.what()));
		}
		catch(...)
		{
			addLogMessage("Unknown exception caught from recognize thread");
		}
	}
}

void Core::processSendVegaPacket(int index)
{
	std::string portName = "NO PORT";
	std::string host;
	uint16_t port = 0;

	if(index < m_portNameVector.size())
	{
		portName = m_portNameVector[index];
	}
	if(m_settings)
	{
		if(portName == portAString)
		{
			host = m_settings->getHostAddressA();
			port = m_settings->getPortA();
		}
		else if(portName == portNAString)
		{
			host = m_settings->getHostAddressNA();
			port = m_settings->getPortNA();
		}
		else if(portName == portBString)
		{
			host = m_settings->getHostAddressB();
			port = m_settings->getPortB();
		}
		else if(portName == portNBString)
		{
			host = m_settings->getHostAddressNB();
			port = m_settings->getPortNB();
		}
	}
	for(;;)
	{
		VegaPacket* packet = 0;
		try
		{
			if(index < 0 || index >= m_messageQueueMutexVector.size() || index >= m_messageQueueVector.size())
			{
				addLogMessage("Wrong send message thread index");
			}
			m_messageQueueMutexVector[index]->lock();
			bool emptyMessageQueue = m_messageQueueVector[index].empty();
			m_messageQueueMutexVector[index]->unlock();

#ifdef USE_MONGODB
			if(m_dbInterface && emptyMessageQueue)
			{
				initDb();
				VegaCommand40000_DataMessage* dataMessageFromStore = m_dbInterface->getNextVegaCommand40000(portName);
				if(dataMessageFromStore)
				{
					m_messageQueueMutexVector[index]->lock();
					m_messageQueueVector[index].push(new VegaPacket(dataMessageFromStore, host, port));
					m_messageQueueMutexVector[index]->unlock();
				}
			}
#endif

			m_messageQueueMutexVector[index]->lock();
			emptyMessageQueue = m_messageQueueVector[index].empty();
			m_messageQueueMutexVector[index]->unlock();
			while(!emptyMessageQueue)
			{
				m_messageQueueMutexVector[index]->lock();
				clearMessageQueueForTimeout(index);
				if(m_messageQueueVector[index].empty())
				{
					m_messageQueueMutexVector[index]->unlock();
					break;
				}
				packet = m_messageQueueVector[index].front();
				m_messageQueueMutexVector[index]->unlock();
				if(packet)
				{
					VegaCommand* command = packet->getVegaCommand();
					std::string host = packet->getHost();
					uint16_t port = packet->getPort();
					if(command && host.size() > 0 && port > 0)
					{
						boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(host), port);
						boost::asio::ip::tcp::socket sock(m_ioService);
						boost::system::error_code ec;
						sock.connect(ep, ec);
						if(ec)
						{
							addLogMessage(std::string("Error code on connect in send message thread for ") + portName + std::string(": ") + ec.message());
							break;
						}
						else
						{
							boost::asio::streambuf streamBuffer;
							std::ostream outData(&streamBuffer);
							command->serialize(outData);
							boost::asio::write(sock, streamBuffer, ec);
							if(ec)
							{
								addLogMessage(std::string("Error code on write in send message thread for ") + portName + std::string(": ") + ec.message());
								sock.close();
								break;
							}
							//sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
							sock.close();
							m_messageQueueMutexVector[index]->lock();
							VegaCommand40000_DataMessage* dataMessage = dynamic_cast<VegaCommand40000_DataMessage*>(command);
							if(dataMessage)
							{
								addLogMessage(std::string("Message with plate ") + dataMessage->getPlate() + std::string(" sended"));
								if(dataMessage->isFromStore())
								{

#ifdef USE_MONGODB
									if(m_dbInterface)
									{
										initDb();
										if(m_dbInterface->removeVegaCommand40000(portName))
										{
											addLogMessage(std::string("MongoDB remove for ") + portName + std::string(" plate ") + dataMessage->getPlate() +
												std::string(". Container count: ") + std::to_string(m_dbInterface->getVegaCommand40000Count(portName)) +
												std::string("/") + std::to_string(m_dbInterface->getAllVegaCommand40000Count()));
										}
										else
										{
											addLogMessage(std::string("MongoDB remove plate ERROR"));
											m_dbInited = false;
										}
									}
#endif

								}
							}
							m_messageQueueVector[index].pop();
							m_messageQueueMutexVector[index]->unlock();
						}
					}
					delete packet;
					packet = 0;
				}
				m_messageQueueMutexVector[index]->lock();
				emptyMessageQueue = m_messageQueueVector[index].empty();
				m_messageQueueMutexVector[index]->unlock();
			}
			if(m_finishAllThreads)
			{
				addLogMessage("processSendVegaPacket finishAllThreads");
				break;
			}
			usleep(1000000 / m_fps);
		}
		catch(boost::thread_interrupted& e)
		{
			addLogMessage("Send message thread interrupted (and ended)");
			return;
		}
		catch(std::exception& e)
		{
			addLogMessage(std::string("Exception caught from send message thread: ") + std::string(e.what()));
		}
		catch(...)
		{
			addLogMessage("Unknown exception caught from send message thread");
		}
	}
}

void Core::setTestPlateString(const std::string& testPlateString)
{
	m_testPlateString = testPlateString;
}

void Core::setTestPlateRect(const CvRect& testPlateRect)
{
	m_testPlateRect = testPlateRect;
}

void Core::setWiringPiNeedVideoCapture()
{
	time(&m_wiringPiCommandTime);
	m_wiringPiNeedVideoCapture = true;
	if(m_logLevel == 1)
	{
		addLogMessage("videoCaptureWiringPi");
	}
}

std::vector<std::string> Core::reloadList(const std::string& filename)
{
	std::vector<std::string> result;
	std::ifstream infile(filename);
	std::string line;
	while(std::getline(infile, line))
	{
		std::string::size_type index = line.find("\n", 0);
		if(index != std::string::npos)
		{
			std::string plateString = line.substr(0, index);
			if(!plateString.empty())
			{
				result.push_back(plateString);
			}
		}
		else
		{
			if(!line.empty())
			{
				result.push_back(line);
			}
		}
	}
	return result;
}

void Core::updateDirection(std::time_t currentDateTime, const std::string& plateString, const CvRect& plateRect)
{
	addLogMessage(std::string("plate ") + plateString + std::string(" start check"));
	std::vector<std::string> plateForRemoveVector;
	for(std::map<std::string, DirectionInfoStruct>::iterator iter = m_directionMap.begin(); iter != m_directionMap.end(); iter++)
	{
		if(currentDateTime - iter->second.lastDateTime > m_checkDirDataLiveTimeout)
		{
			plateForRemoveVector.push_back(iter->first);
		}
	}
	for(int i = 0; i < plateForRemoveVector.size(); i++)
	{
		addLogMessage(std::string("plate ") + plateForRemoveVector[i] + std::string(" ERASE FROM DIR MAP"));
		m_directionMap.erase(plateForRemoveVector[i]);
	}

	if(m_directionMap.find(plateString) == m_directionMap.end())
	{
		DirectionInfoStruct directionInfo;
		directionInfo.dirScore = 0;
		directionInfo.lastPlateRect = plateRect;
		directionInfo.lastDateTime = currentDateTime;
		directionInfo.direction = VegaCommand40000_DataMessage::UnknowDirectionType;
		m_directionMap[plateString] = directionInfo;
		addLogMessage(std::string("plate ") + plateString + std::string(" NEW"));
	}
	else
	{
		DirectionInfoStruct directionInfo = m_directionMap[plateString];
		directionInfo.lastDateTime = currentDateTime;
		if(directionInfo.direction != VegaCommand40000_DataMessage::AproachDirectionType &&
			directionInfo.direction != VegaCommand40000_DataMessage::GoAwayDirectionType)
		{
			if(m_checkDirUp != 0)
			{
				if(directionInfo.lastPlateRect.y > plateRect.y)
				{
					addLogMessage(std::string("plate ") + plateString + std::string(" UP"));
					directionInfo.dirScore += m_checkDirUp;
				}
			}
			if(m_checkDirDown != 0)
			{
				if(directionInfo.lastPlateRect.y < plateRect.y)
				{
					addLogMessage(std::string("plate ") + plateString + std::string(" DOWN"));
					directionInfo.dirScore += m_checkDirDown;
				}
			}
			if(m_checkDirLeft != 0)
			{
				if(directionInfo.lastPlateRect.x > plateRect.x)
				{
					addLogMessage(std::string("plate ") + plateString + std::string(" LEFT"));
					directionInfo.dirScore += m_checkDirLeft;
				}
			}
			if(m_checkDirRight != 0)
			{
				if(directionInfo.lastPlateRect.x < plateRect.x)
				{
					addLogMessage(std::string("plate ") + plateString + std::string(" RIGTH"));
					directionInfo.dirScore += m_checkDirRight;
				}
			}
			if(m_checkDirRectSizeGoToLarge != 0)
			{
				if(directionInfo.lastPlateRect.width < plateRect.width)
				{
					addLogMessage(std::string("plate ") + plateString + std::string(" TO"));
					directionInfo.dirScore += m_checkDirRectSizeGoToLarge;
				}
			}
			if(m_checkDirRectSizeGoToSmall != 0)
			{
				if(directionInfo.lastPlateRect.width > plateRect.width)
				{
					addLogMessage(std::string("plate ") + plateString + std::string(" FROM"));
					directionInfo.dirScore += m_checkDirRectSizeGoToSmall;
				}
			}
			if(directionInfo.dirScore >= m_checkDirBound)
			{
				directionInfo.direction = VegaCommand40000_DataMessage::AproachDirectionType;
				addLogMessage(std::string("plate ") + plateString + std::string(" GET MAX BOUND"));
			}
			if(directionInfo.dirScore <= (-1) *m_checkDirBound)
			{
				directionInfo.direction = VegaCommand40000_DataMessage::GoAwayDirectionType;
				addLogMessage(std::string("plate ") + plateString + std::string(" GET MIN BOUND"));
			}
			directionInfo.lastPlateRect = plateRect;
		}
		m_directionMap[plateString] = directionInfo;
	}
}

void Core::testForListsAndCreateMessage(const std::string& plateString, VegaCommand40000_DataMessage::DirectionType direction, const CvRect& plateRect,
	const cv::Mat& srcImage)
{
	if(srcImage.empty())
	{
		return;
	}
	std::time_t currentDateTime;
	time(&currentDateTime);

	std::string hostA;
	uint16_t portA = 0;
	int indexA = -1;
	if(!m_settings)
	{
		return;
	}
	std::string portAName = "";
	m_aListMutex.lock();
	if(m_aListReinited)
	{
		m_aListReinited = false;
		m_aList = std::move(m_aListTemp);
	}
	m_aListMutex.unlock();
	if(std::find(m_aList.begin(), m_aList.end(), plateString) != m_aList.end())
	{
		hostA = m_settings->getHostAddressA();
		portA = m_settings->getPortA();
		if(m_sendVegaPacketIndexVector.size() > 0)
		{
			indexA = m_sendVegaPacketIndexVector[0];
		}
		portAName = "Port A";

		if(m_portAAutoOpenRelayNumber >= 0)
		{
			addLogMessage(std::string("Auto open port A relay for plate ") + plateString);
			startDigitalOutput(m_portAAutoOpenRelayNumber);
		}
	}

	std::string hostB;
	uint16_t portB = 0;
	int indexB = -1;
	std::string portBName = "";
	m_bListMutex.lock();
	if(m_bListReinited)
	{
		m_bListReinited = false;
		m_bList = std::move(m_bListTemp);
	}
	m_bListMutex.unlock();
	if(std::find(m_bList.begin(), m_bList.end(), plateString) != m_bList.end())
	{
		hostB = m_settings->getHostAddressB();
		portB = m_settings->getPortB();
		if(m_sendVegaPacketIndexVector.size() > 2)
		{
			indexB = m_sendVegaPacketIndexVector[2];
		}
		portBName = "Port B";

		if(m_portBAutoOpenRelayNumber >= 0)
		{
			addLogMessage(std::string("Auto open port B relay for plate ") + plateString);
			startDigitalOutput(m_portBAutoOpenRelayNumber);
		}
	}

	if(indexA == -1 && indexB == -1)
	{
		hostA = m_settings->getHostAddressNA();
		portA = m_settings->getPortNA();
		if(m_sendVegaPacketIndexVector.size() > 1)
		{
			indexA = m_sendVegaPacketIndexVector[1];
		}
		portAName = "Port NA";

		hostB = m_settings->getHostAddressNB();
		portB = m_settings->getPortNB();
		if(m_sendVegaPacketIndexVector.size() > 3)
		{
			indexB = m_sendVegaPacketIndexVector[3];
		}
		portBName = "Port NB";
	}

	char rectBuffer[64];
	sprintf(rectBuffer, "rect(%d, %d, %d, %d)", plateRect.x, plateRect.y, plateRect.width, plateRect.height);

	CvRect sendPlateRect;
	cv::Mat plateImage;
	if(m_sendPlateRect > 0)
	{
		sendPlateRect = plateRect;
	}
	if(m_sendPlateImage > 0 && plateRect.height > 0 && plateRect.width > 0)
	{
		char rectBuffer2[64];
		sprintf(rectBuffer2, "srcImage(%d, %d)", srcImage.cols, srcImage.rows);

		addLogMessage(std::string("RECT") + std::string(rectBuffer) + std::string(" from ") + std::string(rectBuffer2));
		plateImage = cv::Mat(srcImage, plateRect);
	}
	if(!hostA.empty() && portA > 0 && indexA != -1 && indexA < m_messageQueueMutexVector.size() && indexA < m_messageQueueVector.size())
	{
		m_messageQueueMutexVector[indexA]->lock();
		bool plateSended = false;
		int plateImageStatus = 0;
		if(findPlateTimeDataQueueForTimeout(indexA, plateString) == -1)
		{
			VegaCommand40000_DataMessage* dataMessageA = new VegaCommand40000_DataMessage(currentDateTime, plateString, "RUS", direction, srcImage,
				sendPlateRect, plateImage);

			if(m_logLevel == 1)
			{
				if(!srcImage.empty())
				{
					imwrite("/opt/cerberus/image.jpg", srcImage);
				}
				if(!plateImage.empty())
				{
					imwrite("/opt/cerberus/plate.jpg", plateImage);
				}
			}

			m_messageQueueVector[indexA].push(new VegaPacket(dataMessageA, hostA, portA));
			plateSended = true;
			plateImageStatus = dataMessageA->getPlateImageStatus();

			m_plateTimeDataQueueMutexVector[indexA]->lock();
			m_plateTimeDataQueueVector[indexA].push_front(new PlateTimeData(plateString, currentDateTime));
			m_plateTimeDataQueueMutexVector[indexA]->unlock();
		}
		std::string sendedString = "Message exist in sended";
		if(plateSended)
		{
			sendedString = "Message added in queue";
		}
		std::string imageExistString = "plate image NOT EXIST";
		if(plateImageStatus == 1)
		{
			imageExistString = "plate image EXIST";
		}
		else if(plateImageStatus == 1)
		{
			imageExistString = "plate image ENCODE ERROR";
		}
		addLogMessage(sendedString + std::string(" to ") + portAName + std::string(": ") + plateString + " " + std::string(rectBuffer) + std::string(" ") +
			imageExistString);
		m_messageQueueMutexVector[indexA]->unlock();
	}

	if(!hostB.empty() && portB > 0 && indexB != -1 && indexB < m_messageQueueMutexVector.size() && indexB < m_messageQueueVector.size())
	{
		m_messageQueueMutexVector[indexB]->lock();
		bool plateSended = false;
		int plateImageStatus = 0;
		if(findPlateTimeDataQueueForTimeout(indexB, plateString) == -1)
		{
			VegaCommand40000_DataMessage* dataMessageB = new VegaCommand40000_DataMessage(currentDateTime, plateString, "RUS", direction, srcImage,
				sendPlateRect, plateImage);
			m_messageQueueVector[indexB].push(new VegaPacket(dataMessageB, hostB, portB));
			plateSended = true;
			plateImageStatus = dataMessageB->getPlateImageStatus();

			m_plateTimeDataQueueMutexVector[indexB]->lock();
			m_plateTimeDataQueueVector[indexB].push_front(new PlateTimeData(plateString, currentDateTime));
			m_plateTimeDataQueueMutexVector[indexB]->unlock();

		}
		std::string sendedString = "Not send";
		if(plateSended)
		{
			sendedString = "Send";
		}
		std::string imageExistString = "plate image NOT EXIST";
		if(plateImageStatus == 1)
		{
			imageExistString = "plate image EXIST";
		}
		else if(plateImageStatus == 1)
		{
			imageExistString = "plate image ENCODE ERROR";
		}
		addLogMessage(sendedString + std::string(" to ") + portBName + std::string(":") + plateString + " " + std::string(rectBuffer) + std::string(" ") +
			imageExistString);
		m_messageQueueMutexVector[indexB]->unlock();
	}
	if(!plateImage.empty())
	{
		plateImage.release();
	}
}

void Core::clearMessageQueueForTimeout(int index)
{
	std::time_t currentDateTime;
	time(&currentDateTime);
	if(index < 0 || index >= m_messageQueueVector.size())
	{
		return;
	}
	std::string portName = "NO PORT";
	if(index < m_portNameVector.size())
	{
		portName = m_portNameVector[index];
	}
	if(m_logLevel == 1)
	{
		if(!m_messageQueueVector[index].empty())
		{
			char sizeBuffer[64];
			sprintf(sizeBuffer, "%d", m_messageQueueVector[index].size());
			addLogMessage(std::string("Message queue for ") + portName + std::string(" size = ") + std::string(sizeBuffer));
		}
	}
	while(!m_messageQueueVector[index].empty())
	{
		VegaPacket* packet = m_messageQueueVector[index].front();
		if(packet)
		{
			VegaCommand40000_DataMessage* command = dynamic_cast<VegaCommand40000_DataMessage*>(packet->getVegaCommand());
			if(command)
			{
				std::time_t messageDateTime = command->getMessageDateTime();
				if(messageDateTime != 0 && currentDateTime - messageDateTime > m_messageTimeLiveSec)
				{
					m_messageQueueVector[index].pop();

#ifdef USE_MONGODB
					if(m_dbInterface && !command->isFromStore())
					{
						initDb();
						if(m_dbInterface->addVegaCommand40000(portName, command))
						{
							char buffer[64];
							std::tm* dateTimeTM = std::localtime(&messageDateTime);
							std::strftime(buffer, 64, "%Y-%m-%d-%H-%M-%S-000: ", dateTimeTM);
							m_logFile << std::string(buffer);

							addLogMessage(std::string("MongoDB insert for ") + portName + std::string(" plate ") + command->getPlate() +
								std::string(". rec time[") + std::string(buffer) + ("] Container count: ") + std::to_string(m_dbInterface->getVegaCommand40000Count(portName)) + std::string("/") +
								std::to_string(m_dbInterface->getAllVegaCommand40000Count()));
						}
						else
						{
							addLogMessage(std::string("MongoDB insert plate ERROR"));
							m_dbInited = false;
						}
					}
#endif

					delete packet;
					continue;
				}
			}
		}
		break;
	}
	if(m_logLevel == 1)
	{
		if(m_messageQueueVector[index].empty())
		{
			addLogMessage(std::string("Message queue for ") + portName + std::string(" size = 0"));
		}
	}
}

int Core::findPlateTimeDataQueueForTimeout(int index, const std::string& plate)
{
	std::time_t currentDateTime;
	time(&currentDateTime);
	if(index < 0 || index >= m_plateTimeDataQueueVector.size())
	{
		return -1;
	}
	m_plateTimeDataQueueMutexVector[index]->lock();
	while(!m_plateTimeDataQueueVector[index].empty())
	{
		PlateTimeData* plateTimeData = m_plateTimeDataQueueVector[index].back();
		if(plateTimeData)
		{
			std::time_t messageDateTime = plateTimeData->getDateTime();
			if(messageDateTime != 0 && currentDateTime - messageDateTime > m_messageSendTimeoutSec)
			{
				m_plateTimeDataQueueVector[index].pop_back();
				delete plateTimeData;
				continue;
			}
		}
		break;
	}
	for(int i = 0; i < m_plateTimeDataQueueVector[index].size(); i++)
	{
		if(m_plateTimeDataQueueVector[index][i]->getPlateNumber() == plate)
		{
			m_plateTimeDataQueueMutexVector[index]->unlock();
			return i;
		}
	}
	m_plateTimeDataQueueMutexVector[index]->unlock();
	return -1;
}

void Core::testForAddSendVegaPacketThread(const std::string& host, uint16_t port, int portIndex)
{
	if(!host.empty() && port > 0)
	{
		int index = m_sendVegaPacketThreadVector.size();
		if(portIndex == 0)
		{
			m_portNameVector.push_back(portAString);
		}
		else if(portIndex == 1)
		{
			m_portNameVector.push_back(portNAString);
		}
		else if(portIndex == 2)
		{
			m_portNameVector.push_back(portBString);
		}
		else if(portIndex == 3)
		{
			m_portNameVector.push_back(portNBString);
		}

		m_sendVegaPacketIndexVector.push_back(index);
		m_messageQueueMutexVector.push_back(new boost::mutex());
		m_messageQueueVector.push_back(std::queue<VegaPacket*>());
		m_plateTimeDataQueueMutexVector.push_back(new boost::mutex());
		m_plateTimeDataQueueVector.push_back(std::deque<PlateTimeData*>());
		m_sendVegaPacketThreadVector.push_back(boost::thread(&Core::processSendVegaPacket, this, index));
	}
	else
	{
		m_sendVegaPacketIndexVector.push_back(-1);
	}
}

void Core::checkOpenRelayTimeoutAndClose()
{
	if(m_openRelayTimeoutSec > 0 && m_openRelay0Time != 0)
	{
		std::time_t currentDateTime;
		time(&currentDateTime);
		if(currentDateTime - m_openRelay0Time >= m_openRelayTimeoutSec)
		{
			addLogMessage(std::string("Close relay 0 by timeout"));
			m_openRelay0Time = 0;
			stopDigitalOutput(0);
		}
	}
	if(m_openRelayTimeoutSec > 0 && m_openRelay1Time != 0)
	{
		std::time_t currentDateTime;
		time(&currentDateTime);
		if(currentDateTime - m_openRelay1Time >= m_openRelayTimeoutSec)
		{
			addLogMessage(std::string("Close relay 1 by timeout"));
			m_openRelay1Time = 0;
			stopDigitalOutput(1);
		}
	}
}

#ifdef USE_MONGODB

void Core::initDb()
{
	if(m_dbInited)
	{
		return;
	}
	m_dbInited = m_dbInterface->init();
	if(m_dbInited)
	{
		m_dbInterface->freeDb();
	}
}

#endif
