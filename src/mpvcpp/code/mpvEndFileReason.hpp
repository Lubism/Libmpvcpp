#pragma once
#include<mpv/client.h>

namespace mpv
{
	namespace code
	{
		enum class EndFileReason
		{
			EndOfFile	= MPV_END_FILE_REASON_EOF,
			Stopped		= MPV_END_FILE_REASON_STOP,
			Quit		= MPV_END_FILE_REASON_QUIT,
			Error		= MPV_END_FILE_REASON_ERROR,
			Redirection	= MPV_END_FILE_REASON_REDIRECT
		};

		inline mpv_end_file_reason fromEndFileReason(const EndFileReason& value)
		{
			return static_cast<mpv_end_file_reason>(value);
		}

		inline bool operator==(int left, const EndFileReason& right)
		{
			return static_cast<EndFileReason>(left) == right;
		}

		inline EndFileReason toEndFileReason(int value)
		{
			if (value > MPV_END_FILE_REASON_REDIRECT || value < MPV_END_FILE_REASON_EOF)
				return EndFileReason::Error;
			return static_cast<EndFileReason>(value);
		}
	}
}
