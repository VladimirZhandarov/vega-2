#include "opencv2/highgui/highgui_c.h"
#include "../../include/iANPR.h"
#include <stdio.h>


void printHelp (char* fullName)
{
	printf  ("Use: %s <type_number> <path to image>\n\n", fullName);	
	puts ("type_number: 7 for Russian, 104 for Kazakhstan, 203 for Turkmenistan, 300 for Belarus vehicle registration plates");
	puts ("For more type_numbers please refer to iANPR SDK documentation\n");
	printf ("Example: %s 7 C:\\test.jpg - recognition of russian vehicle registration plates from file test.jpg\n", fullName);	
}


int main( int argc, char** argv)
{
	IplImage* Img = 0; 
	IplImage* grayImg = 0;
	

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
	else if (argc < 3)
	{
		printf ("Too few arguments. For help print %s /?", argv [0]);
		return -2;
	}
	else if (strcmp (argv [1], "0") && strcmp (argv [1], "1") && strcmp (argv [1], "2") && strcmp (argv [1], "3") &&
		strcmp (argv [1], "4") && strcmp (argv [1], "5") && strcmp (argv [1], "6") && strcmp (argv [1], "7") &&
		strcmp (argv [1], "100") && strcmp (argv [1], "101") && strcmp (argv [1], "102") && strcmp (argv [1], "103") &&
		strcmp (argv [1], "104") && strcmp (argv [1], "201") && strcmp (argv [1], "202") && strcmp (argv [1], "203") &&
		strcmp (argv [1], "300") && strcmp (argv [1], "301") && strcmp (argv [1], "302"))
	{
		printf  ("type_number is incorrect. Please refer to program help %s /? or to iANPR SDK documentation\n", argv [0]);
		return -3;
	}	
	else // argc >= 3
		Img = cvLoadImage( argv[2], CV_LOAD_IMAGE_COLOR  );

	if (!Img)
	{
		printf( "Can't load file!\n");
		return -4;
	}
	
	CvRect Rects[100];
	int all = 100;
	char** res = new char*[all];
	for(int j=0;j<all;j++) res[j] = new char[20];
	ANPR_OPTIONS a;
	a.Detect_Mode = ANPR_DETECTCOMPLEXMODE;
	a.min_plate_size = 500;
	a.max_plate_size = 50000;
	a.max_text_size = 20;		
	a.type_number = atoi (argv [1]);		
	a.flags = 0;

	// Вызов LicenseValue необходим только для платных версий
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
	LicenseValue( key );

	int i = -9999;
	if (a.type_number == 4 || a.type_number == 7)
		i = anprPlate( Img,  a, &all, Rects, res ); 
	else
	{		
		grayImg = cvCreateImage (cvGetSize (Img), 8, 1);
		cvCvtColor (Img, grayImg, CV_BGR2GRAY);
		i = anprPlate( grayImg,  a, &all, Rects, res ); 
	}
	
	if ( i == 0 )
		for( int j = 0; j < all; j++ ) 
		{				
			printf( "%s\n", res[j] );
		}
	else
		printf( "Error:%d\n", i );

	for(int j=0;j<100;j++) delete [] res[j];
	delete [] res;
	cvReleaseImage ( &Img );
	cvReleaseImage ( &grayImg );

	return 0;
}
