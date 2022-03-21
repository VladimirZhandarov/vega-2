//
// (C) 1997-2005 Aktiv Co. All rights reserved.
// Definitions for use Guardant Stealth.
//       Error codes.
//

#ifndef __GRDERROR__H_
#define __GRDERROR__H_

#ifdef      RUS
#define nv_pszErrors    aszGrdErrorRus
#else       // RUS 
#define nv_pszErrors    aszGrdErrorEng
#endif      // RUS 

char *aszGrdErrorRus[GrdE_LastError + 1] =
		//- Error codes ( Russian )
{
	"��� ������",                                                                                                        // 00
	"���� � ��������� ��������� ������ �� ������",                                                                       // 01
	"�������� ��� �������",                                                                                              // 02
	"������ ������� ������� �����",                                                                                      // 03
	"������� ���� � �������� ���p���� ������� �����",                                                                    // 04
	"������� GP ������ 0",                                                                                               // 05
	"����p��� �������",                                                                                                  // 06
	"����-��� �p� ������",                                                                                               // 07
	"������ �����������",                                                                                                // 08
	"������� �p������ �� ������",                                                                                        // 09
	"������� p���p� �p��p���� ����p���",                                                                                 // 10
	"���������� � �������� Guardant Net ����p���",                                                                       // 11
	"������ Guardant Net �� ������",                                                                                     // 12
	"������ ������������� ������ � ������� Guardant Net",                                                                // 13
	"������ DPMI",                                                                                                       // 14
	"����p����� ������ c�p��p� Guardant Net",                                                                            // 15
	"��p��p Guardant Net ��� ��p����p����",                                                                              // 16
	"������� �� �����p�������� � ���� ������",                                                                           // 17
	"NT �p����p �� ������",                                                                                              // 18
	"������ �������� ���������",                                                                                         // 19
	"������� ����� ������������� �������",                                                                               // 20
	"������� ���������� ��������� Login",                                                                                // 21
	"������� ���������� ��������� Logout",                                                                               // 22
	"���� ���� ������ ������������",                                                                                     // 23
	"������� �� ����� ������ LPT ����",                                                                                  // 24
	"��������������� #25",                                                                                               // 25
	"��������������� #26",                                                                                               // 26
	"��������������� #27",                                                                                               // 27
	"��������������� #28",                                                                                               // 28
	"��������������� #29",                                                                                               // 29
	"������ CRC",                                                                                                        // 30
	"������ CRC ��� ������",                                                                                             // 31
	"������ CRC ��� ������",                                                                                             // 32
	"������� ����� �� ������� ������",                                                                                   // 33
	"�� ������� ����p���� � ����������� �������",                                                                        // 34
	"������ CRC ��� ���������� ����p����",                                                                               // 35
	"������ CRC ��� ���������� ChkNSK ��� ��� ������ ������",                                                            // 36
	"������ NSK ����p�� ��� ������� �����",                                                                              // 37
	"����� ��������� ��� �����o��p������ �p���p��������",                                                                // 38
	"����������� ������ ��� ������ � ����������/�������, �������� ����� �� �����������",                                 // 39
	"�������� ������ ���������",                                                                                         // 40
	"��������� ������ ����������� ����������� ���������� ������",                                                        // 41
	"��� ������� ���������/������ �� ������������ ������ ��� �������",                                                   // 42
	"��������/������ ��������� � ��������� Inactive, ������� �� ���������",                                              // 43
	"������� ��������� ��������, ������� �� ������������ ������� ����-������",                                           // 44
	"� ������ ������ ���� �� ����� ��������� ������� ��������",                                                          // 45
	"������������ ��������� ��� ������ �������",                                                                         // 46
	"������ ������������� ������",                                                                                       // 47
	"������������ �����",                                                                                                // 48
	"���� ���������� ��������� ��� ������������",                                                                        // 49
	"��������������� #50",                                                                                               // 50
	"��������������� #51",                                                                                               // 51
	"��������������� #52",                                                                                               // 52
	"�������� ����������� ��������� ������",                                                                             // 53
	"������ ��� ���������� ���������� �� ��� ������������",                                                              // 54
	"������������ ������ ������ ��� ���������� ����������",                                                              // 55
	"������ ��� ���������� ���������� ��� ������������",                                                                 // 56
	"��������� ������ ��� ��������� ���������� �� ���������",                                                            // 57
	"�������� �������� ���� ������ ���������� ����������",                                                               // 58
	"���������� ������",                                                                                                 // 59
	"������ ����� Guardant API ��� �������������������",                                                                 // 60
	"������ �������",                                                                                                    // 61
	"������ ������� �������",                                                                                            // 62
	"��������� ����� �����/���������� � ������� ���� ART",                                                               // 63
	"������ � ������� AAT �� ��������� EEPROM",                                                                          // 64
	"���������� ���� �� ������������",                                                                                   // 65
	"������� ������ ����",                                                                                               // 66
	"�������� �������� �������",                                                                                         // 67
	"��������� ����� ������� ���� ���������",                                                                            // 68
	"���������� ���� ���������� �� ���������",                                                                           // 69
	"���������� ���� �������",                                                                                           // 70
	"������� ���������� ������������������� Guardant API",                                                               // 71
	"��������� �������� ������������ ����",                                                                              // 72
	"����������� ��� �� ��� �������� �� ���������� �������",                                                             // 73
	"� ����������� ������� ������������� ������ flash-������ ��� ����������������� ����������",                          // 74
	"��������������� #75",                                                                                               // 75
	"��������������� #76",                                                                                               // 76
	"��������������� #77",                                                                                               // 77
	"��������������� #78",                                                                                               // 78
	"��������������� #79",                                                                                               // 79
	"������ ������������ ������ ������� ��������� TGrdLoadableCodeData",                                                 // 80
	"����������� ������ ������� RAM-������ � ����������� �����������",                                                   // 81
	"����������� ������ ������� FLASH-������ � ����������� �����������",                                                 // 82
	"����������� �������� FLASH-������ �������� � ���������� ������������",                                              // 83
	"������� ������� BMAP ����",                                                                                         // 84
	"�������� ��������� �������� �������",                                                                               // 85
	"������ ��� �������� ������",                                                                                        // 86
	"������ ��������� ��� ����������",                                                                                   // 87
	"��� ����������� ��������� ������������",                                                                            // 88
	"����������� � ����������� ����� �����/������ ������������ ��� ��������/������ ������ ��������� ������������",       // 89
	"��� ���������� ���� ��������� ��������� ������ ����������� �����",                                                  // 90
	"����� �����/������, �������� � ��������� ������������, ������� �� ���������� ������� ������",                       // 91
	"����� �� ������� ���������� ������� FLASH-������.",                                                                 // 92
	"�������� ������������ ����������� ��������� ������������ ������������ � ��� ����������� (��������� �������� Init)", // 93
	"������������ ������ ����� GCEXE",                                                                                   // 94
	"������ ������������ ������� ��� ������������ ���� ��� ������ GcaCodeRun",                                           // 95
	"������� ����� ��������� ������� GcaCodeRun",                                                                        // 96
	"�� ������� ������� ���������������� ���� �������",                                                                  // 97
	"����������� ������ #"                                                                                               // 98 (all errors >= GrdE_LastError )
};

