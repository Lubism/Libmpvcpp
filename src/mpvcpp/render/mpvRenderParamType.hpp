#pragma once
#include"mpv/render_gl.h"

namespace mpv
{
	namespace render
	{
		enum class ParamType
		{
			Invalid				= MPV_RENDER_PARAM_INVALID,
			APIType				= MPV_RENDER_PARAM_API_TYPE,
			OpenGL_Init			= MPV_RENDER_PARAM_OPENGL_INIT_PARAMS,
			OpenGL_FBO			= MPV_RENDER_PARAM_OPENGL_FBO,

			Flip_Y				= MPV_RENDER_PARAM_FLIP_Y,
			Depth				= MPV_RENDER_PARAM_DEPTH,
			ICCProfile			= MPV_RENDER_PARAM_ICC_PROFILE,
			AmbientLight		= MPV_RENDER_PARAM_AMBIENT_LIGHT,

			X11_Display			= MPV_RENDER_PARAM_X11_DISPLAY,
			WL_Display			= MPV_RENDER_PARAM_WL_DISPLAY,
			AdvancedControl		= MPV_RENDER_PARAM_ADVANCED_CONTROL,
			NextFrameInfo		= MPV_RENDER_PARAM_NEXT_FRAME_INFO,

			BlockFor_TargetTime = MPV_RENDER_PARAM_BLOCK_FOR_TARGET_TIME,
			SkipRendering		= MPV_RENDER_PARAM_SKIP_RENDERING,
			DRM_DrawSurfaceSize = MPV_RENDER_PARAM_DRM_DRAW_SURFACE_SIZE,
			DRM_Display			= MPV_RENDER_PARAM_DRM_DISPLAY_V2,

			SW_Size				= MPV_RENDER_PARAM_SW_SIZE,
			SW_Format			= MPV_RENDER_PARAM_SW_FORMAT,
			SW_Stride			= MPV_RENDER_PARAM_SW_STRIDE,
			SW_Pointer			= MPV_RENDER_PARAM_SW_POINTER
		};

		inline mpv_render_param_type fromParamType(const ParamType& value)
		{
			return static_cast<mpv_render_param_type>(value);
		}

		inline bool operator==(int left, const ParamType& right)
		{
			return static_cast<ParamType>(left) == right;
		}

		inline ParamType toParamType(int value)
		{
			if (value > MPV_RENDER_PARAM_SW_POINTER)
				return ParamType::Invalid;
			if (value < MPV_RENDER_PARAM_INVALID)
				return ParamType::Invalid;

			return static_cast<ParamType>(value);
		}
	}
}
