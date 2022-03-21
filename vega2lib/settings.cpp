#include "settings.h"

#include <boost/property_tree/ini_parser.hpp>

std::string Settings::logLevelKey = "LOG_LEVEL";
std::string Settings::portAKey = "PORT_A";
std::string Settings::portNAKey = "PORT_NA";
std::string Settings::portBKey = "PORT_B";
std::string Settings::portNBKey = "PORT_NB";
std::string Settings::portControlKey = "PORT_CTRL";
std::string Settings::portApkControlKey = "PORT_APK_CTRL";
std::string Settings::ipCameraUrlKey = "IP_CAMERA_URL";
std::string Settings::webCameraNumberKey = "WEB_CAMERA_NUMBER";
std::string Settings::fpsKey = "FPS";
std::string Settings::recognizeThreadNumberKey = "RECOGNIZE_THREAD_NUMBER";
std::string Settings::logMaxRowNumberKey = "LOG_MAX_ROW_NUMBER";

std::string Settings::minPlateSequenceKey = "MIN_PLATE_SEQUENCE";
std::string Settings::minPlateCountInMapKey = "MIN_PLATE_COUNT_IN_MAP";
std::string Settings::maxPlateCountTimeKey = "MAX_PLATE_COUNT_TIME";

std::string Settings::anprDetectModeKey = "PLATE_DETECT_MODE";
std::string Settings::anprMinPlateSizeKey = "PLATE_MIN_PLATE_SIZE";
std::string Settings::anprMaxPlateSizeKey = "PLATE_MAX_PLATE_SIZE";
std::string Settings::anprMaxTextSizeKey = "PLATE_MAX_TEXT_SIZE";

std::string Settings::anprPlateRectXKey = "PLATE_RECT_X";
std::string Settings::anprPlateRectYKey = "PLATE_RECT_Y";
std::string Settings::anprPlateRectWidthKey = "PLATE_RECT_WIDTH";
std::string Settings::anprPlateRectHeightKey = "PLATE_RECT_HEIGHT";

std::string Settings::messageTimeToLiveSecKey = "MSG_TIME_LIVE_SEC";
std::string Settings::messageSendTimeoutSecKey = "MSG_SEND_TIMEOUT_SEC";
std::string Settings::storeNoSendedPlateKey = "STORE_NO_SENDED_PLATE";

std::string Settings::workRelayWaitTimeKey = "WORK_RELAY_WAIT_TIME";
std::string Settings::workRelayNumberKey = "WORK_RELAY_NUMBER";

std::string Settings::portAAutoOpenRelayNumberKey = "PORT_A_AUTO_OPEN_RELAY_NUMBER";
std::string Settings::portBAutoOpenRelayNumberKey = "PORT_B_AUTO_OPEN_RELAY_NUMBER";
std::string Settings::openRelayTimeoutSecKey = "OPEN_RELAY_TIMEOUT_SEC";

std::string Settings::sendPlateRectKey = "SEND_PLATE_RECT";
std::string Settings::sendPlateImageKey = "SEND_PLATE_IMAGE";

std::string Settings::checkDirUpKey = "CHECK_DIR_UP";
std::string Settings::checkDirDownKey = "CHECK_DIR_DOWN";
std::string Settings::checkDirLeftKey = "CHECK_DIR_LEFT";
std::string Settings::checkDirRightKey = "CHECK_DIR_RIGHT";
std::string Settings::checkDirRectSizeGoToLargeKey = "CHECK_DIR_RECT_SIZE_GOTO_LARGE";
std::string Settings::checkDirRectSizeGoToSmallKey = "CHECK_DIR_RECT_SIZE_GOTO_SMALL";
std::string Settings::checkDirBoundKey = "CHECK_DIR_BOUND";
std::string Settings::checkDirDataLiveTimeoutKey = "CHECK_DIR_DATA_LIVE_TIMEOUT";

