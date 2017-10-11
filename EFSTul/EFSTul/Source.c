#include <Windows.h>
#include <tchar.h>
#include <Lmcons.h>
#include <PathCch.h>
#include <VersionHelpers.h>


#pragma comment(lib, "Pathcch.lib")

#define PATH_TO_D L"D:\\EFS"
#define PATH_TO_E L"E:\\EFS"

VOID ShowError(DWORD errId)
{
	DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD langId = LANG_USER_DEFAULT;
	LPWSTR errMsg;

	if (!FormatMessageW(flags, NULL, errId, langId, (LPWSTR)&errMsg, 0, NULL))
	{
		wprintf(L"Error getting the message. Code: %lu\n", GetLastError());
	}

	wprintf(L"\n%s", errMsg);
	LocalFree(errMsg);
}

int wmain(int argc, WCHAR * argv[])
{
	DWORD dirDExists, dirEExists;
	BOOL encryptDir, efsDir;

	//GetUserName()
	WCHAR bufferName[UNLEN + 1];
	DWORD sizeOfBuff = lstrlenW(bufferName);

	//PathCchCombine
	HRESULT pathResult;
	WCHAR pathOut[MAX_PATH];
	SIZE_T sizePathOut = MAX_PATH;	

	if (IsWindowsServer())	//We don't support this tool on Windows Server
	{
		fwprintf(stderr, L"\nThis tool is not supported on Windows Server versions.\n");
		exit(GetLastError());
	}

	dirDExists = GetFileAttributesW(PATH_TO_D);
	dirEExists = GetFileAttributesW(PATH_TO_E);
	

	if (dirDExists == INVALID_FILE_ATTRIBUTES)	//Directory does not exist
	{

		ShowError(GetLastError());	

		efsDir = CreateDirectoryW(PATH_TO_D, NULL);

		if (efsDir)
		{

			wprintf(L"\nEFS directory has been created.\n");

			//Getting user name 
			if (GetUserNameW(bufferName, &sizeOfBuff))
			{
				//Combine D:\EFS with the username: D:\EFS\%UserName%
				pathResult = PathCchCombine(pathOut, sizePathOut, PATH_TO_D, bufferName);

				if (pathResult == S_OK)
				{
					if (!CreateDirectoryW(pathOut, NULL))
					{
						ShowError(GetLastError());
						exit(GetLastError());
					}

					//Let's encrypt!
					encryptDir = EncryptFileW(pathOut);

					if (!encryptDir)
					{
						wprintf(L"\nCould not encrypt folder, code: ");
						ShowError(GetLastError());
					}
					else
					{
						wprintf(L"\nDirectory has been encrypted using EFS.\n");
					}

				}
				else //If path cannot be combined
				{
					wprintf(L"Path could not be combined, error: ");
					ShowError(GetLastError());

				}

			}
			else //If GetUserNameW() fails
			{
				wprintf(L"User name could not be retrieved, error: ");
				ShowError(GetLastError());

			}			

		}
		else //If CreateDirectoryW() fails
		{
			wprintf(L"\nDirectory could not be created, code: ");
			ShowError(GetLastError());
		}

	}
	//Checking againts the FILE_ATTRIBUTE_DIRECTORY bit 
	else if((dirDExists & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		wprintf(L"\nDirectory exists.");

		//Getting user name 
		if (GetUserNameW(bufferName, &sizeOfBuff))
		{
			//Combine D:\EFS with the username: D:\EFS\%UserName%
			pathResult = PathCchCombine(pathOut, sizePathOut, PATH_TO_D, bufferName);

			if (pathResult == S_OK)
			{
				if (!CreateDirectoryW(pathOut, NULL))
				{
					ShowError(GetLastError());
					exit(GetLastError());
				}

				//Let's encrypt!
				encryptDir = EncryptFileW(pathOut);

				if (!encryptDir) //If encryption fails
				{
					wprintf(L"\nCould not encrypt folder, code: ");
					ShowError(GetLastError());
				}
				else
				{
					wprintf(L"\nDirectory has been encrypted using EFS.\n");
				}

			}
			else //If path cannot be combined
			{
				wprintf(L"Path could not be combined, error: ");
				ShowError(GetLastError());

			}

		}
		else //If GetUserNameW() fails
		{
			wprintf(L"User name could not be retrieved, error: ");
			ShowError(GetLastError());

		}
		
	}

	return 0;
}