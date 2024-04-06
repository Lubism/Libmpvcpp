#pragma once
#include<mpv/client.h>

namespace mpv
{
	namespace code
	{
		enum class Format
		{
			None			= MPV_FORMAT_NONE,
			String			= MPV_FORMAT_STRING,
			OSDString		= MPV_FORMAT_OSD_STRING,

			BooleanInt		= MPV_FORMAT_FLAG,
			Int				= MPV_FORMAT_INT64,
			Double			= MPV_FORMAT_DOUBLE,

			Node			= MPV_FORMAT_NODE,
			NodeArray		= MPV_FORMAT_NODE_ARRAY,
			NodeMap			= MPV_FORMAT_NODE_MAP,

			ByteArray		= MPV_FORMAT_BYTE_ARRAY
		};

		inline bool operator==(int left, const Format& right)
		{
			return static_cast<Format>(left) == right;
		}

		inline mpv_format fromFormat(const Format& value)
		{
			return static_cast<mpv_format>(value);
		}

		inline Format toFormat(int value)
		{
			if (value > MPV_FORMAT_BYTE_ARRAY || value < MPV_FORMAT_NONE)
				return Format::None;

			return static_cast<Format>(value);
		}
	}
}