char *aszGrdErrorEng[GrdE_LastError + 1] =
		//- Error codes ( English )
{
	"No errors",                                                                                             // 00
	"Dongle with specified search conditions not found",                                                     // 01
	"Code not found",                                                                                        // 02
	"The specified address is too big",                                                                      // 03
	"Byte counter too big",                                                                                  // 04
	"GP executions counter exhausted (has 0 value)",                                                         // 05
	"Invalid dongle call command",                                                                           // 06
	"Write time out error",                                                                                  // 07
	"Write verification error",                                                                              // 08
	"Network protocol not found",                                                                            // 09
	"License counter of Guardant Net exhausted",                                                             // 10
	"Connection with Guardant Net server was lost",                                                          // 11
	"Guardant Net server not found",                                                                         // 12
	"Guardant Net server memory allocation error",                                                           // 13
	"Guardant Net server found DPMI error",                                                                  // 14
	"Guardant Net server internal error",                                                                    // 15
	"Guardant Net server has been reloaded",                                                                 // 16
	"This command is not supported by this dongle version (the version is too old)",                         // 17
	"Windows NT driver is required",                                                                         // 18
	"Network protocol error",                                                                                // 19
	"Network packet format is not supported",                                                                // 20
	"Logging in Guardant dongle is required",                                                                // 21
	"Logging out from Guardant dongle is required",                                                          // 22
	"Guardant dongle locked by another copy of protected application",                                       // 23
	"Guardant driver cannot capture the parallel port",                                                      // 24
	"Reserved #25",                                                                                          // 25
	"Reserved #26",                                                                                          // 26
	"Reserved #27",                                                                                          // 27
	"Reserved #28",                                                                                          // 28
	"Reserved #29",                                                                                          // 29
	"CRC error occurred while attempting to call the dongle",                                                // 30
	"CRC error occurred while attempting to read data from the dongle",                                      // 31
	"CRC error occurred while attempting to write data to the dongle",                                       // 32
	"The boundary of the dongle's memory has been override",                                                 // 33
	"The hardware algorithm with specified number has not been found in the dongle",                         // 34
	"CRC error of the hardware algorithm",                                                                   // 35
	"All dongles found, or CRC error occurred while attempting to execute ChkNSK operation",                 // 36
	"Guardant API release is too old",                                                                       // 37
	"Non-existent reversible conversion method has been specified",                                          // 38
	"Unknown error in work with algorithm or protected item, operation may by not complete",                 // 39
	"Invalid activation password",                                                                           // 40
	"Error counter exhaust",                                                                                 // 41
	"Specifed algorithm or protected item not support requested service",                                    // 42
	"This is a inactive algorithm or protected item, command not executed",                                  // 43
	"Dongle server not support specifed command",                                                            // 44
	"Dongle busy at this moment and can't execute any command",                                              // 45
	"One or more function arguments are invalid",                                                            // 46
	"Memory allocation error",                                                                               // 47
	"Invalid handle",                                                                                        // 48
	"This protected container is already in use",                                                            // 49
	"Reserved #50",                                                                                          // 50
	"Reserved #51",                                                                                          // 51
	"Reserved #52",                                                                                          // 52
	"Remote update system data corrupted",                                                                   // 53
	"Remote update question has not been generated",                                                         // 54
	"Invalid remote update data format",                                                                     // 55
	"Remote update question has been already generated",                                                     // 56
	"Remote update writing has not been completed",                                                          // 57
	"Invalid remote update data hash",                                                                       // 58
	"Internal error",                                                                                        // 59
	"This copy of Guardant API has been already initialized",                                                // 60
	"Real Time Clock error",                                                                                 // 61
	"Real Time Clock battery low error",                                                                     // 62
	"Duplicate items/algorithms names",                                                                      // 63
	"Address in AAT table is out of range",                                                                  // 64
	"Session key not generated",                                                                             // 65
	"Invalid public key",                                                                                    // 66
	"Invalid digital sign",                                                                                  // 67
	"Session key generation error",                                                                          // 68
	"Invalid session key",                                                                                   // 69
	"Session key too old",                                                                                   // 70
	"Guardant API must be initialized",                                                                      // 71
	"Verification of loadable code failed",                                                                  // 72
	"Loadable code execute time out",                                                                        // 73
	"Flash-size from descriptor for User Firmware too small",                                                // 74
	"Reserved #75",                                                                                          // 75
	"Reserved #76",                                                                                          // 76
	"Reserved #77",                                                                                          // 77
	"Reserved #78",                                                                                          // 78
	"Reserved #79",                                                                                          // 79
	"TGrdLoadableCodeData structure exceeds space reserved for determinant in item descriptor",              // 80
	"Incorrect RAM area specified in loadable code descriptor",                                              // 81
	"Incorrect FLASH memory area specified in loadable code descriptor",                                     // 82
	"Allocation of intersecting FLASH memory areas for different loadable code modules",                     // 83
	"BMAP file is too long",                                                                                 // 84
	"The loadable code has zero length",                                                                     // 85
	"Data verification failed",                                                                              // 86
	"Error in Guardant Code protocol",                                                                       // 87
	"Loadable code not found",                                                                               // 88
	"IO buffer size specified in loadable code is not enough for transmitting/receiving data",               // 89
	"Security violation in Guardant Code virtual environment",                                               // 90
	"IO buffer specified in loadable code exceeds the bounds of allowed RAM area",                           // 91
	"Loadable code exceeds the bounds of allowed FLASH memory area",                                         // 92
	"Allocation of intersecting RAM areas for different loadable code modules (Init operation is required)", // 93
	"Incorrect GCEXE file format",                                                                           // 94
	"Incorrect RAM area specified in loadable code for GcaCodeRun",                                          // 95
	"Too many nested calls of GcaCodeRun",                                                                   // 96
	"Unable to create client configuration file",                                                            // 97
	"Unknown error #"                                                                                        // 98 (all errors >= GrdE_LastError )
};

#endif // !__GRDERROR__H_
