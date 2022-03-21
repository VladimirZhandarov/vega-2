#include "opencv2/highgui/highgui_c.h"
#include "../../include/iANPR.h"
#include <stdio.h>

void printHelp (char* fullName)
{
	printf  ("Use: %s <type_number> <path to video file>\n\n", fullName);	
	puts ("type_number: 7 for Russian, 104 for Kazakhstan, 203 for Turkmenistan, 300 for Belarus vehicle registration plates");
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
		strcmp (argv [1], "4") && strcmp (argv [1], "5") && strcmp (argv [1], "6") && strcmp (argv [1], "7") && strcmp (argv [1], "8") && strcmp (argv [1], "9") &&
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
	
	/*char buffer[256];
	sprintf( buffer, "out.avi" );
	CvVideoWriter* cvVideoWriter = 0; 		*/
	IplImage* grayFrame = 0;
	IplImage* image = cvCreateImage( cvSize( 800, 600 ), 8, 3 );
	int i = 0;
	char mem[100][20];
	int all_mem = 0;

	// ����� LicenseValue ��������� ������ ��� ������� ������
	// � ������ ���� ���, ����� ������ ��������������. 
	char key [1581] = {0};
	FILE* f = fopen( "lic.key", "rb" );
	if ( f != NULL )
	{
		fread( (void*)key, 1580, 1, f );
		fclose( f );
	}
	else
		puts ("WARNING! File lic.key not found. This may crash program if you use license version of iANPR SDK dlls");
	LicenseValue( key );

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
		}
		cvCvtColor( frame, grayFrame, CV_BGR2GRAY );		

		int all = 100;
		CvRect Rects[100];				
		char** res = new char*[all];
		for(int j=0;j<all;j++) res[j] = new char[20];
		ANPR_OPTIONS a;		
		a.Detect_Mode = ANPR_DETECTCOMPLEXMODE;
		a.min_plate_size = 500;
		a.max_plate_size = 25000;
		a.max_text_size = 20;		
		a.type_number = atoi (argv [1]);				
		a.flags = 0;
		//CvRect arearect = cvRect( 300, 400, 1400, 500);
		CvRect arearect = cvRect( 0, 0, grayFrame->width, grayFrame->height );	
		cvSaveImage( "out.png", grayFrame );
	
		int i1;		

		if (a.type_number == 4 || a.type_number == 7 || a.type_number == 9 )
			i1 = anprPlateRect( frame, arearect, a, &all, Rects, res ); 	
		else
			i1 = anprPlateRect( grayFrame, arearect, a, &all, Rects, res ); 	


		printf( "Ret:%d; num:%d; cand:%d\n", i1, i, all );
		if ( i1 == 0 )
		{						
			for(int j = 0; j < all; j++ )				
			{				
				if ( strlen( res[j] ) >= 1 )
				{
					int k =0;
					for( int j1 = 0; j1 < all_mem; j1++ )
						if ( strcmp( res[j], mem[j1]) == 0 ) k = 1;	// ���� ��������� ��� ������ ���� �����, �� ��������� ��������� ���������
					if ( k == 0 ) continue;
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
				}			
			}
			// ����������� � ������
			for(int j = 0; j < all; j++ )				
			{
				for( i1 = 0; i1 < strlen( res[j] ); i1++ )
					if ( res[j][i1] == '?' ) {
						i1 = -1;
						break;
					}
				if ( i1 == -1 ) continue;
				if ( strlen( res[j] ) >= 1 )
				{
					strcpy( mem[j], res[j] );
				}
			}
			all_mem = all;
		}				
		for(int j=0;j<100;j++) delete [] res[j];
		delete [] res;
		i++;
		cvResize( frame, image );
		cvShowImage( "frame", image);
		/*if (!cvVideoWriter)
		{
				cvVideoWriter=cvCreateVideoWriter( buffer, CV_FOURCC('D', 'I', 'V', '3'), 30,
						cvGetSize( frame) );
		}	
		cvWriteFrame( cvVideoWriter, frame );*/

		int c = cvWaitKey( 20 );
		if ( c== 27 ) break;
	}

	cvReleaseCapture( &capture );
	//cvReleaseVideoWriter( &cvVideoWriter );

	return 0;
}
