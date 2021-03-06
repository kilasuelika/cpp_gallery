#include "pch.h"
#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <string>

using namespace std;

int main(){
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;
	SQLRETURN retcode;

	SQLWCHAR OutConnStr[255];
	SQLSMALLINT OutConnStrLen;

	// Allocate environment handle
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

		// Allocate connection handle
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

				// Connect to data source
				retcode = SQLDriverConnect(
					hdbc,
					0,
					(SQLWCHAR*)L"DSN=mysql;SERVER=127.0.0.1;UID=root;PWD=1234;DRIVER=MariaDB ODBC 3.0 Driver;",
					_countof(L"DSN=mysql;SERVER=127.0.0.1;UID=root;PWD=1234;DRIVER=MariaDB ODBC 3.0 Driver;"),
					OutConnStr,
					255,
					&OutConnStrLen,
					SQL_DRIVER_COMPLETE);

				// Allocate statement handle
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

					// Process data
					retcode = SQLExecDirect(hstmt, (SQLWCHAR*)L"SELECT TEST_STRING, TEST_INTEGER, TEST_FLOAT FROM dbo.testfire", SQL_NTS);

					if (retcode == SQL_SUCCESS) {
						SQLINTEGER  iCount = 1;
						SQLLEN sTestInt, cbTestStr, cbTestInt, cbTestFloat;
						SQLFLOAT dTestFloat;
						SQLCHAR szTestStr[200];
						while (TRUE) {
							retcode = SQLFetch(hstmt);
							if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
								cout << "An error occurred";
							}
							if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

								SQLGetData(hstmt, 1, SQL_C_CHAR, szTestStr, 200, &cbTestStr);
								SQLGetData(hstmt, 2, SQL_C_ULONG, &sTestInt, 0, &cbTestInt);
								SQLGetData(hstmt, 3, SQL_C_DOUBLE, &dTestFloat, 0, &cbTestFloat);

								/* Print the row of data */
								cout << "Row " << iCount << ":" << endl;
								cout << szTestStr << endl;
								cout << sTestInt << endl;
								cout << dTestFloat << endl;
								iCount++;
							}
							else {
								break;
							}
						}
					}
					else {
						cout << "Query execution error." << endl;
					}

					SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
					SQLDisconnect(hdbc);
				}
				else {
					cout << "Connection error" << endl;
				}
				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
		}
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}

	system("pause");
	return 0;
}