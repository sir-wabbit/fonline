#include "IniFile.hpp"

#include <cstdlib>

#include <iostream>
#include <fstream>

namespace {

using namespace IniFile;

void WriteSection(std::ofstream& ofs, const std::string& section) {
  ofs << '[' << section << ']' << std::endl;
}

void WriteRecord(std::ofstream& ofs, const std::string& key, const std::string& value) {
  ofs << key << '=' << value << std::endl;
}

void WriteRecords(std::ofstream& ofs, const RecordMap& records) {
  for (RecordMap::const_iterator i = records.begin(); i != records.end(); ++i) {
    WriteRecord(ofs, i->first, i->second);
  }
}

}; // anonymous namespace

namespace IniFile {

void Flatten(const SectionMap& in, RecordMap& out) {
  out.clear();

  for (SectionMap::const_iterator i = in.begin(); i != in.end(); ++i) {
    std::string section = i->first;
    const RecordMap& records = i->second;
    
    for (RecordMap::const_iterator i = records.begin(); i != records.end(); ++i) {
      std::string key = i->first;
      std::string value = i->second;
      
      if (section != "") {
        key = section + "." + key;
      }
      
      out[key] = value;
    }
  }
}

void UnFlatten(const RecordMap& in, SectionMap& out) {
  out.clear();
  
  for (RecordMap::const_iterator i = in.begin(); i != in.end(); ++i) {
    std::string key = i->first;
    std::string value = i->second;
    std::string section = "";

    // extract section if any
    size_t lastDot = key.find_last_of(".");
    if (lastDot != key.npos) {
      section = key.substr(0, lastDot);
      key = key.substr(lastDot + 1);
    }

    // make sure that there is such an entry 
    if (out.find(section) == out.end()) {
      out[section] = RecordMap();
    }

    out[section][key] = value;
  }
}

// XXX[31.7.2012 alex]: error handling
bool LoadINI(const std::string& path, std::map<std::string, std::string>& ini) {
  std::ifstream is(path.c_str());
	if (!is.is_open()) return false;

  ini.clear();

	std::string line;
  std::string currentSection = "";
  
  while (std::getline(is, line)) {
    size_t textStart = line.find_first_not_of(" \t\r\n", 0);
    
    // skip empty lines
    if (textStart == line.npos) {
      continue;
    }
    // skip comments
    if (line[textStart] == '#' || line[textStart] == ';') {
      continue;
    }
    
    // section
    if (line[textStart] == '[') {
      size_t sectionEnd = line.find(']', textStart);
      
      // invalid section
      if (sectionEnd == line.npos) {
        return false;
      }
      
      // XXX[31.7.2012 alex]: syntax checks?
      currentSection = line.substr(textStart + 1, sectionEnd - textStart - 1);
    }
    
    // value
    size_t equalitySign = line.find('=', textStart);
    if (equalitySign != textStart && equalitySign != line.npos) {
      std::string key = line.substr(textStart, equalitySign - textStart);
      std::string value = line.substr(equalitySign + 1);
      
      // add session prefix
      if (currentSection != "") {
        key = currentSection + "." + key;
      }
      
      ini[key] = value;
    } else {
      // unknown field
      continue;
    }
  }

  return true;
}

bool SaveINI(const std::string& path, const std::map<std::string, std::string>& ini) {
  SectionMap sections;
  UnFlatten(ini, sections);
  
  std::ofstream os(path.c_str());
  if (!os.is_open()) return false;
  
  // first, write all entries without section
  if (sections.find("") != sections.end()) {
    WriteRecords(os, sections[""]);
    sections.erase("");
  }
  
  for (SectionMap::const_iterator i = sections.begin(); i != sections.end(); ++i) {  
    WriteSection(os, i->first);
    WriteRecords(os, i->second);
  }

  return true;
}

template<> std::string GetValue(const RecordMap& ini, const std::string& key, std::string defValue) {
  RecordMap::const_iterator it = ini.find(key);
  if (it != ini.end()) {
    return it->second;
  }
  
  return defValue;
}
template<> int GetValue(const RecordMap& ini, const std::string& key, int defValue) {
  RecordMap::const_iterator it = ini.find(key);
  if (it != ini.end()) {
    // XXX[31.7.2012 alex]: malformed int?
    return atoi(it->second.c_str());
  }
  
  return defValue;
}
template<> bool GetValue(const RecordMap& ini, const std::string& key, bool defValue) {
  RecordMap::const_iterator it = ini.find(key);
  if (it != ini.end()) {
    std::string value = it->second;
    return (value == "1" || value == "yes" || value == "true");
  }
  return defValue;
}
template<> float GetValue(const RecordMap& ini, const std::string& key, float defValue) {
  RecordMap::const_iterator it = ini.find(key);
  if (it != ini.end()) {
    // XXX[7.09.2012 xairy]: malformed float?
    return static_cast<float>(atof(it->second.c_str()));
  }
  
  return defValue;
}
template<> double GetValue(const RecordMap& ini, const std::string& key, double defValue) {
  RecordMap::const_iterator it = ini.find(key);
  if (it != ini.end()) {
    // XXX[7.09.2012 xairy]: malformed double?
    return atof(it->second.c_str());
  }
  
  return defValue;
}

}; // namespace IniFile
