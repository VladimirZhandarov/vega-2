#ifndef VEGABANCOTAG_H
#define VEGABANCOTTAG_H

#include "vegabasetag.h"

#include <stdint.h>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

class VegaBancoTag : public VegaBaseTag
{
public:
	static uint32_t IMAGE;

	/**
	 * Конструктор
	 */
	VegaBancoTag();

	VegaBancoTag(uint32_t jpegQuality, const cv::Mat& image);

	/**
	 * Деструктор
	 */
	virtual ~VegaBancoTag();

	/**
	 * @copydoc VegaBaseTag::getSize()
	 */
	virtual uint32_t getSize() const;

	/**
	 * @copydoc VegaBaseTag::parse()
	 */
	virtual void parse(std::istream& inData);

	/**
	 * @copydoc VegaBaseTag::serialize()
	 */
	virtual void serialize(std::ostream& outData);

private:
	struct Data
	{
		Data()
		{
			reserved1 = 0;
			reserved2 = 0;
			headerSize = 40;
			usefulDataDimension = 0;
			xResolution = 0;
			yResolution = 0;
			imageType = 0;
			bitPerPixel = 0;
			reserved3 = 0;
			imageFormat = 1;
		}

		uint32_t reserved1;
		uint32_t reserved2;
		uint32_t headerSize; ///< 40
		uint32_t usefulDataDimension;
		uint32_t xResolution;
		uint32_t yResolution;
		uint32_t imageType;
		uint32_t bitPerPixel;
		uint32_t reserved3;
		uint32_t imageFormat;	///< 0: Raw image
								///< 1: Jpeg image
	};

	Data m_data;
	std::vector<unsigned char> m_jpegBuffer;
	int32_t m_jpegBufferLen;
};

#endif // VEGABANCOTAG_H
