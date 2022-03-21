/******************************************************************************
	(C) 2018. IntBuSoft. http://intbusoft.com
	iANPR.h - version 1.7
*******************************************************************************/
#pragma once

#include "iANPRerror.h"
#include "iANPRCustom.h"
#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"


// ������ �������������� �������������� ������
// ������, ���������� �� �������������� ������ �������
#define ANPR_DETECTMODE1			0x01  // ������� ���������� ���������
#define ANPR_DETECTMODE2			0x02  // ���������� ��������� � �������������� ����������� �����������
#define ANPR_DETECTMODE3			0x04  // ������� ���������� ���������
// ������, ���������� �� ��������� ������ ������
#define ANPR_DETECTMODE4			0x08  // ������� ���������� ��������� � ���������� ��������	������

#define ANPR_DETECTCOMPLEXMODE            (ANPR_DETECTMODE2 | ANPR_DETECTMODE3 | ANPR_DETECTMODE4)

// ���� �������������� ������������� �������

// ������� �����������
#define ANPR_CUSTOM_TYPE				-1 // ������������� ��� �������������� ������

// �� ���� ��������� ����������� � ��������� ������
#define	ANPR_RUSSIAN_BASE				0  // �������
#define	ANPR_RUSSIAN_BASE2				1  // ������� � ����������
#define	ANPR_RUSSIAN_EXTENDED			2  // �������, ���������� � ������ ��������
#define ANPR_RUSSIAN_PUBLIC				3  // ������ ������������ ���������
#define ANPR_RUSSIAN_POLICE				5  // ������ ������ �������
#define ANPR_RUSSIAN_ARMY				6  // ������ ������� ������
#define ANPR_RUSSIAN_SQUARE_BASE		8  // ���������� ������������ ������ ���������� � ���������
#define ANPR_RUSSIAN_DIPLOMAT			10  // ���������� ��������������� ������
// ��������!!! ��������� ������������ �������� ������ ��� ������� �����������
#define	ANPR_RUSSIAN_EXTENDED2			4  // �������, ���������� � ������ �������� � ANPR_RUSSIAN_PUBLIC
#define	ANPR_RUSSIAN_FULL				7  // ANPR_RUSSIAN_EXTENDED2 + ANPR_RUSSIAN_POLICE + ANPR_RUSSIAN_ARMY + ANPR_RUSSIAN_DIPLOMAT
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
#define ANPR_BY_2004_TRAILER			303 // ������ ���� ��� �������� � ������������ � 2004 �.
#define ANPR_BY_2004_BASE				304 // �������� ����������, ������� � 2004 �.
#define ANPR_BY_SQUARE_BASE				305 // ������ ������������ �����, ������� � 2004�.
#define ANPR_BY_2004_BASE2				306 // ANPR_BY_2004_TRUCK + ANPR_BY_2004_BASE + ANPR_BY_2004_TRAILER
#define ANPR_BY_TRANSIT					307 // ���������� ������
#define ANPR_BY_PUBLIC					308 // ������������ ���������
#define ANPR_BY_POLICE					309 // ������ ���
#define ANPR_BY_FULL					310 // ANPR_BY_2004_BASE2 + ANPR_BY_TRANSIT + ANPR_BY_PUBLIC + ANPR_BY_POLICE
#define ANPR_BY_FULL_WITH_SQUARE		311 // ANPR_BY_FULL + ANPR_BY_SQUARE_BASE
#define ANPR_BY_2004_BASE3				312 // ANPR_BY_2004_BASE2 + ANPR_BY_TRANSIT

// ������ ������
#define ANPR_PL_BASE					400 // ����������� ������ ������ ANPR_PL_BASE_7 | ANPR_PL_BASE_8
#define ANPR_PL_BASE_7					401 // ����������� ������ ������ 7 ��������
#define ANPR_PL_BASE_8					402 // ����������� ������ ������ 8 ��������

