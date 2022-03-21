#ifndef VEGACOMMAND40000_H
#define VEGACOMMAND40000_H

#include "vegacommand.h"

#include <ctime>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

class VegaTag;

/**
 * Сообщение с номером от сервера клиенту
 */
class VegaCommand40000_DataMessage : public VegaCommand
{
public:
	enum DirectionType
	{
		NoDirectionType = -1,
		GoAwayDirectionType = 0,
		AproachDirectionType,
		UnknowDirectionType
	};

	/**
	 * Конструктор
	 */
	VegaCommand40000_DataMessage();

	/**
	 * Конструктор
	 * @param plate номер
	 * @param country код страны
	 * @param image черно-белое избражение
	 */
	VegaCommand40000_DataMessage(const std::time_t& messageDateTime, const std::string& plate, const std::string& country, DirectionType directionType,
		const cv::Mat& image, const CvRect& plateRect = CvRect(), const cv::Mat& plateImage = cv::Mat());

	VegaCommand40000_DataMessage(const std::time_t& messageDateTime, const std::time_t& recognizeDateTime, const std::string& plate,
		const std::string& country, DirectionType directionType, const std::vector<unsigned char>& imageData, const CvRect& plateRect,
		const std::vector<unsigned char>& plateImageData);

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand40000_DataMessage(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand40000_DataMessage();

	const std::string& getPlate() const;
	const std::string& getCountry() const;
	DirectionType getDirectionType() const;
	const CvRect& getPlateRect() const;
	VegaTag* getImageVegaTag() const;
	VegaTag* getPlateImageVegaTag() const;

	const std::time_t& getMessageDateTime() const;
	const std::time_t& getRecognizeDateTime() const;

	int getPlateImageStatus() const;
	bool isFromStore() const;

private:
	void init(const std::string& dateString, const std::string& timeString, const std::string& plate, const std::string& country, DirectionType directionType,
		const CvRect& plateRect);

	std::string m_plate;
	std::string m_country;
	DirectionType m_directionType;
	CvRect m_plateRect;

	VegaTag* m_imageVegaTag;
	VegaTag* m_plateImageVegaTag;

	std::time_t m_messageDateTime;
	std::time_t m_recognizeDateTime;
	int m_plateImageStatus;
	bool m_fromStore;
};

#endif // VEGACOMMAND40000_H
