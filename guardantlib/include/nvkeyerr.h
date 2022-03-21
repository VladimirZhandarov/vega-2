/*
 * (C) 1997-2002 Aktiv Co. All rights reserved.
 * Definitions for use Guardant Stealth.
 *         Error codes.
 */

#ifndef __NVSKERR__H_
#define __NVSKERR__H_

const char *nv_pszErrors[nse_LastError + 1] =

#ifdef  RUS
		/*- Error codes ( Russian ) ------------------------------------------------*/
{
	"��� ������",                                                        /* 00 */
	"���� �� ������",                                                    /* 01 */
	"���� ����, �� �� � ��� �����",                                      /* 02 */
	"������ ������� ������� �����",                                      /* 03 */
	"������� ���� � �������� ���p���� ������� �����",                    /* 04 */
	"GP ������ 0",                                                       /* 05 */
	"����p��� �������",                                                  /* 06 */
	"����-��� �p� ������",                                               /* 07 */
	"������ �����������",                                                /* 08 */
	"������� �p������ �� ������",                                        /* 09 */
	"������� p���p� �p��p���� ����p���",                                 /* 10 */
	"���������� � ����-��p��p�� ����p���",                               /* 11 */
	"��� �� ������ ����-��p��p�",                                        /* 12 */
	"����-��p��p� �� ������� ������",                                    /* 13 */
	"�������� ������ DPMI-��p��p�",                                      /* 14 */
	"����p����� ������",                                                 /* 15 */
	"����-��p��p ��� ��p����p����",                                      /* 16 */
	"������� �� �����p�������� � ���� ������",                           /* 17 */
	"NT �p����p �� ������",                                              /* 18 */
	"������ �������� ���������",                                         /* 19 */
	"������� ����� ������������� �������",                               /* 20 */
	"������� ���������� ��������� Login",                                /* 21 */
	"������� ���������� ��������� Logout",                               /* 22 */
	"���� ���� ������ �����",                                            /* 23 */
	"������� �� ����� ������ LPT ����",                                  /* 24 */
	"",                                                                  /* 25 */
	"",                                                                  /* 26 */
	"",                                                                  /* 27 */
	"",                                                                  /* 28 */
	"",                                                                  /* 29 */
	"������ CRC",                                                        /* 30 */
	"������ CRC ��� ������",                                             /* 31 */
	"������ CRC ��� ������",                                             /* 32 */
	"������� ����� �� ������� ������",                                   /* 33 */
	"�� ������� ����p���� � ����������� �������",                        /* 34 */
	"������ CRC ��� ���������� ����p����",                               /* 35 */
	"������ CRC ��� ���������� ChkNSK ��� ��� ������ ������",            /* 36 */
	"������ NSK ����p�� ��� ������� �����",                              /* 37 */
	"����� ��������� ��� �������-��p������ �p���p��������",              /* 38 */
	"",                                                                  /* 39 */
	"",                                                                  /* 40 */
	"",                                                                  /* 41 */
	"",                                                                  /* 42 */
	"",                                                                  /* 43 */
	"",                                                                  /* 44 */
	"",                                                                  /* 45 */
	"������������ ��������� ��� ������ �������"                          /* 46 */
};

#else           /* RUS  */

		/*- Error codes ( English ) ------------------------------------------------*/
		{
			"No errors",                                                         /* 00 */
			"Dongle with specified search conditions not found",                 /* 01 */
			"Code not found",                                                    /* 02 */
			"The specified address is too big",                                  /* 03 */
			"Byte counter too big",                                              /* 04 */
			"GP executions counter exhausted (has 0 value)",                     /* 05 */
			"Bad dongle call command",                                           /* 06 */
			"Write time out error",                                              /* 07 */
			"Write verification error",                                          /* 08 */
			"Network protocol not found",                                        /* 09 */
			"License counter of Guardant Net exhausted",                         /* 10 */
			"Connection with Guardant Net server was lost",                      /* 11 */
			"Guardant Net server not found",                                     /* 12 */
			"Guardant Net server memory allocation error",                       /* 13 */
			"Guardant Net server found DPMI error",                              /* 14 */
			"Guardant Net server internal error",                                /* 15 */
			"Guardant Net server has been reloaded",                             /* 16 */
			"This command is not supported by this dongle version (the version is too old)",/* 17 */
			"Windows NT driver is required",                                     /* 18 */
			"Network protocol error",                                            /* 19 */
			"Network packet format is not supported",                            /* 20 */
			"Logging in Guardant Net server is required",                        /* 21 */
			"Logging out from Guardant Net server is required",                  /* 22 */
			"Guardant Net is busy (locked by another copy of protected application)",/* 23 */
			"Guardant driver cannot capture the parallel port",                  /* 24 */
			"",                                                                  /* 25 */
			"",                                                                  /* 26 */
			"",                                                                  /* 27 */
			"",                                                                  /* 28 */
			"",                                                                  /* 29 */
			"CRC error occurred while attempting to call the dongle",            /* 30 */
			"CRC error occurred while attempting to read data from the dongle",  /* 31 */
			"CRC error occurred while attempting to write data to the dongle",   /* 32 */
			"The boundary of the dongle's memory has been override",             /* 33 */
			"The hardware algorithm with this number has not been found in the dongle",/* 34 */
			"CRC error of the hardware algorithm",                               /* 35 */
			"CRC error occurred while attempting to execute ChkNSK operation, or all dongles found",/* 36 */
			"Guardant API release is too old",                                   /* 37 */
			"Non-existent reversible conversion method has been specified",      /* 38 */
			"",                                                                  /* 39 */
			"",                                                                  /* 40 */
			"",                                                                  /* 41 */
			"",                                                                  /* 42 */
			"",                                                                  /* 43 */
			"",                                                                  /* 44 */
			"",                                                                  /* 45 */
			"One or more function arguments are invalid"                         /* 46 */
		};

#endif  /* RUS  */

/*--------------------------------------------------------------------------*/

#endif  /* __NVSKERR__H_        */