std::string Settings::wiringPiPinWorkRelay0Key = "WIRING_PI_PIN_WORK_RELAY_0";
std::string Settings::wiringPiPinCheckRelay0Key = "WIRING_PI_PIN_CHECK_RELAY_0";
std::string Settings::wiringPiPinWorkRelay1Key = "WIRING_PI_PIN_WORK_RELAY_1";
std::string Settings::wiringPiPinCheckRelay1Key = "WIRING_PI_PIN_CHECK_RELAY_1";

std::string Settings::wiringPiPinNumberVideoCaptureFromLowToHighKey = "WIRING_PI_PIN_VIDEO_CAPTURE_FROM_LOW_TO_HIGH";
std::string Settings::wiringPiPinNumberVideoCaptureFromHighToLowKey = "WIRING_PI_PIN_VIDEO_CAPTURE_FROM_HIGH_TO_LOW";
std::string Settings::wiringPiEmptyPlateVideoCaptureKey = "WIRING_PI_EMPTY_PLATE_VIDEO_CAPTURE";
std::string Settings::wiringPiVideoCaptureDelaySecKey = "WIRING_PI_VIDEO_CAPTURE_DELAY_SEC";

Settings::Settings(const std::string& fileName)
{
	try
	{
		boost::property_tree::ini_parser::read_ini(fileName, m_propertyTree);
	}
	catch(const boost::property_tree::ini_parser_error& error)
	{
		m_propertyTree.put(logLevelKey, 0);
		m_propertyTree.put(portAKey, "192.168.111.59:32000");
		m_propertyTree.put(portNAKey, "192.168.111.59:32002");
		m_propertyTree.put(portBKey, "192.168.111.59:32001");
		m_propertyTree.put(portNBKey, "192.168.111.59:32003");
		m_propertyTree.put(portControlKey, 31000);
		m_propertyTree.put(portApkControlKey, 31500);
		m_propertyTree.put(ipCameraUrlKey, "rtsp://192.168.111.36:554/axis-media/media.amp");
		m_propertyTree.put(webCameraNumberKey, -1);
		m_propertyTree.put(fpsKey, 30);
		m_propertyTree.put(recognizeThreadNumberKey, 4);
		m_propertyTree.put(logMaxRowNumberKey, 100000);

		m_propertyTree.put(minPlateSequenceKey, 1);
		m_propertyTree.put(minPlateCountInMapKey, 1);
		m_propertyTree.put(maxPlateCountTimeKey, -1);

		m_propertyTree.put(anprDetectModeKey, 6);
		m_propertyTree.put(anprMinPlateSizeKey, 2000);
		m_propertyTree.put(anprMaxPlateSizeKey, 20000);
		m_propertyTree.put(anprMaxTextSizeKey, 15);

		m_propertyTree.put(anprPlateRectXKey, 0);
		m_propertyTree.put(anprPlateRectYKey, 0);
		m_propertyTree.put(anprPlateRectWidthKey, 0);
		m_propertyTree.put(anprPlateRectHeightKey, 0);

		m_propertyTree.put(messageTimeToLiveSecKey, 10);
		m_propertyTree.put(messageSendTimeoutSecKey, 30);
		m_propertyTree.put(storeNoSendedPlateKey, 1000);

		m_propertyTree.put(workRelayWaitTimeKey, 100000);
		m_propertyTree.put(workRelayNumberKey, 0);
		m_propertyTree.put(portAAutoOpenRelayNumberKey, -1);
		m_propertyTree.put(portBAutoOpenRelayNumberKey, -1);
		m_propertyTree.put(openRelayTimeoutSecKey, 15);

		m_propertyTree.put(sendPlateRectKey, 1);
		m_propertyTree.put(sendPlateImageKey, 1);

		m_propertyTree.put(checkDirUpKey, 0);
		m_propertyTree.put(checkDirDownKey, 0);
		m_propertyTree.put(checkDirLeftKey, 0);
		m_propertyTree.put(checkDirRightKey, 0);
		m_propertyTree.put(checkDirRectSizeGoToLargeKey, 1);
		m_propertyTree.put(checkDirRectSizeGoToSmallKey, -1);
		m_propertyTree.put(checkDirBoundKey, 2);
		m_propertyTree.put(checkDirDataLiveTimeoutKey, 10);

		m_propertyTree.put(wiringPiPinWorkRelay0Key, -1);
		m_propertyTree.put(wiringPiPinCheckRelay0Key, -1);
		m_propertyTree.put(wiringPiPinWorkRelay1Key, -1);
		m_propertyTree.put(wiringPiPinCheckRelay1Key, -1);

		m_propertyTree.put(wiringPiPinNumberVideoCaptureFromLowToHighKey, -1);
		m_propertyTree.put(wiringPiPinNumberVideoCaptureFromHighToLowKey, -1);
		m_propertyTree.put(wiringPiEmptyPlateVideoCaptureKey, "NO");
		m_propertyTree.put(wiringPiVideoCaptureDelaySecKey, 0);

		boost::property_tree::ini_parser::write_ini(fileName, m_propertyTree);
	}
}

