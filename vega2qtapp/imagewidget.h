#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QtOpenGL/QGLWidget>

#include <QtGui/QImage>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

#define IOGL_XY_IDENT	0
#define IOGL_Xy			1
#define IOGL_xY			2

class ImageWidget : public QGLWidget
{
	Q_OBJECT
public:
	ImageWidget(QWidget* parent = 0);
	~ImageWidget();

	void setData(const cv::Mat& mat, const CvRect& rect);

protected:
	QImage toQImage(cv::Mat& mat, const CvRect& rect);

	virtual void initializeGL();
	virtual void paintGL();
	virtual void resizeGL(int, int);

private:
	cv::Mat m_mat;
	QImage m_image;

	bool m_mirrorX; // ïðèçíàê ïåðåâîðîòà ïî X
	bool m_mirrorY; // ïðèçíàê ïåðåâîðîòà ïî Y
	double m_scaleX; // ìàñøòàá ïî X
	double m_scaleY; // ìàñøòàá ïî Y
	uint m_sizeX; // Õ ðàçìåð îêíà â ïèêñåëàõ íà ìîíèòîðå
	uint m_sizeY; // Y ðàçìåð îêíà â ïèêñåëàõ íà ìîíèòîðå

	int m_screenX; // Õ ðàçìåð êàðòèíêè â ïèêñåëàõ â îêíå
	int m_screenY; // Y ðàçìåð êàðòèíêè â ïèêñåëàõ â îêíå

	uint m_scaleDiff; // ïðèçíàê ðàçíûõ ìàñøòàáîâ
	bool m_keepAspectRatio;// ïðèçíàê ïðîïîðöèîíàëüíîãî èçìåíåíèÿ ðàçìåðîâ
};

#endif // IMAGEWIDGET_H
