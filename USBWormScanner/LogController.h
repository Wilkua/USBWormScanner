#pragma once

#include <Windows.h>
#include <fstream>

class LogController {
private:
	std::wfstream logFile;
	std::wstring dateTime;
	bool loggingOn;
	std::wstring GetCurrentDateTime(void);
public:
	LogController(void);
	~LogController(void);
	void DisableLogging(void);
	void EnableLogging(void);
	void WriteLog(LPCWSTR);
};
