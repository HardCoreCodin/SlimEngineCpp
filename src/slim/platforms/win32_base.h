#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../core/base.h"

#ifndef NDEBUG
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <new.h>

void DisplayError(LPTSTR lpszFunction) {
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    unsigned int last_error = GetLastError();

    FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, last_error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf, 0, nullptr);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));

    if (FAILED( StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                                TEXT("%s failed with error code %d as follows:\n%s"), lpszFunction, last_error, lpMsgBuf)))
        printf("FATAL ERROR: Unable to output error code.\n");

    _tprintf((LPTSTR)"ERROR: %s\n", (LPCTSTR)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}
#endif

void win32_closeFile(void *handle) {
    CloseHandle(handle);
}

void* win32_openFileForReading(const char* path) {
    HANDLE handle = CreateFileA(path,           // file to open
                                GENERIC_READ,          // open for reading
                                FILE_SHARE_READ,       // share for reading
                                nullptr,                  // default security
                                OPEN_EXISTING,         // existing file only
                                FILE_ATTRIBUTE_NORMAL, // normal file
                                nullptr);                 // no attr. template
#ifndef NDEBUG
    if (handle == INVALID_HANDLE_VALUE) {
        DisplayError((LPTSTR)"CreateFile");
        _tprintf((LPTSTR)"Terminal failure: unable to open file \"%s\" for read.\n", path);
        return nullptr;
    }
#endif
    return handle;
}

void* win32_openFileForWriting(const char* path) {
    HANDLE handle = CreateFileA(path,           // file to open
                                GENERIC_WRITE,          // open for writing
                                0,                      // do not share
                                nullptr,                   // default security
                                OPEN_ALWAYS,            // create new or open existing
                                FILE_ATTRIBUTE_NORMAL,  // normal file
                                nullptr);
#ifndef NDEBUG
    if (handle == INVALID_HANDLE_VALUE) {
        DisplayError((LPTSTR)"CreateFile");
        _tprintf((LPTSTR)"Terminal failure: unable to open file \"%s\" for write.\n", path);
        return nullptr;
    }
#endif
    return handle;
}

bool win32_readFromFile(LPVOID out, DWORD size, HANDLE handle) {
    DWORD bytes_read = 0;
    BOOL result = ReadFile(handle, out, size, &bytes_read, nullptr);
#ifndef NDEBUG
    if (result == FALSE) {
        DisplayError((LPTSTR)"ReadFile");
        printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", (unsigned int)GetLastError());
        CloseHandle(handle);
    }
#endif
    return result != FALSE;
}

bool win32_writeToFile(LPVOID out, DWORD size, HANDLE handle) {
    DWORD bytes_written = 0;
    BOOL result = WriteFile(handle, out, size, &bytes_written, nullptr);
#ifndef NDEBUG
    if (result == FALSE) {
        DisplayError((LPTSTR)"WriteFile");
        printf("Terminal failure: Unable to write to file.\n GetLastError=%08x\n", (unsigned int)GetLastError());
        CloseHandle(handle);
    }
#endif
    return result != FALSE;
}


HWND window_handle;
LARGE_INTEGER performance_counter;

void os::setWindowTitle(char* str) {
    window::title = str;
    SetWindowTextA(window_handle, str);
}

void os::setCursorVisibility(bool on) {
    ShowCursor(on);
}

void os::setWindowCapture(bool on) {
    if (on) SetCapture(window_handle);
    else ReleaseCapture();
}

u64 timers::getTicks() {
    QueryPerformanceCounter(&performance_counter);
    return (u64)performance_counter.QuadPart;
}

void* os::getMemory(u64 size, u64 base) {
    return VirtualAlloc((LPVOID)base, (SIZE_T)size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

void os::closeFile(void *handle) { return win32_closeFile(handle); }
void* os::openFileForReading(const char* path) { return win32_openFileForReading(path); }
void* os::openFileForWriting(const char* path) { return win32_openFileForWriting(path); }
bool os::readFromFile(LPVOID out, DWORD size, HANDLE handle) { return win32_readFromFile(out, size, handle); }
bool os::writeToFile(LPVOID out, DWORD size, HANDLE handle) { return win32_writeToFile(out, size, handle); }