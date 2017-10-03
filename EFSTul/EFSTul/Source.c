#include <Windows.h>
#include <tchar.h>

#define PATH L"D:\\EFS"

VOID ShowError(DWORD errId)
{

}

int wmain(int argc, WCHAR * argv[])
{
	LPCWSTR pFolderPath = L"D:\\EFS\\";
	DWORD dirExists;

	dirExists = GetFileAttributesW(pFolderPath);

	if (dirExists == INVALID_FILE_ATTRIBUTES)
	{
		wprintf(L"Error code: %lu\n", GetLastError());
		

	}
	else if((dirExists & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		wprintf(L"Directory exists.\n");
		
	}

	getchar();

	return 0;
}