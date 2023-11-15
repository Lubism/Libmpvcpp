#pragma once
#include<mpv/client.h>

namespace mpv
{
	namespace code
	{
		enum class LogLevel
		{
			None	= MPV_LOG_LEVEL_NONE,
			Fatal	= MPV_LOG_LEVEL_FATAL,
			Error	= MPV_LOG_LEVEL_ERROR,

			Warn	= MPV_LOG_LEVEL_WARN,
			Info	= MPV_LOG_LEVEL_INFO,
			V		= MPV_LOG_LEVEL_V,

			Debug	= MPV_LOG_LEVEL_DEBUG,
			Trace	= MPV_LOG_LEVEL_TRACE
		};

		inline LogLevel toLogLevel(int value)
		{
			if (value < MPV_LOG_LEVEL_NONE || value > MPV_LOG_LEVEL_TRACE)
				return LogLevel::None;
			return static_cast<LogLevel>(value);
		}

		inline mpv_log_level fromLogLevel(const LogLevel& value)
		{
			return static_cast<mpv_log_level>(value);
		}
	}
}