// ������ ������
#define ANPR_LV_BASE					500 // ����������� ������ 1992 � 2004 ����

// ������ �����
#define ANPR_LT_BASE					600 // ����������� ������ � 2004 ����

// ������ �������
#define ANPR_EST_BASE					700 // ����������� ������ 2004 ����

// ������ �������
#define ANPR_UA_BASE					800 // ����������� ������ 2015 ���� (����������, �������, ��������)
#define ANPR_UA_TRANSIT					801 // ������ ��� ������� ������� �� �����������, �������� � ��� � ���������, 2015 ���
#define ANPR_UA_DIPLOMAT				802 // ����� ��� �������������� ����������� ��������������� ����������������, ����������, ���������������� ������������� �����������, ���������� ��������������� �������� + ������ ������������. � 1 ���� 2013 ����
#define ANPR_UA_BASE2					803 // ANPR_UA_BASE + ANPR_UA_DIPLOMAT
#define ANPR_UA_BASE3					804 // ANPR_UA_BASE2 + ANPR_UA_TRANSIT

// ������ �������
#define ANPR_MD_2011_BASE				900 // ������ ���������� � ����������� ��� 2011-2015 ���� + ������ �����
#define ANPR_MD_2011_TRAILER			901 // ������ �������� � ������������ 2011-2015 ����
#define ANPR_MD_2011_SQUARE_BASE		902 // ������ ����������� 2011-2015 ����
#define ANPR_MD_2011_BASE2				903 // ANPR_MD_2011_BASE + ANPR_MD_2011_TRAILER
#define ANPR_MD_2011_FULL_WITH_SQUARE	911 // ANPR_MD_2011_BASE2 + ANPR_MD_2011_SQUARE_BASE

// ������ ������, ��� ������� ���������� ���������� �� ������������� �� ����������� � �������� ������ (�������������), � ������� (��������������) �����������.
// �� ���� ��������� ������� ���������� ����������� � �������� ������ (�������������)
static const int anprFullTypesCount = 6;
static const int anprFullTypes [anprFullTypesCount] = 
{
	ANPR_RUSSIAN_EXTENDED2, ANPR_RUSSIAN_FULL, ANPR_RUSSIAN_FULL_WITH_SQUARE,
	ANPR_BY_FULL, ANPR_BY_FULL_WITH_SQUARE,
	ANPR_MD_2011_FULL_WITH_SQUARE
};

// ���������� flags � ANPR_OPTIONS
#define DEBUG_RECOGNITION_MODE			0x01  // ����� ���������� ��� ������������ ������, � ��� ����� � � ��������������� ��������� (���� �������)
#define RETURN_TYPE_NUMBER				0x02  // ���������� ��� ������, ������� � ���� ������ ����� ������������ ����� ��������� � ����� ������
											  // ����� ������ � ���� ������ ��������� �� 4 ������� ������, ��� ������������ ������ ������
#define NO_LOW_RELIABILITY				0x08  // �� ���������� ������ � ������ ��������������
#define ONLY_HORIZONTAL_NUMBER			0x04  // ������ ������������� ��������� ����� ������� ������, ��������� ������ ������������
                                              // �� �������� ��� ANPR_DETECTMODE4
#define IR_LIGHTING_CAMERA				0x10  // ��� �����, � ����� ������

