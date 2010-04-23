/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009

  M Roberts (original release)
  Robin Birch <robinb@ruffnready.co.uk>
  Samuel Gisiger <samuel.gisiger@triadis.ch>
  Jeff Goodenough <jeff@enborne.f2s.com>
  Alastair Harrison <aharrison@magic.force9.co.uk>
  Scott Penrose <scottp@dd.com.au>
  John Wharington <jwharington@gmail.com>
  Lars H <lars_hn@hotmail.com>
  Rob Dunning <rob@raspberryridgesheepfarm.com>
  Russell King <rmk@arm.linux.org.uk>
  Paolo Ventafridda <coolwind@email.it>
  Tobias Lohner <tobias@lohner-net.de>
  Mirek Jezek <mjezek@ipplc.cz>
  Max Kellermann <max@duempel.org>
  Tobias Bieniek <tobias.bieniek@gmx.de>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
 */

#include "Logger/LoggerGRecord.hpp"
#include "Logger/MD5.hpp"
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

const TCHAR *
GRecord::GetVersion() const
{
  return _T("Version 1.0.2");
}

int
GRecord::Init()
{  // key #1 used w/ Vali 1.0.0
   // key #2 used w/ Vali 1.0.2
  return Init(2);  // OLC uses key #2 since 9/1/2008
}

int
GRecord::AppendRecordToBuffer(const TCHAR *szIn)
{
  unsigned int iLen = _tcslen(szIn);
  unsigned char buff[BUFF_LEN];

  for (unsigned int i = 0; i <= iLen ; i++) {
    buff[i] = (unsigned char) szIn[i];
  }

  return AppendRecordToBuffer( buff);
}

int
GRecord::AppendRecordToBuffer(const unsigned char *szIn)
{ // returns 1 if record is appended, 0 if skipped

  int iRetVal = 0;
  if ( IncludeRecordInGCalc(szIn) ) {
    iRetVal = 1;
    for (int i = 0; i < 4; i++) {
      oMD5[i].AppendString(szIn, 1);
    }
  }

  return iRetVal;
}

int
GRecord::AppendStringToBuffer(const unsigned char *szIn)
{
  for (int i = 0; i < 4; i++) {
    oMD5[i].AppendString(szIn, 1); // skip whitespace flag=1
  }
  return 1;
}

int
GRecord::FinalizeBuffer()
{
  for (int i = 0; i < 4; i++) {
    oMD5[i].Finalize();
  }
  return 1;
}

int GRecord::GetDigest(TCHAR * szOutput)
{
  TCHAR TempBuff[BUFF_LEN];
  for (int idig=0; idig <=3; idig++) {
    oMD5[idig].GetDigest(TempBuff);
    for (unsigned int i = 0; i < 32; i++) {
      szOutput[idig*32 + i] = TempBuff[i];
    }
  }

  szOutput[128]='\0';
  return 1;
}


int GRecord::Init (int iKey)
{

  unsigned int i=0;
  for (i=0; i< BUFF_LEN; i++) {
    Digest[i]=0;
    DigestRead[i]=0;
    FileName[i]=0;
  }
  ptrDigest=0;
  ptrFileName=0;


  for (i=0; i < 3; i++) {
    oMD5[i].InitDigest();
  }

  switch ( iKey) // 4 different 512 bit keys
  {
  case 1: // key 1
    oMD5[0].InitKey(0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476);
    oMD5[1].InitKey(0x48327203, 0x3948ebea, 0x9a9b9c9e, 0xb3bed89a);

    oMD5[2].InitKey(0x67452301, 0xefcdab89,  0x98badcfe, 0x10325476);
    oMD5[3].InitKey( 0xc8e899e8, 0x9321c28a, 0x438eba12, 0x8cbe0aee);
    break;

  case 2: // key 2

    oMD5[0].InitKey(0x1C80A301,0x9EB30b89,0x39CB2Afe,0x0D0FEA76);
    oMD5[1].InitKey(0x48327203,0x3948ebea,0x9a9b9c9e,0xb3bed89a);

    oMD5[2].InitKey(0x67452301,0xefcdab89,0x98badcfe,0x10325476);
    oMD5[3].InitKey(0xc8e899e8,0x9321c28a,0x438eba12,0x8cbe0aee);
    break;

  case 3: // key 3

    oMD5[0].InitKey(0x7894abde,0x9cb4e90a,0x0bc8f0ea,0x03a9e01a);
    oMD5[1].InitKey(0x3c4a4c93,0x9cbf7ae3,0xa9bcd0ea,0x9a8c2aaa);

    oMD5[2].InitKey(0x3c9ae1f1,0x9fe02a1f,0x3fc9a497,0x93cad3ef);
    oMD5[3].InitKey(0x41a0c8e8,0xf0e37acf,0xd8bcabe2,0x9bed015a);
    break;

  default:  // key 1
    oMD5[0].InitKey(0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476);
    oMD5[1].InitKey(0x48327203, 0x3948ebea, 0x9a9b9c9e, 0xb3bed89a);

    oMD5[2].InitKey(0x67452301, 0xefcdab89,  0x98badcfe, 0x10325476);
    oMD5[3].InitKey( 0xc8e899e8, 0x9321c28a, 0x438eba12, 0x8cbe0aee);
    break;

  }
  return 1;
}

int GRecord::SetFileName(const TCHAR *szFileNameIn)
{
  _tcscpy(FileName,szFileNameIn);
  return 0;
}

