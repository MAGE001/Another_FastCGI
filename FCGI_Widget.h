#ifndef __FCGI_WIDGET_H__
#define __FCGI_WIDGET_H__

#include <string.h>
#include <map>
#include <algorithm>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <sstream>

#include "Utf8Code.h"
#include "global.h"

class FCGI_Printer
{
 public:
    static void print(const char *buf)
    {
        printf("%s", buf);
    }

    static void print(const std::string &str)
    {
        printf("%s", str.c_str());
    }

};

class FCGI_Processor
{
 public:
    typedef std::map<std::string, std::string> store_type;
    store_type & get_query_strings() {
        return qmaps_;
    }

 FCGI_Processor(void) : qmaps_() {
        char *query = getenv("QUERY_STRING");
        if (!query || !(*query)) {
            return;
        }

        char *key = query;
        char *val = strchr(query, '=');

        for ( ; key && val; val=strchr(key, '=')) {
            char *tail;
            int key_len;
            int val_len;

            tail = strchr(++val, '&');
            if (!tail) {
                key_len = val - key - 1;
                if (key_len > 0 && *val) {
                    std::string skey(key, key_len);
                    transform(skey.begin(), skey.end(), skey.begin(), tolower);
                    qmaps_.insert(store_type::value_type( skey, std::string(val) ));
                }
                break;
            }

            key_len = val - key - 1;
            val_len = tail - val;

            if (key_len && val_len) {
                std::string skey(key, key_len);
                transform(skey.begin(), skey.end(), skey.begin(), tolower);
                qmaps_.insert(store_type::value_type(skey, std::string(val, val_len)));
            }
            key = tail + 1;
        }
    }

    std::string get_param(const char *key) {
        store_type::iterator iter;
        iter = qmaps_.find(std::string(key));
        if (iter != qmaps_.end()) {
            return iter->second;
        }
        return std::string("");
    }

    std::string get_param(const std::string &key) {
        store_type::iterator iter;
        iter = qmaps_.find(key);
        if (iter != qmaps_.end()) {
            return iter->second;
        }
        return std::string("");
    }

    void print_reqs(void)
    {
        std::ostringstream resp;
        resp << "Content-Type: text/html; charset=UTF-8\r\n\r\n";
        FCGI_Printer::print(resp.str());
    }

    void print_log(std::string str, bool js = false)
    {
        std::ostringstream resp;
        if (!js) {
            resp << HTML_HEAD;
        } else {
            resp<< HTML_HEAD_JS;
        }

        resp << str;
        FCGI_Printer::print(resp.str());
    }

    void print_txt(std::string &fileName,std::string &str)
    {
        std::ostringstream resp;
        std::string strBrowser;
        char *pbrowser = getenv("HTTP_USER_AGENT");
        if(pbrowser) {
            strBrowser = pbrowser;
        }
        transform(strBrowser.begin(), strBrowser.end(), strBrowser.begin(), ::tolower);

        resp << "contentType:text/plain;\r\n";

        if(strBrowser.find("msie") != std::string::npos) {
            CUtf8Code code;
            std::string strFileName;
            code.Utf8ToUrl(fileName.c_str(),strFileName);
            resp << "Content-Disposition:attachment;filename="<<strFileName<<";\r\n";
        } else if(strBrowser.find("firefox") != std::string::npos) {
            fileName = "\"" + fileName + "\"";
            resp << "Content-Disposition:attachment;filename="<<fileName<<";\r\n";
        } else {
            resp << "Content-Disposition:attachment;filename="<<fileName<<";\r\n";
        }


        resp << "Content-Length: " << str.length() << "\r\n\r\n";
        resp << str;
        FCGI_Printer::print(resp.str());
    }

    void print_log_with_callback(std::string str, std::string callback)
    {
        bool js = !callback.empty();

        std::string resp;
        if (!js) {
            resp += HTML_HEAD;
        } else {
            resp += HTML_HEAD_JS;
        }

        if (!callback.empty()) {
            resp += callback;
            resp += "(";
            resp += str;
            resp += ")";
        } else {
            resp += str;
        }

        FCGI_Printer::print(resp);
    }

    void operator()(void)
    {
        // It should be implemented in subclass.
        return Handle();
    }

    void Handle()
    {
    }

 private:
    FCGI_Processor(const FCGI_Processor&);

 protected:
    store_type qmaps_;
};

template <typename TPROC>
class FCGI_Accepter
{
 public:
    static void dispatch(void)
    {
        while (FCGI_Accept() >= 0) {
            ++_reqs;
            // invoke function object
            TPROC()();
        }
    }

    static long requests(void)
    {
        return _reqs;
    }

 private:
    FCGI_Accepter(void);
    FCGI_Accepter(const FCGI_Accepter&);
    ~FCGI_Accepter(void);

    static long _reqs;
};

template <typename TPROC>
long FCGI_Accepter<TPROC>::_reqs = 0;

/*__FCGI_WIDGET_H__*/

#endif
