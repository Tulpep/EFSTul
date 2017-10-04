#include <Windows.h>
#include <tchar.h>

#define PATH L"D:\\EFS"

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
	DWORD dirExists;
	BOOL encryptDir, efsDir; ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

	dirExists = GetFileAttributesW(PATH);

	if (dirExists == INVALID_FILE_ATTRIBUTES)
	{
		wprintf(L"Here!");
		ShowError(GetLastError());	

		efsDir = CreateDirectoryW(PATH, NULL);

		if (efsDir)
		{
			wprintf(L"\nDirectory has been created.\n");

		}
		else
		{
			wprintf(L"\nDirectory could not be created, code: ");
			ShowError(GetLastError());
		}

	}
	//Checking againts the FILE_ATTRIBUTE_DIRECTORY bit 
	else if((dirExists & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		wprintf(L"\nDirectory exists.");

		//Let's encrypt!
		encryptDir = EncryptFileW(PATH);

		if (!encryptDir)
		{
			ShowError(GetLastError());
		}
		else
		{
			wprintf(L"\nDirectory has been encrypted using EFS.\n");
		}
		
	}

	getchar();

	return 0;
}