bool GRecord::IncludeRecordInGCalc(const unsigned char *szIn)
{ //returns false if record is not to be included in G record calc (see IGC specs)
  bool bValid;
  TCHAR c1;

  bValid=false;
  c1=szIn[0];
  switch ( c1 )
  {
  case 'L':
    if (memcmp(szIn+1,XCSOAR_IGC_CODE,3) ==0)
      bValid=1; // only include L records made by XCS
    break;

  case 'G':
    break;

  case 'H':
    if ((szIn[1] != 'O') && (szIn[1] != 'P'))
      bValid=1;
    break;

  default:
    bValid=1;
  }
  return bValid;
}

int GRecord::LoadFileToBuffer ()
{ //loads a file into the data buffer
  #define MAX_REC_LENGTH 200



  FILE *inFile = NULL;
  char data[MAX_REC_LENGTH];

  unsigned char udata[MAX_REC_LENGTH];


  inFile = _tfopen(FileName, _T("rb"));
  if (inFile == NULL)
    return 0;

  while(fgets(data, MAX_REC_LENGTH, inFile) ) {
    for (unsigned int i = 0; i <= strlen(data); i++) {
      udata[i]=(unsigned char)data[i];
    }

    AppendRecordToBuffer(udata);
  } // read

  fclose (inFile);
  return 1;
}



int GRecord::AppendGRecordToFile(bool bValid) // writes error if invalid G Record
{
  HANDLE hFile;// = INVALID_HANDLE_VALUE;
  DWORD dwBytesRead;

  hFile = CreateFile(FileName, GENERIC_WRITE, FILE_SHARE_WRITE,
         NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

  if (hFile == NULL)
    return 0;

  TCHAR szDigestBuff[BUFF_LEN];
  TCHAR * szDigest;
  szDigest=szDigestBuff;

  GetDigest(szDigest);

  if (bValid) {

    int iLine; //
    int iNumCharsPerLine;
    iNumCharsPerLine=16;
    static char sDig16[BUFF_LEN];
    sDig16[0]='G';
    for ( iLine = 0; iLine < (128/iNumCharsPerLine); iLine++) {// 0 - 15
      for (int iChar = 0; iChar < iNumCharsPerLine; iChar++) {
        sDig16[iChar+1] = (char)szDigest[iChar + iNumCharsPerLine*iLine];
      }

      sDig16[iNumCharsPerLine+1]='\r';  // +1 is the initial "G"
      sDig16[iNumCharsPerLine+2]='\n';
      sDig16[iNumCharsPerLine+3]=0;

      SetFilePointer(hFile, 0, NULL, FILE_END);
      WriteFile(hFile, sDig16, strlen(sDig16), &dwBytesRead,
        (OVERLAPPED *)NULL);

    }
  }
  else {
    char sMessage[] = "G Record Invalid\r\n";

    SetFilePointer(hFile, 0, NULL, FILE_END);
    WriteFile(hFile, sMessage, strlen(sMessage), &dwBytesRead,
      (OVERLAPPED *)NULL);

  }

  // this is the code from LogpointtoFile

  FlushFileBuffers(hFile);

  CloseHandle(hFile);
  return 0;

}

int GRecord::ReadGRecordFromFile(TCHAR szOutput[])
{// returns in szOutput the G Record from the file referenced by FileName member
  #define MAX_REC_LENGTH 200

  FILE *inFile = NULL;
  TCHAR data[MAX_REC_LENGTH];

  inFile = _tfopen(FileName, _T("r"));

  szOutput[0]='\0';
  if (inFile == NULL) {
    return 0;
  }

  unsigned int iLenDigest=0;
  while(1) {
    if (!fgetws(data, MAX_REC_LENGTH, inFile)) {
      break;
    }

    int bValid;
    TCHAR c1, c2;
    int i;
    i = 0;
    bValid=0;
    c1=data[0];
    if ( c1 == 'G') {
      unsigned int iLenData;
      iLenData = _tcslen(data);
      for (unsigned int i = 1; i < iLenData; i++) { // skip initial 'G'
        c2=data[i];
        if ( ( c2 !='\r' ) && (c2 != '\n') && (c2 != 0) ) {
          szOutput[iLenDigest++]=(TCHAR)c2;
        }
      }
    }
  } // read

  szOutput[iLenDigest] = '\0';
  fclose (inFile);
  return 1;
}


bool GRecord::VerifyGRecordInFile()
{ // assumes FileName member is set
  bool bRetVal = true;

  TCHAR OldGRecordBuff[BUFF_LEN];
  TCHAR * szOldGRecord ;
  szOldGRecord = OldGRecordBuff;

  TCHAR NewGRecordBuff[BUFF_LEN];
  TCHAR * szNewGRecord ;
  szNewGRecord = NewGRecordBuff;

  // Load File into Buffer (assume name is already set)
  LoadFileToBuffer();

  // load Existing Digest "old"
  ReadGRecordFromFile(szOldGRecord);


  // recalculate digest from buffer
  FinalizeBuffer();
  GetDigest(szNewGRecord);

  for (unsigned int i = 0; i < 128; i++) {
    if (szOldGRecord[i] != szNewGRecord[i] ) {
      bRetVal = false;
    }
  }
  return bRetVal;
}

int
GRecord::IsValidIGCChar(char c) //returns 1 if Valid IGC Char
{//                                  else 0
  // ToDo: RLD move this function out of MD5 class
  return MD5::IsValidIGCChar(c);
}

