#ifndef AuthProc_H__
#define AuthProc_H__

#include <json/json.h>
#include "FCGI_Widget.h"

class CAuthProc	: public  FCGI_Processor
{
 public:
    CAuthProc(){}
    ~CAuthProc(){}

    void operator()(void)	{Handle();}
    void Handle();

 public:
    int	LogIn(std::string& json);
    int	LogOut(std::string& json);
};

#endif // AuthProc_H__
