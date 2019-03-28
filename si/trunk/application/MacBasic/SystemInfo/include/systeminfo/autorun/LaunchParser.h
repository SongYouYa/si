#ifndef AUTORUN_LAUNCHPARSER_H
#define AUTORUN_LAUNCHPARSER_H

#include "AutorunStruct.h"
#include <boost/any.hpp>

namespace macbasic {

    class CLaunchParser
    {
    public:
        CLaunchParser();
        ~CLaunchParser();

        bool ParseLauchDaemon(PSIHANDLE plistHandle, AutoRunInfo& lauchDaemonInfo);

    private:
        void NoteBool(std::string keyName, bool bValue, std::wstring& note);
        void NoteStr(std::string keyName, std::string strValue, std::wstring& note);
        void NoteInt(std::string keyName, int64 intv, std::wstring& note);
        bool IsTypeString(boost::any& value);
        bool IsTypeArray(boost::any& value);
        bool IsTypeBool(boost::any& value);
        bool IsTypeDict(boost::any& value);
    };
}

#endif // AUTORUN_LAUNCHPARSER_H
