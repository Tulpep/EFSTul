#include <Windows.h>
#include <tchar.h>
#include <Lmcons.h>
#include <Shlwapi.h>
#include <VersionHelpers.h>

#pragma comment(lib, "Shlwapi.lib")


#define PATH_TO_D L"D:\\EFS"
#define PATH_TO_E L"E:\\EFS"

/* This function uses FormatMessageW() function to get 
the error message from the Windows Error Handler */
VOID ShowError(DWORD errId)
{
	DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				  FORMAT_MESSAGE_FROM_SYSTEM     |
				  FORMAT_MESSAGE_IGNORE_INSERTS;
	
	DWORD langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	LPWSTR errMsg;

	if (!FormatMessageW(flags, NULL, errId, 0, (LPWSTR)&errMsg, 0, NULL))
	{
		wprintf(L"\nCould not get error message, code: %lu\n", GetLastError());
	}

	wprintf(L"%s", errMsg);
	LocalFree(errMsg);
}

VOID CreateEFSFolder(LPWSTR folderPath)
{
	DWORD dirExists;
	BOOL encryptDir, efsDir;

	//GetUserName()
	WCHAR bufferName[UNLEN + 1];
	DWORD sizeOfBuff = UNLEN + 1;

	//PathCchCombine
	LPWSTR pathResult;
	WCHAR pathOut[MAX_PATH];

	
	

	dirExists = GetFileAttributesW(folderPath);


	if (dirExists == INVALID_FILE_ATTRIBUTES)	//Directory does not exist
	{

		ShowError(GetLastError());

		efsDir = CreateDirectoryW(folderPath, NULL);

		if (efsDir)
		{

			wprintf(L"\nEFS directory has been created.\n");

			//Getting user name 
			if (GetUserNameW(bufferName, &sizeOfBuff))
			{
				//Combine D:\EFS with the username: D:\EFS\%UserName%
				pathResult = PathCombineW(pathOut, folderPath, bufferName);


				if (pathResult != NULL)
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
						wprintf(L"\nDirectory has been encrypted using EFS: ");
						ShowError(GetLastError());
					}

				}
				else //If path cannot be combined
				{
					wprintf(L"\nPath could not be combined, error: ");
					ShowError(GetLastError());

				}

			}
			else //If GetUserNameW() fails
			{
				wprintf(L"\nUser name could not be retrieved, error: ");
				ShowError(GetLastError());
				wprintf(L"Code: %lu\n", GetLastError());

			}

		}
		else //If CreateDirectoryW() fails
		{
			wprintf(L"\nDirectory could not be created, code: ");
			ShowError(GetLastError());
		}

	}

	//Checking againts the FILE_ATTRIBUTE_DIRECTORY bit 
	else if ((dirExists & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)	//Directory does exist
	{
		wprintf(L"\n%s directory already exists.\n", folderPath);

		//Getting user name 
		if (GetUserNameW(bufferName, &sizeOfBuff))
		{
			//Combine D:\EFS with the username: D:\EFS\%UserName%
			pathResult = PathCombineW(pathOut, folderPath, bufferName);

			if (pathResult != NULL)
			{
				if (!CreateDirectoryW(pathOut, NULL))
				{
					wprintf(L"\nCould not create user profile directory, error: ");
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
					wprintf(L"\nDirectory has been encrypted using EFS: ");
					ShowError(GetLastError());
				}

			}
			else //If path cannot be combined
			{
				wprintf(L"\nPath could not be combined, error: ");
				ShowError(GetLastError());

			}

		}
		else //If GetUserNameW() fails
		{
			wprintf(L"\nUser name could not be retrieved, error: ");
			ShowError(GetLastError());
			

		}

	}


}

int wmain(int argc, WCHAR * argv[])
{	
	//GetLogicalDrives()
	DWORD getDrives;

	//Checks if the app is running on Windows Server
	if (IsWindowsServer())	
	{
		fwprintf(stderr, L"\nThis tool is not supported on Windows Server versions.\n");
		exit(GetLastError());
	}

	//This function gets all the available logical drives
	getDrives = GetLogicalDrives();
	

	if (getDrives == 0) //If function fails
	{

		wprintf(L"\nCould not get logical drives, error: ");
		ShowError(GetLastError());
	}
	else
	{
		if ((getDrives & 8) && GetDriveTypeW(L"D:\\")==DRIVE_FIXED)	//8==D:
		{
			wprintf(L"\nSelected partition: \"D:\"\n");			
			CreateEFSFolder(PATH_TO_D);

		}
		else if ((getDrives & 12) && GetDriveTypeW(L"E:\\")==DRIVE_FIXED)	//12==E
		{
			wprintf(L"\nSelected partition: \"E:\"\n");
			CreateEFSFolder(PATH_TO_E);

		}
		else
			fwprintf(stderr, L"\nThere are not logical drives available to create the EFS folder.\n");

	}	

	return 0;
}