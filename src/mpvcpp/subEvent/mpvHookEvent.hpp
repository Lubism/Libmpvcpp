#pragma once
#include<mpv/client.h>
#include<string>

namespace mpv
{
	namespace event
	{
		class HookEvent
		{
		public:
			std::string Name = "";
			unsigned long long ID = 0;
		public:
			inline HookEvent() {}
			inline ~HookEvent() {}

			inline HookEvent(const HookEvent& right) { this->assign(right); }
			inline HookEvent(const mpv_event_hook& right) { this->assign(right); }
			inline HookEvent(HookEvent&& right) noexcept { this->assign(std::move(right)); }

			inline HookEvent& operator=(const HookEvent& right) { return this->assign(right); }
			inline HookEvent& operator=(const mpv_event_hook& right) { return this->assign(right); }
			inline HookEvent& operator=(HookEvent&& right) noexcept { return this->assign(std::move(right)); }

			inline HookEvent& assign(const HookEvent& right);
			inline HookEvent& assign(HookEvent&& right) noexcept;
			inline HookEvent& assign(const mpv_event_hook& right);
		};

		inline HookEvent& HookEvent::assign(const HookEvent& right)
		{
			if (&right == this) return *this;
			Name = right.Name;
			ID = right.ID;
			return *this;
		}

		inline HookEvent& HookEvent::assign(HookEvent&& right) noexcept
		{
			if (&right == this) return *this;

			Name = std::move(right.Name);
			ID = right.ID;
			right.ID = 0;

			return *this;
		}

		inline HookEvent& HookEvent::assign(const mpv_event_hook& right)
		{
			if (right.name == nullptr)
			{
				std::string().swap(Name);
				return *this;
			}

			Name.assign(right.name);
			ID = right.id;

			return *this;
		}
	}
}
