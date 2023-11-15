#pragma once
#include"mpvString.hpp"
#include<vector>

namespace mpv
{
	namespace node
	{
		class StringArray
		{
			using ulong = unsigned long long;
		public:
			static inline char** Create(ulong size)
			{
				return new char* [size];
			}

			static inline char** Create(const std::vector<std::string>& value)
			{
				ulong index = 0;
				ulong size = value.size();
				char** result = Create(size);
				for (auto it = value.begin(); it != value.end(); it++)
				{
					index = std::distance(value.begin(), it);
					result[index] = String::Create(*it);
				}

				return result;
			}

			static inline void Delete(char**& value, ulong size)
			{
				for (ulong it = 0; it < size; it++)
					delete[] value[it];
				delete[] value;
				value = nullptr;
			}
		};
	}
}
