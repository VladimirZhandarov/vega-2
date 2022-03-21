#ifndef VEGACOMMAND26001_RESPONSE_H
#define VEGACOMMAND26001_RESPONSE_H

#include "vegacommand.h"

#include <vector>

class VegaCommand26001_ReadFile_Response : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand26001_ReadFile_Response();

	//TODO comment
	VegaCommand26001_ReadFile_Response(const std::vector<unsigned char>& data);

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand26001_ReadFile_Response(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand26001_ReadFile_Response();
};

#endif // VEGACOMMAND26001_RESPONSE_H
