#include <opencv2/highgui/highgui.hpp>
#include "../../include/iANPRinterface.h"
#include "../../include/iANPR.h"
#include <stdio.h>

// Note: debug mode must build only with debug opencv libraries, i.e. opencv_core2411d.lib
// or opencv's Mat will not work

void Memory( char* name, int typeNumber )
{
	// ?????? ?????
	FILE* f = fopen( name, "rb" );
	if ( f == NULL ) 
	{
		puts ("Can't load file");
		return;
	}

	long size;

	fseek( f, 0, SEEK_END );
	size = ftell( f );
	fseek( f, 0, SEEK_SET);

	char* buffer = new char[size];	
	fread( buffer, size, 1, f );

	fclose(f);

	// ????? ??????? ?????????????
	CvRect Rects[100];
	int all = 100;
	char** res = new char*[all];
	for(int j=0;j<all;j++) res[j] = new char[20];
	ANPR_OPTIONS a;
	a.Detect_Mode = ANPR_DETECTCOMPLEXMODE;
	a.min_plate_size = 500;
	a.max_plate_size = 25000;
	a.max_text_size = 20;		
	a.type_number = typeNumber;
	a.flags = 0;

	int i = anprPlateMemory( buffer, size,  a, &all, Rects, res ); 
	if ( i == 0 )
		for( int j = 0; j < all; j++ ) {				
			printf( "Mode:Mem; Num:%s\n", res[j] );
	}

	for(int j=0;j<100;j++) delete [] res[j];
	delete [] res;

	delete [] buffer;
}

void WithMat( char* name, int typeNumber )
{
	cv::Mat mat;
	if (typeNumber == 4 || typeNumber == 7)
		mat = cv::imread( name, CV_LOAD_IMAGE_COLOR );
	else
		mat = cv::imread( name, CV_LOAD_IMAGE_GRAYSCALE );

	if ( !mat.data ) 
	{
		puts ("Can't load file to Mat");
		return;
	}

	// ????? ??????? ?????????????
	CvRect Rects[100];
	int all = 100;
	char** res = new char*[all];
	for(int j=0;j<all;j++) res[j] = new char[20];
	ANPR_OPTIONS a;
	a.Detect_Mode = ANPR_DETECTCOMPLEXMODE;
	a.min_plate_size = 500;
	a.max_plate_size = 25000;
	a.max_text_size = 20;		
	a.type_number = typeNumber;
	a.flags = 0;
	int i = anprPlateMatRect( mat,  cv::Rect( 0, 0, mat.cols, mat.rows ), a, &all, Rects, res ); 
	if ( i == 0 )
		for( int j = 0; j < all; j++ ) {				
			printf( "Mode:Mat; Num:%s\n", res[j] );
	}

	for(int j=0;j<100;j++) delete [] res[j];
	delete [] res;
}

void XMLWork( char* name, int typeNumber )
{
	IplImage* Img = 0;
	if (typeNumber == 4 || typeNumber == 7)
		Img = cvLoadImage( name, CV_LOAD_IMAGE_COLOR  );
	else
		Img = cvLoadImage( name, CV_LOAD_IMAGE_GRAYSCALE  );

	if ( Img == NULL )
	{
		printf( "Can't load file!");		
		return;
	}

	ANPR_OPTIONS a;
	a.Detect_Mode = ANPR_DETECTCOMPLEXMODE;
	a.min_plate_size = 500;
	a.max_plate_size = 25000;
	a.max_text_size = 20;		
	a.type_number = typeNumber;
	a.flags = 0;
	char xml[1000];
	int size = 1000;
	CvRect Rect = cvRect( 0, 0, Img->width, Img->height );

	int i = anprPlateRectXML( Img,  Rect, a, xml, &size ); 			
	if ( i == 0 )
		printf( "%s\n", xml );

	cvReleaseImage ( &Img );
}

void XMLWork2( char* name, int typeNumber )
{
	cv::Mat mat;
	if (typeNumber == 4 || typeNumber == 7)
		mat = cv::imread( name, CV_LOAD_IMAGE_COLOR );
	else
		mat = cv::imread( name, CV_LOAD_IMAGE_GRAYSCALE );
	
	if ( !mat.data ) 
	{
		puts ("Can't load file to Mat");
		return;
	}

	// ????? ??????? ?????????????	
	ANPR_OPTIONS a;
	a.Detect_Mode = ANPR_DETECTCOMPLEXMODE;
	a.min_plate_size = 500;
	a.max_plate_size = 25000;
	a.max_text_size = 20;		
	a.type_number = typeNumber;
	a.flags = 0;
	char xml[1000];
	int size = 1000;
	int i = anprPlateMatRectXML( mat,  cv::Rect( 0, 0, mat.cols, mat.rows ), a, xml, &size ); 
	if ( i == 0 )
		printf( "%s\n", xml );	
}


void printHelp (char* fullName)
{
	printf  ("Use: %s <type_number> <path to image> <mode>\n\n", fullName);	
	puts ("type_number: 7 for Russian, 104 for Kazakhstan, 203 for Turkmenistan, 300 for Belarus vehicle registration plates");
	puts ("For more type_numbers please refer to iANPR SDK documentation\n");
	printf ("Example: %s 7 C:\\test.jpg 1 - recognition of russian vehicle registration plates from file test.jpg with Mat\n", fullName);	
}


/*
	?????? ??????:
	image_new <type_number> <file> <mode>
*/
	
int main( int argc, char** argv)
{
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
	else if (argc < 4)
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

	// ????? LicenseValue ????????? ?????? ??? ??????? ??????
	// ? ?????? ???? ???, ????? ?????? ??????????????. 
	char key [1481] = {0};
	FILE* f = fopen( "lic.key", "rb" );
	if ( f != NULL )
	{
		fread( (void*) key, 1480, 1, f );
		fclose( f );
	}
	else
		puts ("WARNING! File lic.key not found. This may crash program if you use license version of iANPR SDK dlls");
	LicenseValue( key );

	int mode = atoi( argv[3] );
	int typeNumber = atoi( argv[1] );
	switch ( mode )
	{
		// anprPlateMemory - ?????? ?? ??????
	case 0:
		Memory( argv[2], typeNumber );
		break;
		// anprPlateMat - ? ??????? cpp ?????????? OpenCV
	case 1:
		WithMat( argv[2], typeNumber );
		break;
		// XML anprPlateRectXML
	case 2:
		XMLWork( argv[2], typeNumber );
		break;
		// XML 
	case 3:
		XMLWork2( argv[2], typeNumber );
		break;
	}	

	return 0;
}