#ifndef VEGACOMMAND25166_RESPONSE_H
#define VEGACOMMAND25166_RESPONSE_H

#include "vegacommand.h"

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

class VegaCommand25166_GetSingleImage_Response : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand25166_GetSingleImage_Response();

	//TODO comment
	VegaCommand25166_GetSingleImage_Response(std::time_t imageDateTime, uint32_t jpegQuality, const cv::Mat& image);

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand25166_GetSingleImage_Response(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand25166_GetSingleImage_Response();
};

#endif // VEGACOMMAND25166_RESPONSE_H
