/***************************************************************************
 * Language model
 *
 * Copyright (c) 2013, Randy Hollines
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the distribution.
 * - Neither the name of the Objeck Team nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <unordered_map>
using namespace stdext;
#else
#include <tr1/unordered_map>
#include <pthread.h>
#include <stdint.h>
namespace std {
  using namespace tr1;
}
#endif

using namespace std;

inline wstring IntToString(int v)
{
  wostringstream str;
  str << v;
  return str.str();
}

/****************************
 * TODO: doc
 ****************************/
class Instruction {
 public:
  Instruction() {
  }
  
  ~Instruction() {
  }
};

/****************************
 * Converts a UTF-8 bytes to
 * native a unicode string
 ****************************/
static bool BytesToUnicode(const std::string &in, std::wstring &out) {    
#ifdef _WIN32
  // allocate space
  int wsize = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), -1, NULL, 0);
  if(!wsize) {
    return false;
  }
  wchar_t* buffer = new wchar_t[wsize];

  // convert
  int check = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), -1, buffer, wsize);
  if(!check) {
    delete[] buffer;
    buffer = NULL;
    return false;
  }
  
  // create string
  out.append(buffer, wsize - 1);

  // clean up
  delete[] buffer;
  buffer = NULL;  
#else
  // allocate space
  size_t size = mbstowcs(NULL, in.c_str(), in.size());
  if(size == (size_t)-1) {
    return false;
  }
  wchar_t* buffer = new wchar_t[size + 1];

  // convert
  size_t check = mbstowcs(buffer, in.c_str(), in.size());
  if(check == (size_t)-1) {
    delete[] buffer;
    buffer = NULL;
    return false;
  }
  buffer[size] = L'\0';

  // create string
  out.append(buffer, size);

  // clean up
  delete[] buffer;
  buffer = NULL;
#endif

  return true;
}

static std::wstring BytesToUnicode(const std::string &in) {
  std::wstring out;
  if(BytesToUnicode(in, out)) {
    return out;
  }

  return L"";
}

/****************************
 * Converts UTF-8 bytes to
 * native a unicode character
 ****************************/
static bool BytesToCharacter(const std::string &in, wchar_t &out) {
  std::wstring buffer;
  if(!BytesToUnicode(in, buffer)) {
    return false;
  }
  
  if(buffer.size() != 1) {
    return false;
  }
  
  out = buffer[0];  
  return true;
}

/****************************
 * Converts a native string
 * to UTF-8 bytes
 ****************************/
static bool UnicodeToBytes(const std::wstring &in, std::string &out) {
#ifdef _WIN32
  // allocate space
  int size = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), -1, NULL, 0, NULL, NULL);
  if(!size) {
    return false;
  }
  char* buffer = new char[size];
  
  // convert std::string
  int check = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), -1, buffer, size, NULL, NULL);
  if(!check) {
    delete[] buffer;
    buffer = NULL;
    return false;
  }
  
  // append output
  out.append(buffer, size - 1);

  // clean up
  delete[] buffer;
  buffer = NULL;
#else
  // convert std::string
  size_t size = wcstombs(NULL, in.c_str(), in.size());
  if(size == (size_t)-1) {
    return false;
  }
  char* buffer = new char[size + 1];
  
  wcstombs(buffer, in.c_str(), size);
  if(size == (size_t)-1) {
    delete[] buffer;
    buffer = NULL;
    return false;
  }
  buffer[size] = '\0';
  
  // create std::string      
  out.append(buffer, size);

  // clean up
  delete[] buffer;
  buffer = NULL;
#endif
  
  return true;
}

static std::string UnicodeToBytes(const std::wstring &in) {
  std::string out;
  if(UnicodeToBytes(in, out)) {
    return out;
  }

  return "";
}

/****************************
 * Converts a native character
 * to UTF-8 bytes
 ****************************/
static bool CharacterToBytes(wchar_t in, std::string &out) {
  if(in == L'\0') {
    return true;
  }
  
  wchar_t buffer[2];
  buffer[0] = in;
  buffer[1] = L'\0';

  if(!UnicodeToBytes(buffer, out)) {
    return false;
  }
  
  return true;
}

#endif
