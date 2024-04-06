#pragma once
#include"mpvRenderParamType.hpp"
#include<utility>

namespace mpv
{
	namespace render
	{
		namespace openGL
		{
			using InitParams = mpv_opengl_init_params;
			using FBO = mpv_opengl_fbo;
			namespace drm
			{
				using DrawSurfaceSize = mpv_opengl_drm_draw_surface_size;
				using Params = mpv_opengl_drm_params_v2;
				using OSDSize = DrawSurfaceSize;
			}
		}

		class Param
		{
		public:
			ParamType Type = ParamType::Invalid;
			void* Data = nullptr;
		public:
			inline Param() {}
			inline ~Param();

			inline Param(const Param& right) { this->assign(right); }
			inline Param(const mpv_render_param& right) { this->assign(right); }
			inline Param(Param&& right) noexcept { this->assign(std::move(right)); }

			inline Param& operator=(const Param& right) { this->assign(right); }
			inline Param& operator=(const mpv_render_param& right) { this->assign(right); }
			inline Param& operator=(Param&& right) noexcept { this->assign(std::move(right)); }

			inline Param& assign(const Param& right);
			inline mpv_render_param translate() const;
			inline Param& assign(Param&& right) noexcept;
			inline Param& assign(const mpv_render_param& right);
			inline bool operator!() const { return (Type == ParamType::Invalid || !Data); }
		};

		inline Param::~Param()
		{
			Type = ParamType::Invalid;
			Data = nullptr;
		}

		inline Param& Param::assign(const Param& right)
		{
			Type = right.Type;
			Data = right.Data;
			return *this;
		}

		inline mpv_render_param Param::translate() const
		{
			mpv_render_param result{};
			result.type = fromParamType(Type);

			result.data = Data;
			return result;
		}

		inline Param& Param::assign(Param&& right) noexcept
		{
			Type = right.Type;
			Data = right.Data;

			right.Type = ParamType::Invalid;
			right.Data = nullptr;
			return *this;
		}

		inline Param& Param::assign(const mpv_render_param& right)
		{
			Type = toParamType(right.type);
			Data = right.data;
			return *this;
		}
	}
}
