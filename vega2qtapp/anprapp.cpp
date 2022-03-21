#include "core.h" //Fix winsock error

#include "anprapp.h"

#include <QtCore/QTimer>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

ANPRApp::ANPRApp(Core* core, QWidget* parent, Qt::WFlags flags) :
    QMainWindow(parent, flags), PlateReadyVisitor(), m_core(core),
	m_repaintImageFlag(false)
{
	ui.setupUi(this);
	m_timer = new QTimer(this);
	m_timer->start(0);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

ANPRApp::~ANPRApp()
{

}

void ANPRApp::plateReady(const std::string& /*plateNumber*/, const std::string& message)
{
	Q_ASSERT(m_core);
	QListWidgetItem * item = new QListWidgetItem(QString::fromStdString(message), ui.listWidget);
	ui.listWidget->addItem(item);
	ui.listWidget->scrollToItem(item);
}

void ANPRApp::imageReady()
{
    m_repaintImageFlag = true;
}

void ANPRApp::onTimeout()
{
	if(m_repaintImageFlag)
	{
		m_repaintImageFlag = false;
		Q_ASSERT(m_core);
		ui.imageWidget->setData(m_core->getImage(), m_core->getSettingsRect());
	}
}

void ANPRApp::openFileAction_triggered()
{
	Q_ASSERT(m_core);
	m_core->openFile(QFileDialog::getOpenFileName().toStdString());
}

void ANPRApp::openStreamAction_triggered()
{
	Q_ASSERT(m_core);
	if(!m_core->startCaptureStream())
	{
		QMessageBox::warning(0, "CarNumApp", "Error opening video stream");
	}
}

void ANPRApp::openWebCamAction_triggered()
{
	Q_ASSERT(m_core);
	if(!m_core->startCaptureWebCam())
	{
		QMessageBox::warning(0, "CarNumApp", "Error opening video webcam");
	}
}
