#pragma once
#include<mpv/client.h>
#include<string>
#include<vector>

namespace mpv
{
	namespace event
	{
		class ClientEvent
		{
		public:
			std::vector<std::string> Args;
		public:
			inline ClientEvent() {}
			inline ~ClientEvent() {}

			inline ClientEvent(const ClientEvent& right) { this->assign(right); }
			inline ClientEvent(const mpv_event_client_message& right) { this->assign(right); }
			inline ClientEvent(ClientEvent&& right) noexcept { this->assign(std::move(right)); }

			inline ClientEvent& operator=(const ClientEvent& right) { return this->assign(right); }
			inline ClientEvent& operator=(const mpv_event_client_message& right) { return this->assign(right); }
			inline ClientEvent& operator=(ClientEvent&& right) noexcept { return this->assign(std::move(right)); }

			inline ClientEvent& assign(const ClientEvent& right);
			inline ClientEvent& assign(ClientEvent&& right) noexcept;
			inline ClientEvent& assign(const mpv_event_client_message& right);
		};

		inline ClientEvent& ClientEvent::assign(const ClientEvent& right)
		{
			if (&right == this) return *this;

			Args.assign(right.Args.begin(),
				right.Args.end());

			return *this;
		}

		inline ClientEvent& ClientEvent::assign(ClientEvent&& right) noexcept
		{
			if (&right == this) return *this;
			Args = std::move(right.Args);
			return *this;
		}

		inline ClientEvent& ClientEvent::assign(const mpv_event_client_message& right)
		{
			std::vector<std::string>().swap(Args);
			if (right.args == nullptr ||
				right.num_args == 0)
				return *this;

			for (int it = 0; it < right.num_args; it++)
				Args.push_back(right.args[it]);
			return *this;
		}
	}
}
