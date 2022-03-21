#ifndef ANPRAPP_H
#define ANPRAPP_H

#include <QtGui/QMainWindow>
#include "ui_anprapp.h"
#include <platereadyvisitor.h>

class Core;

class QTimer;

class ANPRApp : public QMainWindow, public PlateReadyVisitor
{
	Q_OBJECT
public:
    ANPRApp(Core* core,  QWidget* parent = 0, Qt::WFlags flags = 0);
	~ANPRApp();

	virtual void plateReady(const std::string& plateNumber, const std::string& message);
    virtual void imageReady();

protected slots:
	void onTimeout();
	void openFileAction_triggered();
	void openStreamAction_triggered();
	void openWebCamAction_triggered();

private:
	Ui::ANPRApp ui;

	Core* m_core;
	QTimer* m_timer;
	bool m_repaintImageFlag;
};

#endif // ANPRAPP_H