// ������������ ���� ������� (���� ��� �� ������������, �� ������ �� ��� �� ��������� ��� ������ ������, ��� � ������ ��� ��� ������ �� ������� �����)
// -1 ������������ ��� �������������� ����
#define TYPE_RUSSIAN_BASE				0	// ������� ����� ������
#define TYPE_RUSSIAN_TRANSIT			1	// ���������� ����� ������
#define TYPE_RUSSIAN_TRAILER			2	// ����� ������� ������
#define TYPE_RUSSIAN_PUBLIC				3	// ������������ ��������� ������
#define TYPE_RUSSIAN_POLICE				4	// ����� ������� ������
#define TYPE_RUSSIAN_ARMY				5	// ������� ����� ������
#define TYPE_RUSSIAN_SQUARE_BASE		6	// ����� �������� ��� ��������� ������
#define TYPE_RUSSIAN_DIPLOMAT			7  // ���������� ��������������� ������
#define TYPE_TM_BASE					10
#define TYPE_KAZ_PRIVATE1993			21  
#define TYPE_KAZ_ORGANIZATION1993		22  
#define TYPE_KAZ_PRIVATE2012			23  
#define TYPE_KAZ_ORGANIZATION2012		24  
#define TYPE_KAZ_BASE					25
#define TYPE_BY_2004_BASE				30	// ������� ����� ���������� ��������
#define TYPE_BY_TRANSIT					31	// ���������� ����� ���������� ��������
#define TYPE_BY_2004_TRAILER			32	// ����� ������� ���������� ��������
#define TYPE_BY_PUBLIC					33	// ������������ ��������� ���������� ��������
#define TYPE_BY_POLICE					34	// ����� ������� ���������� ��������
#define TYPE_BY_ARMY					35	// ������� ����� ���������� ��������
#define TYPE_BY_SQUARE_BASE				36	// ���������� ������ ���������� ��������
#define TYPE_BY_2004_TRUCK				37	// �������� ���������� � �������� ���������� �������� c 2004
#define TYPE_BY_1992_TRUCK				38	// �������� ���������� � �������� ���������� �������� c 1992
#define TYPE_PL_BASE					40	// ����������� ������ ������ (3 �����, 2 �����, 2 �����), 2000 � 2006 ���
#define TYPE_LV_BASE					50	// ����������� ������ ������ 1992 � 2004 ����
#define TYPE_LT_BASE					60	// ����������� ������ ����� � 2004 ����
#define TYPE_EST_BASE					70	// ����������� ������ ������� 2004 ����
#define TYPE_UA_BASE					80	// ����������� ������ ������� 2015 ���� (�������� ����������, �������, ��������)
#define TYPE_UA_TRANSIT					81	// ������ ��� ������� ������� �� �����������, �������� � ��� � ���������, 2015 ���
#define TYPE_UA_DIPLOMAT				82	// ����� ��� �������������� ����������� ��������������� ����������������, ����������, ���������������� ������������� �����������, ���������� ��������������� �������� + ������ ������������. � 1 ���� 2013 ����
#define TYPE_MD_2011_BASE				90	// ������ ���������� � ����������� ��� 2011-2015 ����
#define TYPE_MD_2011_TRAILER			91	// ������ �������� � ������������ 2011-2015 ����
#define TYPE_MD_2011_SQUARE_BASE		92	// ������ ����������� 2011-2015 ����

struct ANPR_OPTIONS
{
	char sign1 = 'i', sign2 = 'a', sign3 = '1';	 // ��������� ����������, �� ������ ����������������

	int min_plate_size;  // ����������� ������� ������
	int max_plate_size;  // ������������ ������� ������
	int Detect_Mode;     // ������ ��������������	
	int max_text_size;   // ������������ ���������� �������� ������ + 1
	int type_number;     // ��� �������������� ������ 
	int flags;           // �������������� �����
	void* custom;		// ����������� ������ ��� ���� ANPR_CUSTOM_TYPE, ����� �����
	/*
		void, � �� iANPRCustom �� ������� ����, ����� � �� �/C++ ������ �� ����� ���� ��������� ��������� ������
	*/
	char* vers = "1.7"; // ������������ ������ iANPR SDK. ���� �� ������ (vers = 0), �� ��������� 1.5.

	// ��� ����������� ����� �������� ����� ���� �������������� �������� persptrans, ������� ����� ����� � ������� iANPR SDK.
	// 90.0 �������� ���������� ��������
	double alpha = 90.0;			// ������� ������ ��� X
	double beta = 90.0;			// ������� ������ ��� Y
	double gamma = 90.0;			// ������� ������ ��� Z

