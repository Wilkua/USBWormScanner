
#include "stdafx.h"
#include "LogController.h"

using namespace std;

LogController::LogController(void) {
	logFile.open("avbsw.log", ios::out | ios::app);
	// if log file is available and open then begin logging
	if (logFile && logFile.is_open()) {
		loggingOn = true;	// should log actions
		dateTime = GetCurrentDateTime();
		logFile << L"************* Begin logging " << dateTime.c_str() << L" *************" << endl;
	} else {
		loggingOn = false; // can't (or shouldn't) log actions
	} // end if (!logFile && logFile.is_open())
}


LogController::~LogController(void) {
	if (loggingOn) {
		dateTime = GetCurrentDateTime();
		logFile << L"************* End logging " << dateTime.c_str() << L" *************" << endl;
		logFile << endl;
	}

	if (logFile.is_open())
		logFile.close();
}

void LogController::DisableLogging() {
	loggingOn = false;
}

void LogController::EnableLogging() {
	loggingOn = true;
}

/** void LogController::WriteLog(LPWSTR logMsg)
 * Write a message to the log file
 */
void LogController::WriteLog(LPCWSTR logMsg) {
	wchar_t timeBuf[128];
	if (loggingOn) {
		_wstrtime_s(timeBuf, 128);
		logFile << timeBuf << ": " << logMsg << endl;
	}
} // end void LogController::WriteLog(LPWSTR logMsg)

/** wstring GetCurrentDateTime()
 * Gets the current date and time as a wide
 * character string. Formatted as:
 * Day Month Date Hour:Minute:Second Year
 */
wstring LogController::GetCurrentDateTime() {
	wchar_t* dateTime;		// wide character string of date/time
	__time64_t longTime;	// encoded time in UTC time (long integer)
	tm localTime;			// encoded time in local time
	wstring retString;		// for returning the data

	dateTime = new wchar_t[26];					// get dateTime ready
	_time64(&longTime);							// get time of application starting
	_localtime64_s(&localTime, &longTime);		// convert to local time
	_wasctime_s(dateTime, 26, &localTime); 		// convert to ASCII readable time
	dateTime[lstrlenW(dateTime) - 1] = L'\0';	// get rid of the newline at the end

	retString = wstring(dateTime);				// wchar_t* to wstring
	delete[] dateTime;							// delete dynamic memory

	return retString;
} // end wstring GetCurrentDateTime()
