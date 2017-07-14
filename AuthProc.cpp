#include "AuthProc.h"
#include "global.h"

void CAuthProc::Handle()
{
    std::string szmethod = get_param("method");
    try {
        std::string json;

        if (szmethod == "LogOut") {
            LogOut(json);
        } else {
            json = "invalid method:" + szmethod;
        }

        print_log(json);
    }
    catch (const char* msg) {
    }
    catch(...) {
    }
}

int CAuthProc::LogOut(std::string & json)
{
    std::string strLoginName;
    strLoginName = get_param("name");

    Json::Value root;
    Json::FastWriter write;

    root["msg"] = " LogOut Success!";
    root["retcode"] = E_SUCCEEDED;
    root["sign"] = "";
    json = write.write(root);
    return 0;
}
