#pragma once
#include<mpv/client.h>
#include<string>

namespace mpv
{
	namespace code
	{
		enum class Error
		{
			Success					= MPV_ERROR_SUCCESS,
			EventQueueFull			= MPV_ERROR_EVENT_QUEUE_FULL,
			NoMemory				= MPV_ERROR_NOMEM,

			Uninitialized			= MPV_ERROR_UNINITIALIZED,
			InvalidParameter		= MPV_ERROR_INVALID_PARAMETER,
			InexistOption			= MPV_ERROR_OPTION_NOT_FOUND,

			OptionFormat			= MPV_ERROR_OPTION_FORMAT,
			Option					= MPV_ERROR_OPTION_ERROR,
			InexistProperty			= MPV_ERROR_PROPERTY_NOT_FOUND,

			PropertyFormat			= MPV_ERROR_PROPERTY_FORMAT,
			UnavailableProperty		= MPV_ERROR_PROPERTY_UNAVAILABLE,
			Property				= MPV_ERROR_PROPERTY_ERROR,

			Command					= MPV_ERROR_COMMAND,
			LoadingFailure			= MPV_ERROR_LOADING_FAILED,
			AudioInitFailure		= MPV_ERROR_AO_INIT_FAILED,

			VideoInitFailure		= MPV_ERROR_VO_INIT_FAILED,
			EmptyPlayback			= MPV_ERROR_NOTHING_TO_PLAY,
			UnknownFormat			= MPV_ERROR_UNKNOWN_FORMAT,

			Unsupported				= MPV_ERROR_UNSUPPORTED,
			NotImplemented			= MPV_ERROR_NOT_IMPLEMENTED,
			Generic					= MPV_ERROR_GENERIC
		};

		inline std::string errorDescription(const Error&);
		inline mpv_error fromError(const Error&);
		inline Error toError(int);

		inline std::string errorDescription(const Error& value)
		{
			return mpv_error_string(fromError(value));
		}

		inline bool operator==(int left, const Error& right)
		{
			return static_cast<Error>(left) == right;
		}

		inline mpv_error fromError(const Error& value)
		{
			return static_cast<mpv_error>(value);
		}

		inline Error toError(int value)
		{
			if (value > MPV_ERROR_SUCCESS || value < MPV_ERROR_GENERIC)
				return Error::Generic;
			return static_cast<Error>(value);
		}
	}
}
