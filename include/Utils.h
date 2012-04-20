#ifndef __TR_UTILS__
#define __TR_UTILS__

#include "Dependencies.h"

namespace TR
{
	namespace Utils
	{
		static bool stringStartsWith(const std::string &str, const std::string &pattern, bool lowerCase = true);
		static void toLowerCase(std::string& str);

		static bool parseBool(const std::string &value)
		{
			return (TR::Utils::stringStartsWith(value, "true") || 
					  TR::Utils::stringStartsWith(value, "yes") ||
					  TR::Utils::stringStartsWith(value, "1"));
		}

		static TR::Real parseReal(const std::string &value)
		{
			return atof(value.c_str());
		}

		static int parseInt(const std::string &value)
		{
			return atoi(value.c_str());
		}

		bool stringStartsWith(const std::string &str, const std::string &pattern, bool lowerCase)
		{
			size_t thisLen = str.length();
			size_t patternLen = pattern.length();
			if (thisLen < patternLen || patternLen == 0)
				return false;
   
			std::string startOfThis = str.substr(0, patternLen);
			if (lowerCase)
				toLowerCase(startOfThis);
   
			return (startOfThis == pattern);
		}

		void toLowerCase(std::string& str)
		{
			std::transform(
				str.begin(),
				str.end(),
				str.begin(),
				tolower);
		}
	}
}

#endif