#include <QtGui/QApplication>

#include <QtCore/QTextCodec>

#include "anprapp.h"

#include <core.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	Core* core = new Core();
	ANPRApp* win = new ANPRApp(core);
	core->setPlateReadyVisitor(win);
	win->show();
	int exitCode = a.exec();
	core->setPlateReadyVisitor(0);
	delete win;
	delete core;
	return exitCode;
}
