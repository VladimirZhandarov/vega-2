#include "imagewidget.h"

ImageWidget::ImageWidget(QWidget* parent) : QGLWidget(parent), m_mirrorX(false),
    m_mirrorY(false), m_scaleDiff(IOGL_XY_IDENT), m_keepAspectRatio(true)
{

}

ImageWidget::~ImageWidget()
{

}

void ImageWidget::setData(const cv::Mat& mat, const CvRect& rect)
{
	m_mat = mat;
	m_image = toQImage(m_mat, rect);
	resizeGL(size().width(), size().height());
	paintGL();
}

void ImageWidget::initializeGL()
{
	makeCurrent();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDisable(
		GL_SCISSOR_TEST
		| GL_ALPHA_TEST
		| GL_STENCIL_TEST
		| GL_DEPTH_TEST
		| GL_BLEND
		| GL_DITHER
		| GL_COLOR_LOGIC_OP);
	glClearColor(0.0,0.,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	setAutoBufferSwap(false);
}

void ImageWidget::paintGL()
{
	if(!m_image.isNull())
	{
		makeCurrent();
		switch(m_image.depth())
		{
			case 32: glDrawPixels(m_image.bytesPerLine() / 4, m_image.height(), GL_RGBA, GL_UNSIGNED_BYTE, ((GLvoid*)(m_image.bits()))); break;
			case 24: glDrawPixels(m_image.bytesPerLine() / 3, m_image.height(), GL_RGB, GL_UNSIGNED_BYTE, ((GLvoid*)(m_image.bits()))); break;
			case 8: glDrawPixels(m_image.bytesPerLine(), m_image.height(), GL_LUMINANCE, GL_UNSIGNED_BYTE, ((GLvoid*)(m_image.bits()))); break;
		}
	}
	swapBuffers();
}

void ImageWidget::resizeGL(int xx,int yy)
{
	makeCurrent();

	m_screenX = m_sizeX = xx;
	m_screenY = m_sizeY = yy;
	GLint deltaX = 0;
	GLint deltaY = 0;
	int picX = 1;
	int picY = 1;
	if(!m_image.isNull())
	{
		picX = m_image.width();
		picY = m_image.height();
	}
	double ax = ((double)m_sizeX) / ((double)picX);
	double ay = ((double)m_sizeY) / ((double)picY);
	m_scaleDiff = IOGL_XY_IDENT;
	if((m_keepAspectRatio)&&(ax!=ay))
	{
		if(ax > ay)
		{
			ax = ay;
			m_screenX = (((double)picX) * ax);
			deltaX = (m_sizeX - m_screenX) / 2;
			//deltaX = (GLint)(fabs(((((double)size_X) - (((double)PIC_X) * ax)) / 2.)));
			//screenX = screenY;
			m_scaleDiff = IOGL_Xy;
		}
		else
		{
			ay = ax;
			m_screenY = (((double)picY) * ay);
			deltaY = (m_sizeY - m_screenY) / 2;
			//deltaY = (GLint)(fabs(((((double)size_Y) - (((double)PIC_Y) * ay)) / 2.)));
			//screenY = screenX;
			m_scaleDiff = IOGL_xY;
		}
		glClear(GL_COLOR_BUFFER_BIT);
	}
	GLint coordX;
	GLint coordY;
	double scx;
	double scy;
	if(m_mirrorX & m_mirrorY)
	{
		coordX = ((GLint)(m_screenX-1));
		//coordX = ((GLint)(m_screenX));
		coordY = ((GLint)0);
		scx = -ax;
		scy = ay;
	}
	else if(m_mirrorX)
	{
		coordX = ((GLint)(m_screenX-1));
		coordY = ((GLint)(m_screenY-1));
		//coordX = ((GLint)(m_screenX));
		//coordY = ((GLint)(m_screenY));
		scx = -ax;
		scy = -ay;
	}
	else if(m_mirrorY)
	{
		coordX = ((GLint)0);
		coordY = ((GLint)0);
		scx = ax;
		scy = ay;
	}
	else
	{
		coordX = ((GLint)0);
		coordY = ((GLint)(m_screenY-1));
		//coordY = ((GLint)(m_screenY));
		scx = ax;
		scy = -ay;
	}
	m_scaleX = ax;
	m_scaleY = ay;

	glViewport((GLsizei)deltaX, (GLsizei)deltaY, (GLsizei)m_screenX, (GLsizei)m_screenY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLfloat)m_screenX, 0.0, (GLfloat)m_screenY, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(coordX, coordY);
	glPixelZoom((GLfloat)scx, (GLfloat)scy);
}

QImage ImageWidget::toQImage(cv::Mat& mat, const CvRect& rect)
{
	if(mat.empty())
	{
		return QImage();
	}
	if(rect.width > 0 && rect.height >0)
	{
		cv::rectangle(mat, rect, cv::Scalar::all(255), 3);
	}
	switch(mat.type())
	{
		case CV_8UC4:
		{
			return QImage(mat.data, mat.cols, mat.rows, (int)mat.step, QImage::Format_ARGB32);
		}
		case CV_8UC3:
		{
			QImage image(mat.data, mat.cols, mat.rows, (int)mat.step, QImage::Format_RGB888);
			return image.rgbSwapped();
		}
		case CV_8UC1:
		{
			static QVector<QRgb> sColorTable;
			if(sColorTable.isEmpty())
			{
				for(int i = 0; i < 256; i++)
				{
					sColorTable.push_back(qRgb(i, i, i));
				}
			}
			QImage image(mat.data, mat.cols, mat.rows, (int)mat.step, QImage::Format_Indexed8);
			image.setColorTable(sColorTable);
			return image;
		}
	};
	return QImage();
}
