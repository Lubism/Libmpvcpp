#pragma once
#include<mpv/client.h>
#include<vector>

namespace mpv
{
	namespace node
	{
		class ByteArray
		{
			using ulong = unsigned long long;
			using byte = mpv_byte_array;
			using uchar = unsigned char;
		public:
			static inline mpv_byte_array* Create(ulong size)
			{
				return new byte{new uchar[size], size};
			}

			static inline mpv_byte_array* Create(const std::vector<uchar>& value)
			{
				byte* result = ByteArray::Create(value.size());
				uchar* data = static_cast<uchar*>(
					result->data);
				ulong index = 0;

				for (auto it = value.begin(); it != value.end(); it++)
				{
					index = std::distance(value.begin(), it);
					data[index] = *it;
				}

				return result;
			}

			static inline void Assign(std::vector<uchar>& left,
				const mpv_byte_array& right)
			{
				const uchar* data = static_cast<const uchar*>(right.data);
				std::vector<uchar>().swap(left);
				left.reserve(right.size);

				for (size_t it = 0; it != right.size; it++)
					left.push_back(data[it]);
			}

			static inline void Delete(byte*& value)
			{
				delete[] value->data;
				value->data = nullptr;

				delete value;
				value = nullptr;
			}
		};
	}
}
