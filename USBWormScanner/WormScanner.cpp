
#include "stdafx.h"
#include "WormScanner.h"
#include "LogController.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

WormScanner::WormScanner(LogController& lc)
{
	logCtrl = &lc;
}

WormScanner::~WormScanner(void)
{
}

/*
Run the application
*/
int WormScanner::run(HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow)
{
	return 0;
} // end void WormScanner::run()

/*
Stops any running VBS scripts by terminating the wscript.exe process.
*/
void WormScanner::StopRunningVBS()
{
	wstring processName = L"wscript.exe";
	HANDLE hSnapShot;
	PROCESSENTRY32 pEntry;
	BOOL hRes;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	pEntry.dwSize = sizeof(pEntry);
	hRes = Process32First(hSnapShot, &pEntry);

	while (hRes)
	{
		if (lstrcmpW(pEntry.szExeFile, processName.c_str()) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
				logCtrl->WriteLog(L"Process wscript.exe terminated");
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
} // end void WormScanner::StopRunningVBS()

/*
Scans the common system folders for the virus software and removes it.
*/
bool WormScanner::ScanSystemFolders()
{
	vector<wstring> fileList;	// listing of files in directories
	LPWSTR wszPath;				// wide character pointer to file path strings
	wstring searchString;		// file path with search info
	wstring fullFilePath;		// full path to .vbs file to delete
	HRESULT hResult;			// used for results from various functions
	BOOL success = false;		// file delete success
	wstring logMsg;				// prepared log messages

	// First scans through CommonStartup folder
	hResult = SHGetKnownFolderPath(FOLDERID_CommonStartup, 0, NULL, &wszPath);
	searchString = wstring(wszPath);
	searchString.append(L"\\*.vbs");

	if (hResult == S_OK)
	{
		// get list of files
		GetFileList(searchString.c_str(), fileList);

		if (!fileList.empty())
		{
			for (vector<wstring>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
			{
				fullFilePath = wstring(wszPath);
				fullFilePath.append(L"\\");
				fullFilePath.append(it->c_str());

				SetFileAttributes(fullFilePath.c_str(), FILE_ATTRIBUTE_NORMAL);
				success = DeleteFile(fullFilePath.c_str());

				AddInfectionToList(it->c_str());

				if (success)
				{
					logMsg = L"Deleted file ";
					logMsg.append(fullFilePath);
					logCtrl->WriteLog(logMsg.c_str());
				}
				else
				{
					logMsg = L"Could not delete file ";
					logMsg.append(fullFilePath);
					logCtrl->WriteLog(logMsg.c_str());
				}
			} // end for (vector<wstring>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
		}
		else
		{
			logMsg = L"No infections found in ";
			logMsg.append(wszPath);
			logCtrl->WriteLog(logMsg.c_str());
		}
	}
	else
	{
		logMsg = L"Error: Common startup folder not available.";
		logCtrl->WriteLog(logMsg.c_str());
	} // end if (hResult == S_OK)

	// clear file list for next folder
	fileList.clear();

	// Now scan user startup folder
	hResult = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &wszPath);
	searchString = wstring(wszPath);
	searchString.append(L"\\*.vbs");

	if (hResult == S_OK)
	{
		// get list of files
		GetFileList(searchString.c_str(), fileList);

		if (!fileList.empty())
		{
			for (vector<wstring>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
			{
				fullFilePath = wstring(wszPath);
				fullFilePath.append(L"\\");
				fullFilePath.append(it->c_str());

				SetFileAttributes(fullFilePath.c_str(), FILE_ATTRIBUTE_NORMAL);
				success = DeleteFile(fullFilePath.c_str());

				AddInfectionToList(it->c_str());

				if (success)
				{
					logMsg = L"Deleted file ";
					logMsg.append(fullFilePath);
					logCtrl->WriteLog(logMsg.c_str());
				}
				else
				{
					logMsg = L"Could not delete file ";
					logMsg.append(fullFilePath);
					logCtrl->WriteLog(logMsg.c_str());
				}
			} // end for (vector<wstring>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
		}
		else
		{
			logMsg = L"No infections found in ";
			logMsg.append(wszPath);
			logCtrl->WriteLog(logMsg.c_str());
		}
	}
	else
	{
		logMsg = L"Error: User startup folder not available.";
		logCtrl->WriteLog(logMsg.c_str());
	} // end if (hResult == S_OK)

	// clear file list for next folder
	fileList.clear();

	//hResult = SHGetKnownFolderPath(FOLDERID_ProgramData, KF_FLAG_DEFAULT_PATH, NULL, &wszPath);
	hResult = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &wszPath);
	searchString = wstring(wszPath);
	searchString.append(L"\\*.vbs");

	if (hResult == S_OK)
	{
		// get list of files
		GetFileList(searchString.c_str(), fileList);

		if (!fileList.empty())
		{
			for (vector<wstring>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
			{
				fullFilePath = wstring(wszPath);
				fullFilePath.append(L"\\");
				fullFilePath.append(it->c_str());

				SetFileAttributes(fullFilePath.c_str(), FILE_ATTRIBUTE_NORMAL);
				success = DeleteFile(fullFilePath.c_str());

				AddInfectionToList(it->c_str());

				if (success)
				{
					logMsg = L"Deleted file ";
					logMsg.append(fullFilePath);
					logCtrl->WriteLog(logMsg.c_str());
				}
				else
				{
					logMsg = L"Could not delete file ";
					logMsg.append(fullFilePath);
					logCtrl->WriteLog(logMsg.c_str());
				}
			} // end for (vector<wstring>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
		}
		else
		{
			logMsg = L"No infections found in ";
			logMsg.append(wszPath);
			logCtrl->WriteLog(logMsg.c_str());
		}
	}
	else
	{
		logMsg = L"Error: Roaming app data folder not available.";
		logCtrl->WriteLog(logMsg.c_str());
	} // end if (hResult == S_OK)

	return true;
} // end bool ScanSystemFolders(wfstream &logFile, wstring infectionName)

/*
Scans the system's registry for keys with the VBS virus on them.
*/
bool WormScanner::ScanSystemRegistry()
{
	HKEY hKey;			// structure for key data
	LONG lRes;			// result of operations
	wstring logMsg;		// logging stuff

	// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\run\ (vbsfilename)
	// HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\run\ (vbsfilename)

	if (infectionList.empty())
	{
		logCtrl->WriteLog(L"No infections to scan for in registry.");
	}

	for (vector<wstring>::const_iterator it = infectionList.begin(); it != infectionList.end(); it++)
	{
		lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WOW64_64KEY | KEY_READ | KEY_WRITE | KEY_EXECUTE, &hKey);
		if (lRes == S_OK)
		{
			lRes = RegDeleteValueW(hKey, TrimExt(*it).c_str());

			if (lRes == S_OK)
			{
				logMsg = L"Deleted key HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\";
				logMsg.append(TrimExt(*it).c_str());
				logCtrl->WriteLog(logMsg.c_str());
			}
			else if (lRes == ERROR_ACCESS_DENIED)
			{
				logMsg = L"Access denied to key HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\";
				logMsg.append(TrimExt(*it).c_str());
				logCtrl->WriteLog(logMsg.c_str());
			}
			else if (lRes == ERROR_FILE_NOT_FOUND)
			{
				logMsg = L"No registry key HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\windows\\CurrentVersion\\Run\\";
				logMsg.append(TrimExt(*it).c_str());
				logCtrl->WriteLog(logMsg.c_str());
			}
			else
			{
				logMsg = L"Coult not delete key HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\";
				logMsg.append(TrimExt(*it).c_str());
				logCtrl->WriteLog(logMsg.c_str());
			}
		}
		else if (lRes == ERROR_ACCESS_DENIED)
		{
			logMsg = L"Access denied to tree HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
			logCtrl->WriteLog(logMsg.c_str());
		}
		else if (lRes == ERROR_FILE_NOT_FOUND)
		{
			logMsg = L"No registry tree HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
			logCtrl->WriteLog(logMsg.c_str());
		}
		else
		{
			logMsg = L"Error opening registry tree HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
			logCtrl->WriteLog(logMsg.c_str());
		}

		RegCloseKey(hKey);

		lRes = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ | KEY_WRITE | KEY_EXECUTE, &hKey);
		if (lRes == S_OK)
		{
			lRes = RegDeleteKeyW(hKey, TrimExt(*it).c_str());

			if (lRes == S_OK)
			{
				logMsg = L"Deleted key HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
				logMsg.append(TrimExt(*it).c_str());
				logCtrl->WriteLog(logMsg.c_str());
			}
			else if (lRes == ERROR_ACCESS_DENIED)
			{
				logMsg = L"Access denied to key HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
				logMsg.append(TrimExt(*it).c_str());
				logCtrl->WriteLog(logMsg.c_str());
			}
			else if (lRes == ERROR_FILE_NOT_FOUND)
			{
				logMsg = L"No registry key HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
				logMsg.append(TrimExt(*it).c_str());
				logCtrl->WriteLog(logMsg.c_str());
			}
			else
			{
				logMsg = L"Could not delete HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
				logMsg.append(TrimExt(*it).c_str());
				logCtrl->WriteLog(logMsg.c_str());
			}
		}
		else if (lRes == ERROR_ACCESS_DENIED)
		{
			logMsg = L"Access denied to tree HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
			logCtrl->WriteLog(logMsg.c_str());
		}
		else if (lRes == ERROR_FILE_NOT_FOUND)
		{
			logMsg = L"No registry tree HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
			logCtrl->WriteLog(logMsg.c_str());
		}
		else
		{
			logMsg = L"Error opening registry tree HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
			logCtrl->WriteLog(logMsg.c_str());
		}

		RegCloseKey(hKey);

	} // end for (vector<wstring>::const_iterator it = infectionList.begin(); it != infectionList.end(); it++)

	// RegOpenKeyExW()
	return true;
} // end bool ScanSystemRegistry(wfstream&)

/*
Wrapper function to call both the file and registry scanners.
*/
void WormScanner::ScanAndCleanSystem()
{
	ScanSystemFolders();
	ScanSystemRegistry();
} // end void WormScanner::ScanAndCleanSystem()

/*
Scans for and removes .vbs and .lnk files in the root folder of the drive.
*/
void WormScanner::ScanAndCleanDrive(wstring driveLetter)
{
	vector<wstring> fileList;	// list of files to look through
	wstring scanFilter;			// path to look in
	wstring logMsg;

	// first look for .vbs infections
	scanFilter = driveLetter;
	scanFilter.append(L"*.vbs");
	GetFileList(scanFilter.c_str(), fileList);

	for (vector<wstring>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
	{
		DeleteFileW(it->c_str());
		logMsg = L"Deleted file ";
		logMsg.append(driveLetter);
		logMsg.append(*it);
		logCtrl->WriteLog(logMsg.c_str());
	}

	// now look for .lnk files

} // end void WormScanner::ScanAndCleanDrive(wstring driveLetter)

/*Gets a list of files in the directory specified in searchKey.
  searchKey can also contain wild cards for searching through
  directories. Example: "C:\\Users\\Bob\\Documents\\*.doc"
  will return all files in Bob's "Documents" folder that end with ".doc".
*/
int WormScanner::GetFileList(LPCWSTR searchKey, vector<wstring>& list)
{
	WIN32_FIND_DATA fd;		// data that is returned by the function
	HANDLE h;				// file handle

	h = FindFirstFile(searchKey, &fd);

	if (h == INVALID_HANDLE_VALUE)
	{
		return 0; // no files found
	}

	// loop through all the files and add them
	do
	{
		list.push_back(fd.cFileName);
	} while (FindNextFile(h, &fd) != FALSE);
	return list.size();
} // end int GetFileList(LPCWSTR searchKey, vector<wstring> &list)

/** wstring WormScanner::TrimExt(wstring fileName)
 * Removes the file extension from the end of a
 * file name.
 */
wstring WormScanner::TrimExt(wstring fileName)
{
	wstring retString = fileName;		// return string
	int count = 0;
	unsigned int c;

	for (c = 0; c < fileName.size(); c++)
	{
		if (fileName[c] == L'.') break;
		count++;
	}
	retString[count] = L'\0';

	return retString;
} // end wstring WormScanner::TrimExt(wstring fileName)

/** void WormScanner::AddInfectionToList(vector<wstring>&)
 * search for infection already in list
 * if not -> add to list
*/
void WormScanner::AddInfectionToList(wstring infectionName)
{
	bool exists = false;

	// go through list of items
	for (vector<wstring>::const_iterator it = infectionList.begin(); it != infectionList.end(); it++)
	{
		if (lstrcmpW(it->c_str(), infectionName.c_str()) == 0)
			exists = true;
	} // end for (vector<wstring>::const_iterator it = infectionList.begin(); it != infectionList.end(); it++) {

	if (!exists)
		infectionList.push_back(infectionName);
} // end void WormScanner::AddInfectionToList(wstring infectionName)