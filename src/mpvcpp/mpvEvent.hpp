#pragma once
#include"mpvcpp/subEvent/mpvStartFileEvent.hpp"
#include"mpvcpp/subEvent/mpvPropertyEvent.hpp"
#include"mpvcpp/subEvent/mpvCommandEvent.hpp"
#include"mpvcpp/subEvent/mpvEndFileEvent.hpp"
#include"mpvcpp/subEvent/mpvClientEvent.hpp"
#include"mpvcpp/subEvent/mpvHookEvent.hpp"
#include"mpvcpp/subEvent/mpvLogEvent.hpp"
#include"mpvcpp/mpvHandle.hpp"

namespace mpv
{
	class Event
	{
		using ulong = unsigned long long;
		using EventID = code::EventID;
		using Error = code::Error;
	public:
		event::StartFileEvent StartFile;
		event::PropertyEvent Property;
		event::CommandEvent Command;
		event::EndFileEvent EndFile;
		event::ClientEvent Client;
		event::HookEvent Hook;
		event::LogEvent Log;
	public:
		Error ErrorCode = Error::Generic;
		EventID ID = EventID::None;
		ulong ReplyData = 0;
	public:
		inline Event() {}
		inline ~Event() {}

		inline Event(const Event& right) { this->assign(right); }
		inline Event(const Handle& handle) { this->receive(handle); }
		inline Event(const mpv_event& right) { this->assign(right); }
		inline Event(Event&& right) noexcept { this->assign(std::move(right)); }

		inline Event& operator=(const Event& right) { return this->assign(right); }
		inline Event& operator=(const Handle& handle) { return this->receive(handle); }
		inline Event& operator=(const mpv_event& right) { return this->assign(right); }
		inline Event& operator=(Event& right) noexcept { return this->assign(std::move(right)); }

		inline Event& receive(const Handle& handle, double timeout = 0.0);
		static inline code::Error ToNode(const Handle& handle, Node& result, double timeout = 0.0);

		inline Event& assign(const Event& right);
		inline Event& assign(Event&& right) noexcept;
		inline Event& assign(const mpv_event& right);
	};

	inline Event& Event::receive(const Handle& handle, double timeout)
	{
		mpv_event data = *mpv_wait_event(handle.Data, timeout);
		this->assign(data);
		return *this;
	}

	inline code::Error Event::ToNode(const Handle& handle, Node& result, double timeout)
	{
		mpv_event data = *mpv_wait_event(handle.Data, timeout);
		mpv_node storage{};

		auto ret = code::toError(mpv_event_to_node(&storage, &data));
		if (ret == Error::Success) result = storage;

		mpv_free_node_contents(&storage);
		return ret;
	}

	inline Event& Event::assign(const Event& right)
	{
		if (&right == this) return *this;

		StartFile = right.StartFile;
		Property = right.Property;
		Command = right.Command;
		EndFile = right.EndFile;
		Client = right.Client;
		Hook = right.Hook;
		Log = right.Log;

		ReplyData = right.ReplyData;
		ErrorCode = right.ErrorCode;
		ID = right.ID;

		return *this;
	}

	inline Event& Event::assign(Event&& right) noexcept
	{
		if (&right == this) return *this;

		StartFile = std::move(right.StartFile);
		Property = std::move(right.Property);
		Command = std::move(right.Command);
		EndFile = std::move(right.EndFile);
		Client = std::move(right.Client);
		Hook = std::move(right.Hook);
		Log = std::move(right.Log);

		ReplyData = right.ReplyData;
		ErrorCode = right.ErrorCode;
		ID = right.ID;

		right.ErrorCode = Error::Generic;
		right.ID = EventID::None;
		right.ReplyData = 0;

		return *this;
	}

	inline Event& Event::assign(const mpv_event& right)
	{
		ID = code::toEventID(right.event_id);
		if (!right.data) return *this;

		switch (ID)
		{
		case EventID::PropertyChanged:
			ReplyData = right.reply_userdata;
			Property = *static_cast<mpv_event_property*>(right.data);
			break;
		case EventID::ClientMessage:
			Client = *static_cast<mpv_event_client_message*>(right.data);
			break;
		case EventID::CommandReply:
			ErrorCode = code::toError(right.error);
			ReplyData = right.reply_userdata;
			Command = *static_cast<mpv_event_command*>(right.data);
			break;
		case EventID::GetProperty:
			ErrorCode = code::toError(right.error);
			ReplyData = right.reply_userdata;
			Property = *static_cast<mpv_event_property*>(right.data);
			break;
		case EventID::SetProperty:
			ErrorCode = code::toError(right.error);
			ReplyData = right.reply_userdata;
			break;
		case EventID::LogMessage:
			Log = *static_cast<mpv_event_log_message*>(right.data);
			break;
		case EventID::StartFile:
			StartFile = *static_cast<mpv_event_start_file*>(right.data);
			break;
		case EventID::EndFile:
			EndFile = *static_cast<mpv_event_end_file*>(right.data);
			break;
		case EventID::Hook:
			ReplyData = right.reply_userdata;
			Hook = *static_cast<mpv_event_hook*>(right.data);
			break;
		}

		return *this;
	}
}
