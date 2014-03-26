#pragma once

#include <string>
#include <map>

#ifdef _WIN32
  #ifdef INIFILE_DLL
    #define INIFILE_API __declspec(dllexport)
  #else
    #define INIFILE_API __declspec(dllimport)
  #endif
#else
  #define INIFILE_API
#endif

namespace IniFile {

typedef std::map<std::string, std::string> RecordMap;
typedef std::map<std::string, RecordMap> SectionMap;

INIFILE_API void Flatten(const SectionMap& in, RecordMap& out);
INIFILE_API void UnFlatten(const RecordMap& in, SectionMap& out);

INIFILE_API bool LoadINI(const std::string& path, RecordMap& ini);
INIFILE_API bool SaveINI(const std::string& path, const RecordMap& ini);

template<class T> T GetValue(const RecordMap& ini, const std::string& key, T defValue = T());
template<> INIFILE_API std::string GetValue(const RecordMap& ini, const std::string& key, std::string defValue);
template<> INIFILE_API int GetValue(const RecordMap& ini, const std::string& key, int defValue);
template<> INIFILE_API bool GetValue(const RecordMap& ini, const std::string& key, bool defValue);
template<> INIFILE_API float GetValue(const RecordMap& ini, const std::string& key, float defValue);
template<> INIFILE_API double GetValue(const RecordMap& ini, const std::string& key, double defValue);

}; // namespace IniFile
