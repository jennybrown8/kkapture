/* kkapture: intrusive demo video capturing.
 * Copyright (c) 2005-2010 Fabian "ryg/farbrausch" Giesen.
 *
 * This program is free software; you can redistribute and/or modify it under
 * the terms of the Artistic License, Version 2.0beta5, or (at your opinion)
 * any later version; all distributions of this program should contain this
 * license in a file named "LICENSE.txt".
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT UNLESS REQUIRED BY
 * LAW OR AGREED TO IN WRITING WILL ANY COPYRIGHT HOLDER OR CONTRIBUTOR
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "stdafx.h"
#include "x264_videoencoder.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

X264VideoEncoder::X264VideoEncoder(const char *fileName, int _fpsNum, int _fpsDenom, const char *_opts)
: BMPVideoEncoder(fileName)
{
  fpsNum = _fpsNum;
  fpsDenom = _fpsDenom;
  strncpy_s(opts, 256, _opts, _TRUNCATE);
  hProcess = hStream = 0;

  TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
  GetModuleFileName((HINSTANCE)&__ImageBase, x264ExePath, _countof(x264ExePath));
  _tsplitpath(x264ExePath, drive, dir, fname, ext);
  strcat_s(dir, "tools");
  _tmakepath(x264ExePath, drive, dir, _T("x264"), _T("exe"));
}

X264VideoEncoder::~X264VideoEncoder()
{
  if (hStream)
  {
    CloseHandle(hStream);
  }
  if (hProcess)
  {
    Real_WaitForSingleObject(hProcess, INFINITE);
  }
}

void X264VideoEncoder::WriteFrame(const unsigned char *buffer)
{
  DWORD dwDummy;

  if (!xRes || !yRes)
  {
    return;  // resolution not set yet, can't do anything
  }

  // start x264
  if (!hProcess && !hStream)
  {
    char temp[512];
    static const char eol[2] = { 13, 10 };

    // create a SECURITY_ATTRIBUTES structure with inheritance enabled (we need that twice)
    SECURITY_ATTRIBUTES sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    // create log file name and open log file
    _snprintf_s(temp, sizeof(temp)/sizeof(*temp), "%s.x264.log", prefix);
    HANDLE hLogFile = CreateFile(temp, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hErrLog = 0;
    DuplicateHandle(GetCurrentProcess(), hLogFile, GetCurrentProcess(), &hErrLog, 0, TRUE, DUPLICATE_SAME_ACCESS);

    // create the input pipe
    HANDLE hChildInput = 0, hDummy = 0;
    CreatePipe(&hChildInput, &hDummy, &sa, 0);
    // create a non-inheritable copy of the write end and close the other (inheritable) original
    DuplicateHandle(GetCurrentProcess(), hDummy, GetCurrentProcess(), &hStream, 0, FALSE, DUPLICATE_SAME_ACCESS);
    CloseHandle(hDummy);

    // build the x264 command line
    _snprintf_s(temp, sizeof(temp)/sizeof(*temp),
                "\"%s\" --demuxer raw --input-csp bgr --input-res %dx%d --fps %d/%d %s -o \"%s.264\" -",
                x264ExePath, xRes, yRes, fpsNum, fpsDenom, opts, prefix);
    printLog("x264: command line: %s\n", temp);
    WriteFile(hLogFile, (LPCVOID)&temp[0], (DWORD)strlen(temp), &dwDummy, NULL);
    WriteFile(hLogFile, (LPCVOID)&eol[0], 2, &dwDummy, NULL);

    // create the x264 process
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hChildInput;
    si.hStdOutput = hLogFile;
    si.hStdError = hErrLog;
    if (Real_CreateProcessA(NULL, temp, NULL, NULL, TRUE, DETACHED_PROCESS, NULL, NULL, &si, &pi))
    {
      hProcess = pi.hProcess;
    }
    else
    {
      printLog("x264: failed to start encoder (error code 0x%08X)\n", GetLastError());
    }

    // close obsolete handles
    CloseHandle(hChildInput);
    CloseHandle(hLogFile);
    CloseHandle(hErrLog);
  }

  if (hStream && hProcess)
  {
    for (int y = yRes - 1;  y >= 0;  --y)
    {
      WriteFile(hStream, (LPCVOID) &buffer[y * xRes*3], xRes*3, &dwDummy, NULL);
    }
  }

  frame++;
}