Settings::~Settings()
{

}

int32_t Settings::getLogLevel() const
{
	return getInt32(logLevelKey, 0);
}

std::string Settings::getHostAddressA() const
{
	return getHostAddress(portAKey);
}

uint16_t Settings::getPortA() const
{
	return getPort(portAKey);
}

std::string Settings::getHostAddressNA() const
{
	return getHostAddress(portNAKey);
}

uint16_t Settings::getPortNA() const
{
	return getPort(portNAKey);
}

std::string Settings::getHostAddressB() const
{
	return getHostAddress(portBKey);
}

uint16_t Settings::getPortB() const
{
	return getPort(portBKey);
}

std::string Settings::getHostAddressNB() const
{
	return getHostAddress(portNBKey);
}

uint16_t Settings::getPortNB() const
{
	return getPort(portNBKey);
}

uint16_t Settings::getPortControl() const
{
	return m_propertyTree.get<uint16_t>(portControlKey, 0);
}

uint16_t Settings::getPortApkControl() const
{
	return m_propertyTree.get<uint16_t>(portApkControlKey, 0);
}

std::string Settings::getIpCameraUrl() const
{
	return m_propertyTree.get<std::string>(ipCameraUrlKey);
}

int32_t Settings::getWebCameraNumber() const
{
	return getInt32(webCameraNumberKey, -1);
}

int32_t Settings::getFps() const
{
	return getInt32(fpsKey, 30);
}

int32_t Settings::getRecognizeThreadNumber() const
{
	return getInt32(recognizeThreadNumberKey, 4);
}

int32_t Settings::getLogMaxRowNumber() const
{
	return getInt32(logMaxRowNumberKey, 100000);
}

int32_t Settings::getMinPlateSequence() const
{
	return getInt32(minPlateSequenceKey, 1);
}

int32_t Settings::getMinPlateCountInMap() const
{
	return getInt32(minPlateCountInMapKey, 1);
}

int32_t Settings::getMaxPlateCountTime() const
{
	return getInt32(maxPlateCountTimeKey, -1);
}
int32_t Settings::getAnprDetectMode() const
{
	return getInt32(anprDetectModeKey, 6);
}

int32_t Settings::getAnprMinPlateSize() const
{
	return getInt32(anprMinPlateSizeKey, 2000);
}

int32_t Settings::getAnprMaxPlateSize() const
{
	return getInt32(anprMaxPlateSizeKey, 20000);
}

int32_t Settings::getAnprMaxTextSize() const
{
	return getInt32(anprMaxTextSizeKey, 15);
}

CvRect Settings::getRect() const
{
	int32_t x = getInt32(anprPlateRectXKey, 0);
	int32_t y = getInt32(anprPlateRectYKey, 0);
	int32_t w = getInt32(anprPlateRectWidthKey, 0);
	int32_t h = getInt32(anprPlateRectHeightKey, 0);
	return cvRect(x, y, w, h);
}

int32_t Settings::getMessageTimeToLiveSec() const
{
	return getInt32(messageTimeToLiveSecKey, 10);
}

int32_t Settings::getMessageSendTimeoutSec() const
{
	return getInt32(messageSendTimeoutSecKey, 30);
}

int32_t Settings::getStoreNoSendedPlate() const
{
	return getInt32(storeNoSendedPlateKey, 1000);
}

