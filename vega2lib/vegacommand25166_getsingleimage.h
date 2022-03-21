#ifndef VEGACOMMAND25166_H
#define VEGACOMMAND25166_H

#include "vegacommand.h"

class VegaCommand25166_GetSingleImage : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand25166_GetSingleImage();

	//TODO comment
	VegaCommand25166_GetSingleImage(uint32_t jpegQuality, uint32_t imageWidth, uint32_t imageHeight);

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand25166_GetSingleImage(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand25166_GetSingleImage();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND25166_H
