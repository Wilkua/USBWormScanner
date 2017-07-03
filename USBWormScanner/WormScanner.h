#pragma once

#include <vector>
#include <string>

#include "LogController.h"

class WormScanner {
private:
	LogController* logCtrl;
	std::vector<std::wstring> infectionList;
public:
	WormScanner(LogController&);
	~WormScanner(void);
	int run(HINSTANCE, HINSTANCE, int);
	void StopRunningVBS(void);
	bool ScanSystemFolders(void);
	bool ScanSystemRegistry(void);
	void ScanAndCleanSystem(void);
	void ScanAndCleanDrive(std::wstring);
	int GetFileList(LPCWSTR, std::vector<std::wstring>&);
	std::wstring TrimExt(std::wstring);
	void AddInfectionToList(std::wstring);
};