	int max_threads = 1;		// ���������� �������
};


// ���� ��� CUSTOM
// ������
#define CUSTOM_RUSSIAN_BASE_EXTENDED		ANPR_RUSSIAN_EXTENDED
#define CUSTOM_RUSSIAN_PUBLIC				ANPR_RUSSIAN_PUBLIC	
#define CUSTOM_RUSSIAN_POLICE				ANPR_RUSSIAN_POLICE
#define CUSTOM_RUSSIAN_ARMY					ANPR_RUSSIAN_ARMY
#define CUSTOM_RUSSIAN_SQUARE				ANPR_RUSSIAN_SQUARE_BASE
#define CUSTOM_RUSSIAN_DIPLOMAT				ANPR_RUSSIAN_DIPLOMAT
// ���������
#define CUSTOM_KAZ_1993_PRIVATE				ANPR_KAZ_1993_PRIVATE
#define CUSTOM_KAZ_1993_ORGANIZATION		ANPR_KAZ_1993_ORGANIZATION
#define CUSTOM_KAZ_2012_PRIVATE				ANPR_KAZ_2012_PRIVATE
#define CUSTOM_KAZ_2012_ORGANIZATION		ANPR_KAZ_2012_ORGANIZATION
// ��������
#define CUSTOM_BY_2004_BASE2				ANPR_BY_2004_BASE2
#define CUSTOM_BY_TRANSIT					ANPR_BY_TRANSIT
#define CUSTOM_BY_PUBLIC					ANPR_BY_PUBLIC
#define CUSTOM_BY_POLICE					ANPR_BY_POLICE
#define CUSTOM_BY_SQUARE_BASE				ANPR_BY_SQUARE_BASE
// ������
#define CUSTOM_PL_BASE_7					ANPR_PL_BASE_7
#define CUSTOM_PL_BASE_8					ANPR_PL_BASE_8
// ������
#define CUSTOM_LV_BASE						ANPR_LV_BASE
// �����
#define CUSTOM_LT_BASE						ANPR_LT_BASE
// �������
#define CUSTOM_EST_BASE						ANPR_EST_BASE
// �������
#define CUSTOM_MD_2011_BASE					ANPR_MD_2011_BASE
#define CUSTOM_MD_2011_TRAILER				ANPR_MD_2011_TRAILER
#define CUSTOM_MD_2011_SQUARE_BASE			ANPR_MD_2011_SQUARE_BASE
// �������
#define CUSTOM_UA_BASE						ANPR_UA_BASE
#define CUSTOM_UA_TRANSIT					ANPR_UA_TRANSIT
#define CUSTOM_UA_DIPLOMAT					ANPR_UA_DIPLOMAT

int
#ifdef WIN32
__stdcall 
#else
__attribute__ ((visibility("default")))
#endif 
anprPlate( IplImage* Image, ANPR_OPTIONS Options, int* AllNumber, CvRect* Rects, char** Texts, void* param = NULL );

int
#ifdef WIN32
__stdcall 
#else
__attribute__ ((visibility("default")))
#endif 
anprPlateRect( IplImage* Image, CvRect Rect, ANPR_OPTIONS Options, int* AllNumber, CvRect* Rects, char** Texts, void* param = NULL );

void 
#ifdef WIN32
__stdcall 
#else
__attribute__ ((visibility("default")))
#endif 
LicenseValue( char* lic );

int
#ifdef WIN32
__stdcall
#else
__attribute__((visibility("default")))
#endif 
createIANPROptions(char charArray[sizeof (ANPR_OPTIONS)], char sing1, char sing2, char sing3, int min_plate_size, int max_plate_size, int Detect_Mode,
int max_text_size, int type_number, int flags, void* custom, char* vers, double alpha, double beta, double gamma, int max_threads);