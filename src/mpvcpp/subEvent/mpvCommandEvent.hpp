#pragma once
#include"mpvcpp/mpvNode.hpp"

namespace mpv
{
	namespace event
	{
		class CommandEvent
		{
		public:
			mpv::Node Node;
		public:
			inline CommandEvent() {}
			inline ~CommandEvent() {}

			inline CommandEvent(const CommandEvent& right) { this->assign(right); }
			inline CommandEvent(const mpv_event_command& right) { this->assign(right); }
			inline CommandEvent(CommandEvent&& right) noexcept { this->assign(std::move(right)); }

			inline CommandEvent& operator=(const CommandEvent& right) { return this->assign(right); }
			inline CommandEvent& operator=(const mpv_event_command& right) { return this->assign(right); }
			inline CommandEvent& operator=(CommandEvent&& right) noexcept { return this->assign(std::move(right)); }

			inline CommandEvent& assign(const CommandEvent& right);
			inline CommandEvent& assign(CommandEvent&& right) noexcept;
			inline CommandEvent& assign(const mpv_event_command& right);
		};

		inline CommandEvent& CommandEvent::assign(const CommandEvent& right)
		{
			if (&right == this) return *this;
			Node = right.Node;
			return *this;
		}

		inline CommandEvent& CommandEvent::assign(CommandEvent&& right) noexcept
		{
			if (&right == this) return *this;
			Node = std::move(right.Node);
			return *this;
		}

		inline CommandEvent& CommandEvent::assign(const mpv_event_command& right)
		{
			Node = right.result;
			return *this;
		}
	}
}
