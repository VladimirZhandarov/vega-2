#include "opencv2/highgui/highgui_c.h"
#include "../../include/iANPRcapture.h"
#include <stdio.h>


void printHelp (char* fullName)
{
	printf  ("Use: %s <type_number> <path to video file>\n\n", fullName);	
	puts ("type_number: 7 for Russian, 104 for Kazakhstan, 203 for Turkmenistan, 300...... for Belarus vehicle registration plates");
	puts ("For more type_numbers please refer to iANPR SDK documentation\n");
	printf ("Example: %s 7 C:\\test.avi - recognition of russian vehicle registration plates from file test.avi\n", fullName);
	printf ("Example: %s 104 - recognition of kazakh vehicle registration plates from webcam\n", fullName);	
}


int main(int argc, char** argv)
{	
	CvCapture* capture = 0;
	
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
	else if (strcmp (argv [1], "0") && strcmp (argv [1], "1") && strcmp (argv [1], "2") && strcmp (argv [1], "3") &&
		strcmp (argv [1], "4") && strcmp (argv [1], "5") && strcmp (argv [1], "6") && strcmp (argv [1], "7") &&
		strcmp (argv [1], "100") && strcmp (argv [1], "101") && strcmp (argv [1], "102") && strcmp (argv [1], "103") &&
		strcmp (argv [1], "104") && strcmp (argv [1], "201") && strcmp (argv [1], "202") && strcmp (argv [1], "203") &&
		strcmp (argv [1], "300") && strcmp (argv [1], "301") && strcmp (argv [1], "302"))
	{
		printf  ("type_number is incorrect. Please refer to program help %s /? or to iANPR SDK documentation\n", argv [0]);
		return -2;
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

	IplImage* grayFrame = 0;
	IplImage* image = cvCreateImage( cvSize( 800, 600 ), 8, 3 );
	int i = 0;	

	// Вызов LicenseCapture необходим только для платных версий
	// И только один раз, перед первым распознаванием. 
	char key[1481] = { 0 };
	FILE* f = fopen( "lic.key", "rb" );
	if ( f != NULL )
	{
		fread( (void*)key, 1480, 1, f );
		fclose( f );
	}
	else 
		puts ("WARNING! File lic.key not found. This may crash program if you use license version of iANPR SDK dlls");
	LicenseCapture( key );

	CvPoint Lines[4] = {0};
	for(;;)
    {		
		IplImage* frame = 0;    
		frame = cvQueryFrame(capture );
		if( !frame )
            break;

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
		}		
		cvCvtColor( frame, grayFrame, CV_BGR2GRAY );		

		int i1 = 0;
		CvRect Rects[100];
		int all = 100;
		char** res = new char*[all];
		for(int j=0;j<all;j++) res[j] = new char[20];

		if (a.type_number == 4 || a.type_number == 7)
			i1 = AddFrameToiANPRCapture( i_capture, frame, &all, Rects, res );
		else 
			i1 = AddFrameToiANPRCapture( i_capture, grayFrame, &all, Rects, res );

		printf( "Ret:%d; num:%d; cand:%d  ", i1, i, all );
		if ( i1 == 0 )
		{						
			for(int j = 0; j < all; j++ )				
			{				
				if ( strlen( res[j] ) >= 1 )
				{					
					cvRectangle( frame,cvPoint( Rects[j].x, Rects[j].y),cvPoint(Rects[j].x+Rects[j].width,
						Rects[j].y+Rects[j].height), CV_RGB(255,255,0), 2);
					
					CvFont font;
					float aa=0.001f*frame->width;
					cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, aa,
									aa,0,1, 8 ); 
					CvPoint pp2,pp1;
					pp2.x=Rects[j].x;
					pp2.y=Rects[j].y;
					pp1.x=Rects[j].x+1;
					pp1.y=Rects[j].y+1;
					cvPutText( frame, res[j], pp1, &font, CV_RGB(0,0,0) );
					cvPutText( frame, res[j], pp2, &font, CV_RGB(0,255,0) );
					printf( "%s;", res[j] );
				}			
			}
		}//if ( i1 == 0 )			
		printf( "\n" );
		all = 100;
		CvPoint Points[1000];
		int all_points = 1000;
		GetNumbersInMemory( i_capture, &all, res , 20, Points, &all_points );
		if ( all > 0 )
		{
			for(int j = 0; j < all; j++ )
			{
				printf( "(%s)", res[j] );
			}
			printf( "\n" );
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

			CvPoint pp2,pp1;
			pp2.x=0;
			pp2.y=40;
			pp1.x=1;
			pp1.y=41;
			cvPutText( frame, res[0], pp1, &font, CV_RGB(0,0,0) );
			cvPutText( frame, res[0], pp2, &font, CV_RGB(0,255,0) );
			cvResize( frame, image );
			cvShowImage( "frame", image);	
			for(int j = 0; j < 60; j++ )
				cvWriteFrame( cvVideoWriter, frame );
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

	return 0;
}
