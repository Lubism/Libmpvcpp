#pragma once
#include<string>

namespace mpv
{
	namespace node
	{
		class String
		{
			using ulong = unsigned long long;
		public:
			static inline char* Create(ulong size)
			{
				char* result = new char[size];
				result[size - 1] = '\0';
				return result;
			}

			static inline char* Create(const std::string& value)
			{
				ulong capacity = value.size() + 1;
				char* result = String::Create(
					capacity);

				memcpy(result, value.c_str(), --capacity);
				result[capacity] = '\0';
				return result;
			}

			static inline void Delete(char*& value)
			{
				delete[] value;
				value = nullptr;
			}
		};
	}
}
