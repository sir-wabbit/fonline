#ifndef FONLINE_BASE_INI_FILE_HPP_
#define FONLINE_BASE_INI_FILE_HPP_

#include <string>
#include <map>
#include "fo-base.hpp"

namespace fonline { namespace ini {

typedef std::map<std::string, std::string> RecordMap;
typedef std::map<std::string, RecordMap> SectionMap;

FO_BASE_DECL void Flatten(const SectionMap& in, RecordMap& out);
FO_BASE_DECL void UnFlatten(const RecordMap& in, SectionMap& out);

FO_BASE_DECL bool LoadINI(const std::string& path, RecordMap& ini);
FO_BASE_DECL bool SaveINI(const std::string& path, const RecordMap& ini);

template<class T> FO_BASE_DECL T GetValue(const RecordMap& ini, const std::string& key, T defValue = T());
template<> FO_BASE_DECL std::string GetValue(const RecordMap& ini, const std::string& key, std::string defValue);
template<> FO_BASE_DECL int GetValue(const RecordMap& ini, const std::string& key, int defValue);
template<> FO_BASE_DECL bool GetValue(const RecordMap& ini, const std::string& key, bool defValue);

}}; // namespace fonline::ini

#endif /*FONLINE_BASE_INI_FILE_HPP_*/
