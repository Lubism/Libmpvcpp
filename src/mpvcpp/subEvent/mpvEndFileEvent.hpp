#pragma once
#include"mpvcpp/code/mpvEndFileReason.hpp"
#include"mpvcpp/code/mpvEventID.hpp"
#include"mpvcpp/code/mpvError.hpp"

namespace mpv
{
	namespace event
	{
		class EndFileEvent
		{
		public:
			code::EndFileReason Reason = code::EndFileReason::Error;
			code::Error ErrorCode = code::Error::Generic;

			long long EntryID = -1;		//For playlist
			long long InsertID = -1;	//For playlist
			int InsertNumEntries = -1;	//For playlist
		public:
			inline EndFileEvent() {}
			inline ~EndFileEvent() {}

			inline EndFileEvent(const EndFileEvent& right) { this->assign(right); }
			inline EndFileEvent(const mpv_event_end_file& right) { this->assign(right); }
			inline EndFileEvent(EndFileEvent&& right) noexcept { this->assign(std::move(right)); }

			inline EndFileEvent& operator=(const EndFileEvent& right) { return this->assign(right); }
			inline EndFileEvent& operator=(const mpv_event_end_file& right) { return this->assign(right); }
			inline EndFileEvent& operator=(EndFileEvent&& right) noexcept { return this->assign(std::move(right)); }

			inline EndFileEvent& assign(const EndFileEvent& right);
			inline EndFileEvent& assign(EndFileEvent&& right) noexcept;
			inline EndFileEvent& assign(const mpv_event_end_file& right);
		};

		inline EndFileEvent& EndFileEvent::assign(const EndFileEvent& right)
		{
			if (&right == this) return *this;

			InsertNumEntries = right.InsertNumEntries;
			ErrorCode = right.ErrorCode;
			InsertID = right.InsertID;
			EntryID = right.EntryID;
			Reason = right.Reason;

			return *this;
		}

		inline EndFileEvent& EndFileEvent::assign(EndFileEvent&& right) noexcept
		{
			if (&right == this) return *this;

			InsertNumEntries = right.InsertNumEntries;
			ErrorCode = right.ErrorCode;
			InsertID = right.InsertID;
			EntryID = right.EntryID;
			Reason = right.Reason;

			right.Reason = code::EndFileReason::Error;
			right.ErrorCode = code::Error::Generic;
			right.InsertNumEntries = -1;
			right.InsertID = -1;
			right.EntryID = -1;

			return *this;
		}

		inline EndFileEvent& EndFileEvent::assign(const mpv_event_end_file& right)
		{
			InsertNumEntries = right.playlist_insert_num_entries;
			Reason = code::toEndFileReason(right.reason);
			ErrorCode = code::toError(right.error);
			InsertID = right.playlist_insert_id;
			EntryID = right.playlist_entry_id;

			return *this;
		}
	}
}
