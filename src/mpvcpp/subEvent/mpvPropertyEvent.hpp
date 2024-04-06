#pragma once
#include"mpvcpp/mpvNode.hpp"

namespace mpv
{
	namespace event
	{
		class PropertyEvent
		{
			using Format = code::Format;
		public:
			Format FormatType = code::Format::None;
			std::string Name = "";

			std::string String = "";
			double Double = 0.0;
			long long Int = 0;
			bool Bool = false;
			mpv::Node Node;
		public:
			inline PropertyEvent() {}
			inline ~PropertyEvent() {}

			inline PropertyEvent(const PropertyEvent& right) { this->assign(right); }
			inline PropertyEvent(const mpv_event_property& right) { this->assign(right); }
			inline PropertyEvent(PropertyEvent&& right) noexcept { this->assign(std::move(right)); }

			inline PropertyEvent& operator=(const PropertyEvent& right) { return this->assign(right); }
			inline PropertyEvent& operator=(const mpv_event_property& right) { return this->assign(right); }
			inline PropertyEvent& operator=(PropertyEvent&& right) noexcept { return this->assign(std::move(right)); }

			inline PropertyEvent& assign(const PropertyEvent& right);
			inline PropertyEvent& assign(PropertyEvent&& right) noexcept;
			inline PropertyEvent& assign(const mpv_event_property& right);
		};

		inline PropertyEvent& PropertyEvent::assign(const PropertyEvent& right)
		{
			if (&right == this) return *this;

			FormatType = right.FormatType;
			String = right.String;
			Double = right.Double;
			Name = right.Name;
			Node = right.Node;
			Bool = right.Bool;
			Int = right.Int;

			return *this;
		}

		inline PropertyEvent& PropertyEvent::assign(PropertyEvent&& right) noexcept
		{
			if (&right == this) return *this;

			String = std::move(right.String);
			Name = std::move(right.Name);
			Node = std::move(right.Node);

			FormatType = right.FormatType;
			Double = right.Double;
			Bool = right.Bool;
			Int = right.Int;

			right.FormatType = code::Format::None;
			right.Double = 0.0;
			right.Bool = false;
			right.Int = 0;

			return *this;
		}

		inline PropertyEvent& PropertyEvent::assign(const mpv_event_property& right)
		{
			if (right.data == nullptr)
			{
				FormatType = Format::None;
				return *this;
			}

			int flag = 0;
			Name = right.name;
			FormatType = code::toFormat(right.format);
			switch (this->FormatType)
			{
			case Format::BooleanInt:
				flag = *static_cast<int*>(right.data);
				Bool = ((flag == 1) ? true : false);
				break;
			case Format::String:
				String = static_cast<const char*>(right.data);
				break;
			case Format::Double:
				Double = *static_cast<double*>(right.data);
				break;
			case Format::Node:
				Node = *static_cast<mpv_node*>(right.data);
				break;
			case Format::Int:
				Int = *static_cast<long long*>(right.data);
				break;
			default:
				break;
			}

			return *this;
		}
	}
}
