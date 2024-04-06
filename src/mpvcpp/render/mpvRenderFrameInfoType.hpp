#pragma once
#include"mpv/render.h"

namespace mpv
{
	namespace render
	{
		enum class FrameInfoType
		{
			None		= 0,
			Present		= MPV_RENDER_FRAME_INFO_PRESENT,
			Redraw		= MPV_RENDER_FRAME_INFO_REDRAW,
			Repeat		= MPV_RENDER_FRAME_INFO_REPEAT,
			BlockVsync	= MPV_RENDER_FRAME_INFO_BLOCK_VSYNC
		};

		inline mpv_render_frame_info_flag fromFrameInfoType(const FrameInfoType& value)
		{
			return static_cast<mpv_render_frame_info_flag>(value);
		}

		inline bool operator==(int left, const FrameInfoType& right)
		{
			return static_cast<FrameInfoType>(left) == right;
		}

		inline FrameInfoType toFrameInfoType(int value)
		{
			if (value > MPV_RENDER_FRAME_INFO_BLOCK_VSYNC)
				return FrameInfoType::Present;
			if (value < MPV_RENDER_FRAME_INFO_PRESENT)
				return FrameInfoType::BlockVsync;

			return static_cast<FrameInfoType>(value);
		}
	}
}
