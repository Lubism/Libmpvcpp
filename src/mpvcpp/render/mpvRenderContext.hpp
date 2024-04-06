#pragma once
#include"mpvcpp/mpvHandle.hpp"
#include"mpvRenderParam.hpp"
#include<memory>
#include<vector>

namespace mpv
{
	namespace render
	{
		class Context
		{
			using uInt = unsigned long long;
		public:
			inline Context() {}
			inline ~Context() { this->destroy(); }
			inline Context(const Context&) = delete;
			inline Context& operator=(const Context&) = delete;

			inline code::Error create(const Handle& handle,
				const std::vector<Param>& params);
			inline code::Error receiveInfo(Param& param) const;
			inline code::Error setParam(const Param& param) const;

			template<typename Func, typename Arg>
			inline code::Error setWakeupCallback(Func func, Arg arg) const;

			inline code::Error renderFrame(const std::vector<Param>& params) const;
			inline code::Error update(unsigned long long& frameCount) const;
			inline code::Error swapFrame() const;
			inline void destroy();
		private:
			mpv_render_context* Data = nullptr;
		};

		inline code::Error Context::create(const Handle& handle,
			const std::vector<Param>& params)
		{
			if (!handle || params.empty()) return code::Error::InvalidParameter;
			size_t size = params.size();
			size_t index = 0;

			std::unique_ptr<mpv_render_param> storage(
				new mpv_render_param[size + 1]);
			for (auto it = params.begin();
				it != params.end(); it++)
			{
				index = std::distance(params.begin(), it);
				storage.operator->()[index].data = it->Data;
				storage.operator->()[index].type = fromParamType(it->Type);
			}

			storage.operator->()[size].type = fromParamType(
				ParamType::Invalid);
			storage.operator->()[size].data = nullptr;

			return code::toError(mpv_render_context_create(
				&Data, handle.Data, storage.get())
			);
		}

		inline code::Error Context::receiveInfo(Param& param) const
		{
			if (!Data) return code::Error::Uninitialized;
			mpv_render_param storage{};

			auto error = code::toError(mpv_render_context_get_info(
				Data, storage));

			if (error == code::Error::Success)
				param = storage;
			return error;
		}

		inline code::Error Context::setParam(const Param& param) const
		{
			if (!Data) return code::Error::Uninitialized;
			if (!param) return code::Error::InvalidParameter;

			return code::toError(mpv_render_context_set_parameter(
				Data, param.translate())
			);
		}

		template<typename Func, typename Arg>
		inline code::Error Context::setWakeupCallback(Func func, Arg arg) const
		{
			if (!Data) return code::Error::Uninitialized;
			auto callback = reinterpret_cast<
				void (*)(void*)>(func);

			mpv_render_context_set_update_callback(
				Data, callback, &arg);
			return code::Error::Success;
		}

		inline code::Error Context::renderFrame(const std::vector<Param>& params) const
		{
			if (!Data || params.empty()) return code::Error::InvalidParameter;
			size_t size = params.size();
			size_t index = 0;

			std::unique_ptr<mpv_render_param> storage(
				new mpv_render_param[size + 1]);
			for (auto it = params.begin();
				it != params.end(); it++)
			{
				index = std::distance(params.begin(), it);
				storage.operator->()[index].data = it->Data;
				storage.operator->()[index].type = fromParamType(it->Type);
			}

			storage.operator->()[size].type = fromParamType(
				ParamType::Invalid);
			storage.operator->()[size].data = nullptr;

			return code::toError(mpv_render_context_render(Data,
				storage.get()));
		}

		inline code::Error Context::update(unsigned long long& frameCount) const
		{
			if (!Data) return code::Error::Uninitialized;
			frameCount = mpv_render_context_update(Data);
			return code::Error::Success;
		}

		inline code::Error Context::swapFrame() const
		{
			if (!Data) return code::Error::Uninitialized;
			mpv_render_context_report_swap(Data);
			return code::Error::Success;
		}

		inline void Context::destroy()
		{
			mpv_render_context_free(Data);
			Data = nullptr;
		}
	}
}
