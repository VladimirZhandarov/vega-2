#include "opencv2/highgui/highgui_c.h"
#include "../../include/iANPRcapture.h"
#include <stdio.h>
#include <ctime>
#include <list>
#include <limits>
#include <ratio>
#include <chrono>


struct Plate
{	
	std::string str;
	std::string timeStamp;

	IplImage* img;
	CvRect rect;	
	
	CvPoint plateCenter;
	int distToCenterY;

	Plate() : str(""), timeStamp(""), img(0), rect(cvRect (0,0,0,0)), plateCenter (cvPoint (0,0)), distToCenterY(INT_MAX) {}
};

void printHelp (char* fullName)
{
	printf  ("Use: %s <type_number> <path to video file> <OPTIONAL listSize> \n\n", fullName);	
	puts ("type_number: 7 for Russian, 104 for Kazakhstan, 203 for Turkmenistan, 300...... for Belarus vehicle registration plates");
	puts ("For more type_numbers please refer to iANPR SDK documentation\n");
	printf ("Example: %s 7 C:\\test.avi - recognition of russian vehicle registration plates from file test.avi\n", fullName);
	printf ("Example: %s 104 - recognition of kazakh vehicle registration plates from webcam\n", fullName);
}


std::string getTimeStamp()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[512];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d.%m.%Y; %H:%M:%S", timeinfo);
	std::string str(buffer);

	return str;
}

int drawPlate(IplImage* img, CvRect rect, const char* str)
{
	cvRectangle(img, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width,
		rect.y + rect.height), CV_RGB(255, 255, 0), 2);

	CvFont font;
	float aa = 0.001f*img->width;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, aa,
		aa, 0, 1, 8);
	CvPoint pp2, pp1;
	pp2.x = rect.x;
	pp2.y = rect.y;
	pp1.x = rect.x + 1;
	pp1.y = rect.y + 1;
	cvPutText(img, str, pp1, &font, CV_RGB(0, 0, 0));
	cvPutText(img, str, pp2, &font, CV_RGB(0, 255, 0));

	return 0;
}


bool isInTrajectory(CvPoint* Points, CvPoint pt, int all_points)
{
	for (int j = 0; j < all_points; j++)
		if (Points[j].x == pt.x && Points[j].y == pt.y)
			return true;

	return false;
}


