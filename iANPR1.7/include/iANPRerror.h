/******************************************************************************
	(C) 2018. IntBuSoft. http://intbusoft.com
	iANPRerror.h - version 1.7
*******************************************************************************/
#pragma once

#define IMAGE_EMPTY						-2	  // ����������� ������
#define ERROR_RECT						-1	  // ������ � �������� ������� �� �����������
#define ERROR_SIZE_XML_BUF				-3	  // ������������� ������ XML-������
#define ERROR_TYPE_PLATE				-100  // ���������������� ��� ������ ������������ ��� ������
#define ERROR_TYPE_FOR_COLOR			-101  // ���������������� ���� ��� �������� ����������� - ������ ANPR_RUSSIAN_EXTENDED2 � ANPR_RUSSIAN_EXTENDED3
#define ERROR_NO_CUSTOM_DATA			-102  // ��� ������ ��� �������������� ���� ������

#define CAPTURE_EMPTY					-1000 // ������ ����� iANPRCapture
#define ERROR_CREATEMEMORY_PARAM		-1001 // ������������ ��������� � ������� CreateMemoryForiANPRCapture
#define ERROR_SLOPE_LINE				-1002 // �������� ������ �����
#define ERROR_NO_PARALLEL_LINES			-1003 // ����� �� �����������
