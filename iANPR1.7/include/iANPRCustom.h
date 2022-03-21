/******************************************************************************
	(C) 2018. IntBuSoft. http://intbusoft.com
	iANPRCustom.h - version 1.7
*******************************************************************************/
#pragma once

#define FLAG_CUSTOM_MULTI_RESULT		0x01	// ��� CUSTOM ���������� ��������� �������� ��������� ������� ����� "|", ������ �������� ��������� (�� ������ 3-�)

const int max_countries = 5;
const int max_ianpr_custom_in_types = 20; // ������������� ���������� ����� ������� ��� ������ ������

// ��� ������������� ��� ������������� ������������� �������
// ������ ������� ������������� ������. ��� ��������� �� C++
// ����� �������� � ������� ������ �������������� ���� ��������������
// ��� ������ ������ ������������� ��� ������������ ������������ XML
// ���������. � ��� �������� 0 ������� �� ������� �����
struct iANPRCustomElement
{
	int all_types;
	float probability;                                  // ����������� ������
	int types[max_ianpr_custom_in_types];               // ���� ������� � iANPR
	float probability_types[max_ianpr_custom_in_types]; // ����������� ������� ����, ����� �� ���������
};
struct iANPRCustom
{
	int all_countries;
	iANPRCustomElement* Elements;
	int flags; // �� ��������� 0 
};

// ������� ������� ��������� �� iANPRCustom �� ������ xml
#ifndef WIN32
__attribute__((visibility("default")))
#endif
void* 
#ifdef WIN32
__stdcall
#endif
CreateiANPRCustom(char* xml_buffer, int buffer_size);

// ������� ������� �����
void 
#ifdef WIN32
__stdcall
#else
__attribute__((visibility("default")))
#endif
DeleteiANPRCustom(void* xml);
