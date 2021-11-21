#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

int ListDirectoryContents(const wchar_t *sDir)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    wchar_t sPath[2048];

    // Specify a file mask. *.* = We want everything!
    StringCchCopy(szDir, MAX_PATH, argv[1]);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
    wsprintf(sPath, L"%s\\*.*", sDir);

    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        wprintf(L"Path not found: [%s]\n", sDir);
        return 0;
    }

    do
    {
        // Find first file will always return "."
        //     and ".." as the first two directories.
        if (wcscmp(fdFile.cFileName, L".") != 0 && wcscmp(fdFile.cFileName, L"..") != 0)
        {
            // Build up our file path using the passed in
            //   [sDir] and the file/foldername we just found:
            wsprintf(sPath, L"%s\\%s", sDir, fdFile.cFileName);

            // Is the entity a File or Folder?
            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                wprintf(L"Directory: %s\n", sPath);
                ListDirectoryContents(sPath); // Recursion, I love it!
            }
            else
            {
                wprintf(L"File: %s\n", sPath);
            }
        }
    } while (FindNextFile(hFind, &fdFile)); // Find the next file.

    FindClose(hFind); // Always, Always, clean things up!

    return 1;
}

int _tmain(int argc, TCHAR *argv[])
{
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    TCHAR szDir[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;
    argv[1] = "F:\\Projects\\c_labs\\zapper";
    // If the directory is not specified as a command-line argument,
    // print usage.

    // if (argc != 2)
    // {
    //     _tprintf(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
    //     return (-1);
    // }

    // Check that the input path plus 3 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus NULL appended below.

    StringCchLength(argv[1], MAX_PATH, &length_of_arg);

    if (length_of_arg > (MAX_PATH - 3))
    {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }

    _tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

    // Prepare string for use with FindFile functions.  First, copy the
    // string to a buffer, then append '\*' to the directory name.

    StringCchCopy(szDir, MAX_PATH, argv[1]);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.

    hFind = FindFirstFile(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        DisplayErrorBox(TEXT("FindFirstFile"));
        return dwError;
    }

    // List all the files in the directory with some info about them.

    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            _tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            _tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        DisplayErrorBox(TEXT("FindFirstFile"));
    }

    FindClose(hFind);
    return dwError;
}