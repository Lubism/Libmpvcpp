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
		event::StartFileEvent StartFileEvent;
		event::PropertyEvent PropertyEvent;
		event::CommandEvent CommandEvent;
		event::EndFileEvent EndFileEvent;
		event::ClientEvent ClientEvent;
		event::HookEvent HookEvent;
		event::LogEvent LogEvent;
	public:
		Error ErrorCode = Error::Generic;
		EventID ID = EventID::None;
		ulong ReplyData = 0;
	public:
		inline Event() {}
		inline ~Event() {}

		inline Event(const Event& right) { this->assign(right); }
		inline Event(const mpv_event& right) { this->assign(right); }
		inline Event(Event&& right) noexcept { this->assign(std::move(right)); }

		inline Event& operator=(const Event& right) { return this->assign(right); }
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

		StartFileEvent = right.StartFileEvent;
		PropertyEvent = right.PropertyEvent;
		CommandEvent = right.CommandEvent;
		EndFileEvent = right.EndFileEvent;
		ClientEvent = right.ClientEvent;
		HookEvent = right.HookEvent;
		LogEvent = right.LogEvent;

		ReplyData = right.ReplyData;
		ErrorCode = right.ErrorCode;
		ID = right.ID;

		return *this;
	}

	inline Event& Event::assign(Event&& right) noexcept
	{
		if (&right == this) return *this;

		StartFileEvent = std::move(right.StartFileEvent);
		PropertyEvent = std::move(right.PropertyEvent);
		CommandEvent = std::move(right.CommandEvent);
		EndFileEvent = std::move(right.EndFileEvent);
		ClientEvent = std::move(right.ClientEvent);
		HookEvent = std::move(right.HookEvent);
		LogEvent = std::move(right.LogEvent);

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

		switch (ID)
		{
		case EventID::PropertyChanged:
			ReplyData = right.reply_userdata;
			PropertyEvent = *static_cast<mpv_event_property*>(right.data);
			break;
		case EventID::ClientMessage:
			ClientEvent = *static_cast<mpv_event_client_message*>(right.data);
			break;
		case EventID::CommandReply:
			ErrorCode = code::toError(right.error);
			ReplyData = right.reply_userdata;
			CommandEvent = *static_cast<mpv_event_command*>(right.data);
			break;
		case EventID::GetProperty:
			ErrorCode = code::toError(right.error);
			ReplyData = right.reply_userdata;
			PropertyEvent = *static_cast<mpv_event_property*>(right.data);
			break;
		case EventID::SetProperty:
			ErrorCode = code::toError(right.error);
			ReplyData = right.reply_userdata;
			break;
		case EventID::LogMessage:
			LogEvent = *static_cast<mpv_event_log_message*>(right.data);
			break;
		case EventID::StartFile:
			StartFileEvent = *static_cast<mpv_event_start_file*>(right.data);
			break;
		case EventID::EndFile:
			EndFileEvent = *static_cast<mpv_event_end_file*>(right.data);
			break;
		case EventID::Hook:
			ReplyData = right.reply_userdata;
			HookEvent = *static_cast<mpv_event_hook*>(right.data);
			break;
		}

		return *this;
	}
}
