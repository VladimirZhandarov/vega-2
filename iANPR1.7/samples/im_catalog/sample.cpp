#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include "opencv2/highgui/highgui_c.h"
#include "../../include/iANPR.h"

int main(int argc, char** argv)
{
    DIR *dir = opendir(argv[1]);
    if(dir)
    {
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

		LicenseValue(key);
		delete [] key; key = 0;

	    struct dirent *ent;		
		char buf[256];

		std::vector <std::string> files;
		while((ent = readdir(dir)) != NULL)
		{
			files.push_back (ent->d_name);
		}

	std::sort (files.begin(), files.end());

	for (int fi = 0; fi < files.size();fi++)
	{
		std::string file = files [fi];
	        puts(file.c_str());
			// Recognition
			strcpy(buf, argv[1]);
			strcat (buf, "/");
			strcat(buf,file.c_str());
			IplImage* Img = cvLoadImage( buf ); 
			if ( Img == NULL ) continue;
			IplImage* Gray = cvCreateImage( cvGetSize( Img ), 8, 1 ); 
			cvCvtColor( Img, Gray, CV_BGR2GRAY );

			strcpy(buf, argv[2]);
			strcat (buf, "/");
			strcat(buf,file.c_str());

			int all = 100;
			CvRect Rects[100];				
			char** res = new char*[all];
			for(int j=0;j<all;j++) res[j] = new char[20];

			ANPR_OPTIONS a;		
			a.Detect_Mode = ANPR_DETECTCOMPLEXMODE;
			a.min_plate_size = 500;
			a.max_plate_size = 50000;
			a.max_text_size = 20;		
			a.type_number = atoi (argv[3]);
			a.flags = 2;
			CvRect arearect = cvRect( 0, 0, Gray->width, Gray->height );

			int i1 = anprPlateRect( Img, arearect, a, &all, Rects, res ); 
			printf ("%d\n", all);

			if ( i1 == 0 )
			{						
				for(int j = 0; j < all; j++ )				
				{
					puts (res [j]);				
					if ( strlen( res[j] ) >= 6 )
					{						
						cvRectangle( Img,cvPoint( Rects[j].x, Rects[j].y),cvPoint(Rects[j].x+Rects[j].width,
							Rects[j].y+Rects[j].height), CV_RGB(255,255,0), 2);					
						CvFont font;
						float aa=0.001f*Img->width;
						cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, aa,
										aa,0,1, 8 ); 
						CvSize size;
						int b;
						cvGetTextSize( res[j], &font, &size, &b );
						cvRectangle( Img, cvPoint(Rects[j].x - 2, Rects[j].y - b*2 - 8 ), cvPoint( Rects[j].x + size.width + 2, 
							Rects[j].y - b*2  + size.height + 6 ), CV_RGB( 255, 255, 255 ), CV_FILLED );

						CvPoint pp2,pp1;
						pp2.x=Rects[j].x;
						pp2.y=Rects[j].y;
						pp1.x=Rects[j].x+1;
						pp1.y=Rects[j].y+1;
						cvPutText( Img, res[j], pp1, &font, CV_RGB(0,0,0) );
						cvPutText( Img, res[j], pp2, &font, CV_RGB(0,255,0) );						
					}			
				}
			}
			for(int j=0;j<100;j++) delete [] res[j];
			delete [] res;
				
			cvSaveImage( buf, Img);

			cvReleaseImage( &Img );
			cvReleaseImage( &Gray );
	    }
	}
	else
	{
	    fprintf(stderr, "Error opening directory\n");
	}
	return 0;
}
