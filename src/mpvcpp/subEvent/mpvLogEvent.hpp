#pragma once
#include"mpvcpp/code/mpvLogLevel.hpp"
#include<string>

namespace mpv
{
	namespace event
	{
		class LogEvent
		{
		public:
			code::LogLevel Level = code::LogLevel::None;
			std::string LevelDescription = "";
			std::string Prefix = "";
			std::string Text = "";
		public:
			inline LogEvent() {}
			inline ~LogEvent() {}

			inline LogEvent(const LogEvent& right) { this->assign(right); }
			inline LogEvent(const mpv_event_log_message& right) { this->assign(right); }
			inline LogEvent(LogEvent&& right) noexcept { this->assign(std::move(right)); }

			inline LogEvent& operator=(const LogEvent& right) { return this->assign(right); }
			inline LogEvent& operator=(const mpv_event_log_message& right) { return this->assign(right); }
			inline LogEvent& operator=(LogEvent&& right) noexcept { return this->assign(std::move(right)); }

			inline LogEvent& assign(const LogEvent& right);
			inline LogEvent& assign(LogEvent&& right) noexcept;
			inline LogEvent& assign(const mpv_event_log_message& right);
		};

		inline LogEvent& LogEvent::assign(const LogEvent& right)
		{
			if (&right == this) return *this;

			LevelDescription = right.LevelDescription;
			Prefix = right.Prefix;
			Level = right.Level;
			Text = right.Text;

			return *this;
		}

		inline LogEvent& LogEvent::assign(LogEvent&& right) noexcept
		{
			if (&right == this) return *this;

			LevelDescription = std::move(right.LevelDescription);
			Prefix = std::move(right.Prefix);
			Text = std::move(right.Text);
			Level = right.Level;

			right.Level = code::LogLevel::None;
			return *this;
		}

		inline LogEvent& LogEvent::assign(const mpv_event_log_message& right)
		{
			if (!right.prefix || !right.level || !right.text) {
				std::string().swap(LevelDescription);
				std::string().swap(Prefix);
				std::string().swap(Text);
				return *this;
			}

			Level = code::toLogLevel(right.log_level);
			LevelDescription = right.level;
			Prefix = right.prefix;
			Text = right.text;
			return *this;
		}
	}
}
