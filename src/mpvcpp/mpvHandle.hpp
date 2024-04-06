#pragma once
#include"code/mpvLogLevel.hpp"
#include"code/mpvEventID.hpp"
#include"code/mpvError.hpp"

namespace mpv
{
	namespace render { class Context; }

	class Handle
	{
		using Error = code::Error;
		using ulong = unsigned long long;
	public:
		enum InitType { None = 0, Create, Init };
	public:
		inline Handle() {}
		inline Handle(InitType state);
		inline ~Handle() { this->destroy(); }

		inline Handle(const Handle&) = delete;
		inline Handle(Handle&& right) noexcept;
		inline Handle& operator=(const Handle&) = delete;
		inline Handle& operator=(Handle&& right) noexcept;

		inline code::Error create();
		inline code::Error initialize();
		inline code::Error create(const Handle& right, const std::string& name);
		inline code::Error weakCreate(const Handle& right, const std::string& name);

		inline code::Error clientID(long long& value) const;
		inline code::Error clientName(std::string& value) const;
		inline code::Error loadConfig(const std::string& filePath) const;
		inline code::Error internalTime(long long& value, bool isNanosecond = false) const;

		inline code::Error wakeup() const;
		template<typename Func, typename Arg>
		inline code::Error setWakeupCallback(Func func, Arg arg) const;

		inline code::Error waitAsyncRequest() const;
		inline code::Error setLogLevel(const code::LogLevel& logLevel) const;
		inline code::Error setEventRequest(const code::EventID& eventID, bool enable = true) const;

		inline code::Error continueHook(ulong hookID) const;
		inline code::Error addHook(ulong replyData, const std::string& name, int priority = 0) const;

		inline operator bool() const { return Data; }
		inline bool isCreated() const { return Data; }
		inline bool operator!() const { return !Data; }
		inline bool isInitialized() const { return InitState; }

		inline void destroy() { mpv_destroy(Data); this->clearState(); }
		inline void terminate() { mpv_terminate_destroy(Data); this->clearState(); }
		static inline ulong APIVersion() { return static_cast<ulong>(mpv_client_api_version()); }
	private:
		friend class render::Context;
		friend class Property;
		friend class Command;
		friend class Event;

		mpv_handle* Data = nullptr;
		bool InitState = false;

		inline void clearState()
		{
			InitState = false;
			Data = nullptr;
		}
	};

	inline Handle::Handle(InitType state)
	{
		switch (state)
		{
		case InitType::Create:
			this->create();
			break;
		case InitType::Init:
			this->create();
			this->initialize();
			break;
		default:
			break;
		}
	}

	inline Handle::Handle(Handle&& right) noexcept
	{
		this->operator=(std::move(right));
	}

	inline Handle& Handle::operator=(Handle&& right) noexcept
	{
		if (&right == this) return *this;
		InitState = right.InitState;
		Data = right.Data;

		right.clearState();
		return *this;
	}

	inline code::Error Handle::create()
	{
		if (Data) return Error::Success;
		Data = mpv_create();

		if (!Data) return Error::NoMemory;
		return Error::Success;
	}
	inline code::Error Handle::initialize()
	{
		if (this->isInitialized()) return Error::Success;
		auto error = code::toError(mpv_initialize(Data));
		if (error != Error::Success)
			return error;

		InitState = true;
		return error;
	}
	inline code::Error Handle::create(const Handle& right,
		const std::string& name)
	{
		if (Data != nullptr) return Error::Success;
		Data = mpv_create_client(right.Data,
			name.data());

		if (!Data) return Error::NoMemory;
		return Error::Success;
	}
	inline code::Error Handle::weakCreate(const Handle& right,
		const std::string& name)
	{
		if (!right.Data) return Error::InvalidParameter;
		if (Data) return Error::Success;
		Data = mpv_create_weak_client(
			right.Data, name.data());

		if (!Data) return Error::NoMemory;
		return Error::Success;
	}

	inline code::Error Handle::clientID(long long& value) const
	{
		if (!Data) return Error::Uninitialized;
		value = mpv_client_id(Data);
		return Error::Success;
	}
	inline code::Error Handle::clientName(std::string& value) const
	{
		if (!Data) return Error::Uninitialized;
		value.assign(mpv_client_name(Data));
		return Error::Success;
	}
	inline code::Error Handle::loadConfig(const std::string& filePath) const
	{
		if (filePath.empty()) return Error::InvalidParameter;
		if (!Data) return Error::Uninitialized;
		return code::toError(mpv_load_config_file(
			Data, filePath.data()));
	}
	inline code::Error Handle::internalTime(long long& value, bool isNanosecond) const
	{
		if (!Data) return Error::Uninitialized;

		if (isNanosecond)
			value = mpv_get_time_ns(Data);
		else
			value = mpv_get_time_us(Data);

		return Error::Success;
	}

	inline code::Error Handle::wakeup() const
	{
		if (!Data) return Error::InvalidParameter;

		mpv_wakeup(Data);
		return Error::Success;
	}
	template<typename Func, typename Arg>
	inline code::Error Handle::setWakeupCallback(Func func, Arg arg) const
	{
		if (!Data) return Error::InvalidParameter;
		auto callback = reinterpret_cast<
			void (*)(void*)>(func);

		mpv_set_wakeup_callback(Data, callback, &arg);
		return Error::Success;
	}

	inline code::Error Handle::waitAsyncRequest() const
	{
		if (!Data) return Error::InvalidParameter;
		mpv_wait_async_requests(Data);
		return Error::Success;
	}
	inline code::Error Handle::setLogLevel(const code::LogLevel& logLevel) const
	{
		if (!Data) return Error::InvalidParameter;
		using Level = code::LogLevel;
		std::string storage = "";

		switch (logLevel)
		{
		case Level::None:	storage = "no";		break;
		case Level::Fatal:	storage = "fatal";	break;
		case Level::Error:	storage = "error";	break;
		case Level::Warn:	storage = "warn";	break;
		case Level::Info:	storage = "info";	break;
		case Level::V:		storage = "v";		break;
		case Level::Debug:	storage = "debug";	break;
		case Level::Trace:	storage = "trace";	break;
		}

		return code::toError(mpv_request_log_messages(
			Data, storage.c_str()));
	}
	inline code::Error Handle::setEventRequest(const code::EventID& eventID,
		bool enable) const
	{
		if (!Data) return Error::InvalidParameter;
		return code::toError(mpv_request_event(
			Data, code::fromEventID(eventID),
			enable ? 1 : 0));
	}

	inline code::Error Handle::continueHook(ulong hookID) const
	{
		if (!Data) return Error::InvalidParameter;
		return code::toError(mpv_hook_continue(
			Data, hookID));
	}
	inline code::Error Handle::addHook(ulong replyData,
		const std::string& name,
		int priority) const
	{
		if (!Data || name.empty()) return Error::InvalidParameter;
		return code::toError(mpv_hook_add(Data,
			replyData, name.c_str(),
			priority));
	}
}
