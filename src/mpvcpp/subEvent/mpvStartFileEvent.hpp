#pragma once
#include<mpv/client.h>
#include<utility>

namespace mpv
{
	namespace event
	{
		class StartFileEvent
		{
		public:
			long long EntryID = -1;	//For playlist
		public:
			inline StartFileEvent() {}
			inline ~StartFileEvent() {}

			inline StartFileEvent(const StartFileEvent& right) { this->assign(right); }
			inline StartFileEvent(const mpv_event_start_file& right) { this->assign(right); }
			inline StartFileEvent(StartFileEvent&& right) noexcept { this->assign(std::move(right)); }

			inline StartFileEvent& operator=(const StartFileEvent& right) { return this->assign(right); }
			inline StartFileEvent& operator=(const mpv_event_start_file& right) { return this->assign(right); }
			inline StartFileEvent& operator=(StartFileEvent&& right) noexcept { return this->assign(std::move(right)); }

			inline StartFileEvent& assign(const StartFileEvent& right);
			inline StartFileEvent& assign(StartFileEvent&& right) noexcept;
			inline StartFileEvent& assign(const mpv_event_start_file& right);
		};

		inline StartFileEvent& StartFileEvent::assign(const StartFileEvent& right)
		{
			if (&right == this) return *this;
			EntryID = right.EntryID;

			return *this;
		}

		inline StartFileEvent& StartFileEvent::assign(StartFileEvent&& right) noexcept
		{
			if (&right == this) return *this;

			EntryID = right.EntryID;
			right.EntryID = -1;

			return *this;
		}

		inline StartFileEvent& StartFileEvent::assign(const mpv_event_start_file& right)
		{
			EntryID = right.playlist_entry_id;
			return *this;
		}
	}
}
