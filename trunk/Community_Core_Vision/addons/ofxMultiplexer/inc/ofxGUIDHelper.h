#ifndef _OFX_GUID_HELPER_
#define _OFX_GUID_HELPER_

#include <sstream>
#include <string>
#include <windows.h>

//Convert an hex string to a number
template<class T> T HexToInt(const std::string &str);
//Convert a number to an hex string
template<class T> std::string ToHex(const T &value);
//Convert a GUID to string representation
std::string GUIDToString(const GUID &guid);
//Convert a string to GUID
GUID StringToGUID(const std::string &str);

#endif//_OFX_GUID_HELPER_