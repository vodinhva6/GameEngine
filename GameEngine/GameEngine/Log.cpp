#include "Log.h"
#include "GameEngine.h"
#include <Windows.h>

void OutConsole(std::string text)
{
    GameEngine::get()->OutConsole(text);
}

void OutConsole(std::wstring text)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, NULL, 0, NULL, NULL);
    std::string str(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, &str[0], len, NULL, NULL);
    OutConsole(str);
}

void OutConsoleResult(std::string text, int line, std::wstring fileLoca)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, fileLoca.c_str(), -1, NULL, 0, NULL, NULL);
    std::string filestr(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, fileLoca.c_str(), -1, &filestr[0], len, NULL, NULL);
    OutConsole(text);
    OutConsole(" At File :" + filestr);
    OutConsole("Line : " + std::to_string(line));
}

void OutConsoleResult(std::wstring text, int line, std::wstring fileLoca)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, NULL, 0, NULL, NULL);
    std::string str(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, &str[0], len, NULL, NULL);

    len = WideCharToMultiByte(CP_UTF8, 0, fileLoca.c_str(), -1, NULL, 0, NULL, NULL);
    std::string filestr(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, fileLoca.c_str(), -1, &filestr[0], len, NULL, NULL);

    OutConsole(str);
    OutConsole(" At File :" + filestr);
    OutConsole("Line : " + std::to_string(line));
}
