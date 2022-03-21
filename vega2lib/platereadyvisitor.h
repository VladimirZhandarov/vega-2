#ifndef PLATEREADYVISITOR_H
#define PLATEREADYVISITOR_H

#include "vega2libdef.h"

#include <string>

/**
 * Интерфейс объекта, который должен знать о событиях обрабоки изображения
 */
class VEGA2LIB_EXPORT PlateReadyVisitor
{
public:
	/**
	 * Конструктор
	 */
	PlateReadyVisitor();

	/**
	 * Деструктор
	 */
	~PlateReadyVisitor();

	/**
	 * Готов новый распознанный номер или возникла ошибка
	 * @param plateNumber распознанный номер
	 * @param message текст ошибки
	 */
	virtual void plateReady(const std::string& plateNumber, const std::string& message) = 0;

	/**
	 * Готово новое изображение
	 */
	virtual void imageReady() = 0;
};

#endif // PLATEREADYVISITOR_H
