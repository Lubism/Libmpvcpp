#pragma once
#include"mpvRenderFrameInfoType.hpp"
#include<utility>

namespace mpv
{
	namespace render
	{
		class FrameInfo
		{
		public:
			FrameInfoType Type = FrameInfoType::None;
			long long TargetTime = 0LL;
		public:
			inline FrameInfo() {}
			inline ~FrameInfo() {}

			inline FrameInfo(const FrameInfo& right) { this->assign(right); }
			inline FrameInfo(const mpv_render_frame_info& right) { this->assign(right); }
			inline FrameInfo(FrameInfo&& right) noexcept { this->assign(std::move(right)); }

			inline FrameInfo& operator=(const FrameInfo& right) { return this->assign(right); }
			inline FrameInfo& operator=(const mpv_render_frame_info& right) { return this->assign(right); }
			inline FrameInfo& operator=(FrameInfo&& right) noexcept { return this->assign(std::move(right)); }

			inline FrameInfo& assign(const FrameInfo& right);
			inline FrameInfo& assign(FrameInfo&& right) noexcept;
			inline FrameInfo& assign(const mpv_render_frame_info& right);
		};

		inline FrameInfo& FrameInfo::assign(const FrameInfo& right)
		{
			TargetTime = right.TargetTime;
			Type = right.Type;
			return *this;
		}

		inline FrameInfo& FrameInfo::assign(FrameInfo&& right) noexcept
		{
			TargetTime = right.TargetTime;
			Type = right.Type;

			right.Type = FrameInfoType::None;
			right.TargetTime = 0LL;
			return *this;
		}

		inline FrameInfo& FrameInfo::assign(const mpv_render_frame_info& right)
		{
			Type = toFrameInfoType(right.flags);
			TargetTime = right.target_time;
			return *this;
		}
	}
}
