/******************************************************************************
	(C) 2015. IntBuSoft. http://intbusoft.com
	iANPRcapture.h - version 1.3
*******************************************************************************/
#pragma once
#include "iANPR.h"
#include <opencv2/imgproc/imgproc.hpp>

typedef void* iANPRCapture;

/*
	�������� ���������� �������������.
	max_frames - ���������� ������ �  ������;
	Options - ��������� �������������;
	Rect - ������� �������������.
	������� ���������� ��������� �� ��������� ������
*/
iANPRCapture 
#ifdef WIN32
__stdcall 
#endif 
CreateiANPRCapture( int max_frames, ANPR_OPTIONS Options, CvRect Rect );

// ������� ������ ��� ���������� �������������
void 
#ifdef WIN32
__stdcall 
#endif 	
ReleaseiANPRCapture( iANPRCapture *Capture );

/*
	���������� ����� (�������������).
	Capture - ��������� ����� ������ ���������� �������������;
	Image - ��������� ����������� (8 - ������ ������� (3 ������) ��� 8 ������ 1 ���������);
	AllNumber - ������������ ���������� ������������ ������� �� ��������� ����;
	Texts - ���������� ������.
	������� ���������� 0 ��� �������� ������, ����� ������.
*/
int 
#ifdef WIN32
__stdcall 
#endif 	
AddFrameToiANPRCapture( iANPRCapture Capture, IplImage* Image, int* AllNumber, CvRect* Rects, char** Texts );

int 
#ifdef WIN32
__stdcall 
#endif 	
AddFrameToiANPRCaptureMat( iANPRCapture Capture, cv::Mat Mat, int* AllNumber, CvRect* Rects, char** Texts );

/* 
	�������� ������ ������������� ����������
	Capture - ��������� ����� ������ ���������� �������������; 
	min_frames_with_plate - ���������� ������ ����� ������ � ��������� ������������ �������, ����� �������� ����� �������, ��� ��� ������������� �����;
	frames_without_plate - ���������� ������ ��� ������������� ����� ������, ����� �������� ����� �������� ���������; 
	max_plates_in_mem - ������������ ���������� ������� � ������.
	������� ���������� 0 ��� �������� ������, ����� ������.
*/
int 
#ifdef WIN32
__stdcall 
#endif 	
CreateMemoryForiANPRCapture( iANPRCapture Capture, int min_frames_with_plate, int frames_without_plate, int max_plates_in_mem );

/*
	���������� ������������� ��������� ���������� ������ �� ������. �� ������������ ������ ����� frames_without_plate ������.
	Capture - ��������� ����� ������ ���������� �������������; 
	AllNumber - ������������ ���������� ������������ ������� �� ��������� ���� �� ������;
	Texts - ���������� ������;
	Size_Texts - ������ ������� ������� Texts;
	Points - ��������� �� ���������� ���� ������� ������ CvPoint ��� ���������� (���� NULL, �� �� ������������);
	all_points - ���� ���������� ������ ������� Points, � ������������ ���������� ��������� �����, ���� ������ ������ (������������� 1000 ���������), ���
	������� ������� �����, �� ����� ���������� ������ �� ���������� �����, ��� ������� ���� �����.
	������� ���������� 0 ��� �������� ������, ����� ������.

	��������! ���� ������������ ������������ 2 ������ � �����, ��� ������������, �� ������� ������������ � ��������, �� ����� ����������� ���������� 
	������ ��� �������.
*/
int 
#ifdef WIN32
__stdcall 
#endif 	
GetNumbersInMemory( iANPRCapture Capture,  int* AllNumber, 
					char** Texts, int Size_Texts,
					CvPoint* Points, int* all_point );

/*
	������� ������� ����� ���� �������� �����������. �� ����� ���� ����� ������� �� ���� ����� -
	� ����������� ����������� ������ �����, ����� ������������ ��� (������� ��� ���������� ������
	������������. 
	Capture - ��������� ����� ������ ���������� �������������; 
	p1a � p2a - ����� ��������������� ������� ����� (����� �� ����� ���� ������������ - ��������
	������ x ����� ���� � 3 ���� ������ ������ y. ����� ������ ERROR_SLOPE_LINE); 
	p1b � p2b - ����� ��������������� ������ �����.
	������� ���������� 0 ��� �������� ������, ����� ������.

	��������! ����� ������ ���� ����������� ���� �����. ����� ERROR_NO_PARALLEL_LINES.
*/
int 
#ifdef WIN32
__stdcall 
#endif 	
CreateLineIntersection( iANPRCapture Capture, CvPoint p1a, CvPoint p2a, 
						CvPoint p1b, CvPoint p2b );


void 
#ifdef WIN32
__stdcall 
#endif 
LicenseCapture( char* lic );