int main(int argc, char** argv)
{	
	using namespace std::chrono;
	CvCapture* capture = 0;
	
	size_t listSize = 50;	
	int controLinesCenterY = -1;
	int minDiffToControlLinesCenter = -1;
	
	// filter input
	if (argc < 2)
	{
		printf ("Too few arguments. For help print %s /?", argv [0]);
		return -1;
	}
	else if (!strcmp (argv [1], "help") || !strcmp (argv [1], "-help") || !strcmp (argv [1], "--help") || !strcmp (argv [1], "/?"))
	{
		printHelp (argv [0]);
		return 0;
	}
	else if (argc == 2)		
		capture = cvCaptureFromCAM( 0 );
	else
		capture = cvCaptureFromFile(argv[2]); 

	if (!capture)
	{
		puts ("Can't load file or camera");
		return -100;
	}

	if (argc > 3)
		listSize = atoi(argv[3]);

	std::list <Plate> plates;
	std::list <Plate>::iterator* optimalPlates = new std::list <Plate>::iterator[listSize];
	
	char buffer[256];
	sprintf( buffer, "out.avi" );
	CvVideoWriter* cvVideoWriter = 0; 		
	ANPR_OPTIONS a;
	a.Detect_Mode = ANPR_DETECTCOMPLEXMODE;
	a.min_plate_size = 500;
	a.max_plate_size = 50000;
	a.max_text_size = 20;		
	a.type_number = atoi (argv [1]);
	iANPRCapture i_capture;

	bool isFullType = false;
	for (size_t i = 0; i < anprFullTypesCount; i++)
	if (anprFullTypes[i] == a.type_number)
		isFullType = true;

	IplImage* grayFrame = 0;
	IplImage* image = cvCreateImage( cvSize( 800, 600 ), 8, 3 );	
	int i = 0;	

	// Вызов LicenseCapture необходим только для платных версий
	// И только один раз, перед первым распознаванием.	
	char* key = new char[8001]; memset(key, 0, 8001);
	FILE* f = fopen("lic.key", "rb");
	if (f != NULL)
	{
		fread((void*)key, 8000, 1, f);
		fclose(f);
	}
	else
		puts("WARNING! File lic.key not found. This may crash program if you use license version of iANPR SDK dlls");

	LicenseCapture(key);
	delete [] key; key = 0;

	CvPoint Lines[4] = {0};
	for(;;)
    {		
		IplImage* frame = 0;    		
		frame = cvQueryFrame(capture );
		if( !frame )
            break;

		while (plates.size() > listSize - 1) // Удаляем устаревший кадр
		{
			if (plates.front().img != 0)
				cvReleaseImage(&plates.front().img);
			plates.pop_front();
		}

		if (!grayFrame)
		{
			grayFrame = cvCreateImage(cvGetSize(frame), 8, 1);
			cvZero( grayFrame );
            grayFrame->origin = frame->origin;			
			i_capture = CreateiANPRCapture( 20, a, cvRect( 0, 0, frame->width, frame->height ) );			
			CreateMemoryForiANPRCapture( i_capture, 10, 15, 100 );
			/*Lines[0].x = int( frame->width * 0.1f ); Lines[0].y = int( frame->height * 0.5f );
			Lines[1].x = int( frame->width * 0.3f ); Lines[1].y = int( frame->height * 0.8f );			
			Lines[2].x = int( frame->width * 0.1f ); Lines[2].y = int( frame->height * 0.6f );
			Lines[3].x = int( frame->width * 0.3f ); Lines[3].y = int( frame->height * 0.9f );*/
			Lines[0].x = int( frame->width * 0.1f ); Lines[0].y = int( frame->height * 0.6f );
			Lines[1].x = int( frame->width * 0.3f ); Lines[1].y = int( frame->height * 0.6f );			
			Lines[2].x = int( frame->width * 0.1f ); Lines[2].y = int( frame->height * 0.7f );
			Lines[3].x = int( frame->width * 0.3f ); Lines[3].y = int( frame->height * 0.7f );
			CreateLineIntersection( i_capture, Lines[0], Lines[1], Lines[2], Lines[3] );			
			controLinesCenterY = (Lines[0].y + Lines[1].y + Lines[2].y + Lines[3].y) / 4;			
		}		
		cvCvtColor( frame, grayFrame, CV_BGR2GRAY );		

		high_resolution_clock::time_point tick1 = high_resolution_clock::now();
		
		int i1 = 0;
		CvRect Rects[100];
		int all = 100;
		char** res = new char*[all];
		for(int j=0;j<all;j++) res[j] = new char[20];

		if (isFullType)
			i1 = AddFrameToiANPRCapture( i_capture, frame, &all, Rects, res );
		else 
			i1 = AddFrameToiANPRCapture( i_capture, grayFrame, &all, Rects, res );

		high_resolution_clock::time_point tick2 = high_resolution_clock::now();

		duration <double> dur = duration_cast <milliseconds> (tick2-tick1);
		
		printf( "Ret:%d; num:%d; time:%5.3f sec; cand:%d  ", i1, i, (float) dur.count (), all );
		
		if (i1 == 0)
		{
			for (int j = 0; j < all; j++)
			{
				if (strlen(res[j]) >= 1)
				{
					printf("%s;", res[j]);

					int plateCenterY = Rects[j].y + Rects[j].height / 2; // Добавляем результат успешного распознавания в список

					Plate tmpPlate;
					tmpPlate.str = res[j];
					tmpPlate.timeStamp = getTimeStamp();

					tmpPlate.rect = Rects[j];
					tmpPlate.distToCenterY = abs(plateCenterY - controLinesCenterY);
					tmpPlate.plateCenter = cvPoint(Rects[j].x + Rects[j].width / 2, Rects[j].y + Rects[j].height / 2);

					tmpPlate.img = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
					cvCopy(frame, tmpPlate.img);

					plates.push_back(tmpPlate);

					drawPlate(frame, Rects[j], res[j]);
				}
			}
		}//if ( i1 == 0 )		
		else // "Сдвигаем" список 
			plates.push_back(Plate());

		printf( "\n" );
		all = 100;
		CvPoint Points[1000];
		int all_points = 1000;
		GetNumbersInMemory( i_capture, &all, res , 20, Points, &all_points );

		if ( all > 0 )
		{
			for (int j = 0; j < all; j++)
			{				
				optimalPlates [j] = plates.end();

				for (std::list <Plate>::iterator l = plates.begin(); l != plates.end(); l++)
					if (optimalPlates[j] == plates.end() || (l->str == res[j] && l->distToCenterY < optimalPlates[j]->distToCenterY && isInTrajectory(Points, l->plateCenter, all_points)))					
						optimalPlates[j] = l;				

				if (optimalPlates[j]->str != res[j])
					printf("listSize is too small. Output is incorrect.\n");
			}

			for(int j = 0; j < all; j++ )
				printf("(%s: %s)\n", optimalPlates[j]->str.c_str(), optimalPlates[j]->timeStamp.c_str());
		}		

		if ( Lines[0].y != 0 )
		{	
			cvLine( frame, Lines[0], Lines[1], CV_RGB(0,255,255), 2 );
			cvLine( frame, Lines[2], Lines[3], CV_RGB(0,255,255), 2 );
		}

		cvResize( frame, image );
		cvShowImage( "frame", image);		

		if (!cvVideoWriter)
		{
				cvVideoWriter=cvCreateVideoWriter( buffer, CV_FOURCC('D', 'I', 'V', '3'), 20,
						cvGetSize( frame) );
		}	
		cvWriteFrame( cvVideoWriter, frame );

		// Рисование траектории и вывод дополнительного окна
		if ( all > 0 && all_points > 1)
		{
			for(int j = 0; j < all_points; j++ )
			{
				cvCircle( frame, Points[j], 5, CV_RGB(0,0,255), 3 );
				if ( j > 0 ) cvLine( frame, Points[j], Points[j-1], CV_RGB(0,0,255), 2 );

				cvCircle(optimalPlates[0]->img, Points[j], 5, CV_RGB(0, 0, 255), 3);
				if (j > 0) cvLine(optimalPlates[0]->img, Points[j], Points[j - 1], CV_RGB(0, 0, 255), 2);
			}

			CvFont font;
			float aa=0.001f*frame->width;
			cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, aa,
							aa,0,1, 8 ); 
			CvSize size;
			int b;
			cvGetTextSize( res[0], &font, &size, &b );
			cvRectangle( frame, cvPoint(0, 0 ), cvPoint( size.width + 2, 
				50 ), CV_RGB( 255, 255, 255 ), CV_FILLED );
			
			cvRectangle(optimalPlates [0]->img, cvPoint(0, 0), cvPoint(size.width + 2,
				50), CV_RGB(255, 255, 255), CV_FILLED);

			CvPoint pp2,pp1;
			pp2.x=0;
			pp2.y=40;
			pp1.x=1;
			pp1.y=41;
			cvPutText( frame, res[0], pp1, &font, CV_RGB(0,0,0) );
			cvPutText( frame, res[0], pp2, &font, CV_RGB(0,255,0) );

			cvPutText(optimalPlates[0]->img, res[0], pp1, &font, CV_RGB(0, 0, 0));
			cvPutText(optimalPlates[0]->img, res[0], pp2, &font, CV_RGB(0, 255, 0));

			cvResize( frame, image );
			cvShowImage( "frame", image);	
			for(int j = 0; j < 60; j++ )
				cvWriteFrame( cvVideoWriter, frame );

			for (int j = 0; j < all; j++)
			{
				drawPlate(optimalPlates[j]->img, optimalPlates[j]->rect, optimalPlates[j]->str.c_str());

				/*char buf[512];
				sprintf(buf, "%s:%s_intersectFrame.jpg");				
				cvSaveImage(buf, optimalPlates[j]->img);*/

				cvSaveImage("intersectFrame.jpg", optimalPlates[j]->img);
			}

			// Удаляем отработавшие номера из списка
			for (int j = 0; j < 1/*all*/; j++)
			{
				std::list <Plate>::iterator l = plates.begin();
				while (l != plates.end())
				{
					if (l->str == res[j] && isInTrajectory(Points, l->plateCenter, all_points))
					{
						if (l->img != 0)
							cvReleaseImage(&l->img);

						l = plates.erase(l);
					}						
					else
						l++;
				}					
			}
		}				

		for(int j=0;j<100;j++) delete [] res[j];
		delete [] res;
		i++;

		int c = cvWaitKey( 20 );
		if ( c == 32 )
			c = cvWaitKey( 0 ); // pause
		if ( c== 27 ) break;
	}

	if ( i_capture != NULL ) ReleaseiANPRCapture( &i_capture );
	cvReleaseCapture( &capture );
	cvReleaseVideoWriter( &cvVideoWriter );

	delete[] optimalPlates;

	for (auto it = plates.begin(); it != plates.end(); it++)
		if (it->img != 0)
			cvReleaseImage(&it->img);

	return 0;
}
