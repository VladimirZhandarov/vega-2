/******************************************************************************
	(C) 2015. IntBuSoft. http://intbusoft.com
	iANPR.h - version 1.4		
*******************************************************************************/
#pragma once

#include "iANPRerror.h"
#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"

// ������ �������������� �������������� ������
// ������, ���������� �� �������������� ������ �������
#define ANPR_DETECTMODE1			0x01  // ������� ���������� ���������
#define ANPR_DETECTMODE2			0x02  // ���������� ��������� � �������������� ����������� �����������
#define ANPR_DETECTMODE3			0x04  // ������� ���������� ���������
// ������, ���������� �� ��������� ������ ������
#define ANPR_DETECTMODE4			0x08  // ������� ���������� ��������� � ���������� ��������	������

#define ANPR_DETECTCOMPLEXMODE		ANPR_DETECTMODE2 | ANPR_DETECTMODE3 | ANPR_DETECTMODE4

// ���� �������������� ������������� �������
// �� ���� ��������� ����������� � ��������� ������
#define	ANPR_RUSSIAN_BASE				0  // �������
#define	ANPR_RUSSIAN_BASE2				1  // ������� � ����������
#define	ANPR_RUSSIAN_EXTENDED			2  // �������, ���������� � ������ ��������
#define ANPR_RUSSIAN_PUBLIC				3  // ������ ������������ ���������
#define ANPR_RUSSIAN_POLICE				5  // ������ ������ �������
#define ANPR_RUSSIAN_ARMY				6  // ������ ������� ������
#define ANPR_RUSSIAN_SQUARE_BASE		8  // ���������� ������������ ������ ���������� � ���������
// ��������!!! ��������� ������������ �������� ������ ��� ������� �����������
#define	ANPR_RUSSIAN_EXTENDED2			4  // �������, ���������� � ������ �������� � ANPR_RUSSIAN_PUBLIC
#define	ANPR_RUSSIAN_FULL				7  // ANPR_RUSSIAN_EXTENDED2 + ANPR_RUSSIAN_POLICE + ANPR_RUSSIAN_ARMY
#define	ANPR_RUSSIAN_FULL_WITH_SQUARE	9  // ANPR_RUSSIAN_FULL + ANPR_RUSSIAN_SQUARE_BASE

// ������ ����������
// �� ���� ��������� ����������� � ��������� ������
#define ANPR_KAZ_1993_PRIVATE			100
#define ANPR_KAZ_1993_ORGANIZATION		101
#define ANPR_KAZ_2012_PRIVATE			102
#define ANPR_KAZ_2012_ORGANIZATION		103
#define ANPR_KAZ_BASE					104   // ANPR_KAZ_1993_PRIVATE + ANPR_KAZ_1993_ORGANIZATION + ANPR_KAZ_2012_PRIVATE + ANPR_KAZ_2012_ORGANIZATION

// ������ �������������
// �� ���� ��������� ����������� � ��������� ������
#define ANPR_TM_2009					201
#define ANPR_TM_PRIVATE_BEFORE_2009		202
#define ANPR_TM_BASE					203  // ANPR_TM_2009 + ANPR_TM_PRIVATE_BEFORE_2009

// ������ ��������
#define ANPR_BY_TRUCK					300 // ANPR_BY_2004_TRUCK + ANPR_BY_1992_TRUCK
#define ANPR_BY_2004_TRUCK				301 // �������� ���������� � �������� � 2004�.
#define ANPR_BY_1992_TRUCK				302 // �������� ���������� � �������� � 1992�.

// ���������� flags � ANPR_OPTIONS
#define DEBUG_RECOGNITION_MODE			0x01  // ����� ���������� ��� ������������ ������, � ��� ����� � � ��������������� ��������� (���� �������)
#define RETURN_TYPE_NUMBER				0x02  // ���������� ��� ������, ������� � ���� ������ ����� ������������ ����� ��������� � ����� ������
											  // ����� ������ � ���� ������ ��������� �� 4 ������� ������, ��� ������������ ������ ������

// ������������ ���� ������� (���� ��� �� ������������, �� ������ �� ��� �� ��������� ��� ������ ������, ��� � ������ ��� ��� ������ �� ������� �����)
#define TYPE_RUSSIAN_BASE				0	// ������� ����� ������
#define TYPE_RUSSIAN_TRANSIT			1	// ���������� ����� ������
#define TYPE_RUSSIAN_TRAILER			2	// ����� ������� ������
#define TYPE_RUSSIAN_PUBLIC				3	// ������������ ��������� ������
#define TYPE_RUSSIAN_POLICE				4	// ����� ������� ������
#define TYPE_RUSSIAN_ARMY				5	// ������� ����� ������
#define TYPE_RUSSIAN_SQUARE_BASE		6	// ����� �������� ��� ��������� ������

struct ANPR_OPTIONS
{
	int min_plate_size; // ����������� ������� ������
	int max_plate_size; // ������������ ������� ������
	int Detect_Mode; // ������ ��������������	
	int max_text_size; // ������������ ���������� �������� ������ + 1
	int type_number; // ��� �������������� ������ 
	int flags; // �������������� �����
};

int
#ifdef WIN32
__stdcall 
#endif 
anprPlate( IplImage* Image, ANPR_OPTIONS Options, int* AllNumber, CvRect* Rects, char** Texts, void* param = NULL );

int
#ifdef WIN32
__stdcall 
#endif 
anprPlateRect( IplImage* Image, CvRect Rect, ANPR_OPTIONS Options, int* AllNumber, CvRect* Rects, char** Texts, void* param = NULL );

void 
#ifdef WIN32
__stdcall 
#endif 
LicenseValue( char* lic );