int32_t Settings::getWorkRelayWaitTimeMilliSec() const
{
	return getInt32(workRelayWaitTimeKey, 100000);
}

int32_t Settings::getWorkRelayNumber() const
{
	return getInt32(workRelayNumberKey, 0);
}

int32_t Settings::getPortAAutoOpenRelayNumber() const
{
	return getInt32(portAAutoOpenRelayNumberKey, -1);
}

int32_t Settings::getPortBAutoOpenRelayNumber() const
{
	return getInt32(portBAutoOpenRelayNumberKey, -1);
}

int32_t Settings::getOpenRelayTimeoutSec() const
{
	return getInt32(openRelayTimeoutSecKey, 15);
}

int32_t Settings::getSendPlateRect() const
{
	return getInt32(sendPlateRectKey, 1);
}

int32_t Settings::getSendPlateImage() const
{
	return getInt32(sendPlateImageKey, 1);
}

int32_t Settings::getCheckDirUp() const
{
	return getInt32(checkDirUpKey, 0);
}

int32_t Settings::getCheckDirDown() const
{
	return getInt32(checkDirDownKey, 0);
}

int32_t Settings::getCheckDirLeft() const
{
	return getInt32(checkDirLeftKey, 0);
}

int32_t Settings::getCheckDirRight() const
{
	return getInt32(checkDirRightKey, 0);
}

int32_t Settings::getCheckDirRectSizeGoToLarge() const
{
	return getInt32(checkDirRectSizeGoToLargeKey, 1);
}

int32_t Settings::getCheckDirRectSizeGoToSmall() const
{
	return getInt32(checkDirRectSizeGoToSmallKey, -1);
}

int32_t Settings::getCheckDirBound() const
{
	return getInt32(checkDirBoundKey, 2);
}

int32_t Settings::getCheckDirDataLiveTimeout() const
{
	return getInt32(checkDirDataLiveTimeoutKey, 10);
}

int32_t Settings::getWiringPiPinWorkRelay0() const
{
	return getInt32(wiringPiPinWorkRelay0Key, -1);
}

int32_t Settings::getWiringPiPinCheckRelay0() const
{
	return getInt32(wiringPiPinCheckRelay0Key, -1);
}

int32_t Settings::getWiringPiPinWorkRelay1() const
{
	return getInt32(wiringPiPinWorkRelay1Key, -1);
}

int32_t Settings::getWiringPiPinCheckRelay1() const
{
	return getInt32(wiringPiPinCheckRelay1Key, -1);
}

int32_t Settings::getWiringPiPinNumberVideoCaptureFromLowToHigh() const
{
	return getInt32(wiringPiPinNumberVideoCaptureFromLowToHighKey, -1);
}

int32_t Settings::getWiringPiPinNumberVideoCaptureFromHighToLow() const
{
	return getInt32(wiringPiPinNumberVideoCaptureFromHighToLowKey, -1);
}

std::string Settings::getWiringPiEmptyPlateVideoCapture() const
{
	return m_propertyTree.get<std::string>(wiringPiEmptyPlateVideoCaptureKey);
}

int32_t Settings::getWiringPiVideoCaptureDelaySec() const
{
	return getInt32(wiringPiVideoCaptureDelaySecKey, 0);
}

std::string Settings::getHostAddress(const std::string& key) const
{
	std::string res = m_propertyTree.get<std::string>(key);
	std::string::size_type index = res.find(":", 0);
	if(index == std::string::npos)
	{
		return std::string();
	}
	res = res.substr(0, index);
	return res;
}

uint16_t Settings::getPort(const std::string& key) const
{
	std::string res = m_propertyTree.get<std::string>(key);
	std::string::size_type index = res.find(":", 0);
	if(index == std::string::npos)
	{
		return 0;
	}
	res = res.substr(index + 1, res.length() - index - 1);
	return std::stoi(res);
}

int32_t Settings::getInt32(const std::string& key, int32_t defaultValue) const
{
	return m_propertyTree.get<int32_t>(key, defaultValue);
}
