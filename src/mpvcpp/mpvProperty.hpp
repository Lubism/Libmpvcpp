#pragma once
#include"mpvHandle.hpp"
#include"mpvNode.hpp"

namespace mpv
{
	class Property
	{
		using ulong = unsigned long long;
		using Int = long long;

		using Format = code::Format;
		using String = std::string;
	public:
		static inline code::Error Apply(const Handle& handle, const String& name,
			const Int& data, Int replyData = -1);
		static inline code::Error Apply(const Handle& handle, const String& name,
			const String& data, Int replyData = -1);
		static inline code::Error Apply(const Handle& handle, const String& name,
			const char* data, Int replyData = -1);
		static inline code::Error Apply(const Handle& handle, const String& name,
			const double& data, Int replyData = -1);
		static inline code::Error Apply(const Handle& handle, const String& name,
			const bool& data, Int replyData = -1);
		static inline code::Error Apply(const Handle& handle, const String& name,
			const Node& data, Int replyData = -1);

		static inline code::Error Acquire(const Handle& handle, const String& name,
			Int& data);
		static inline code::Error Acquire(const Handle& handle, const String& name,
			String& data);
		static inline code::Error Acquire(const Handle& handle, const String& name,
			double& data);
		static inline code::Error Acquire(const Handle& handle, const String& name,
			bool& data);
		static inline code::Error Acquire(const Handle& handle, const String& name,
			Node& data);
		static inline code::Error Acquire(const Handle& handle, const String& name,
			ulong replyData, const Format& format = Format::Node);
		static inline code::Error AcquireOSDString(const Handle& handle,
			const String& name, String& data);

		static inline code::Error Observe(const Handle& handle, const String& name,
			ulong replyData, const Format& format = Format::Node);
		static inline code::Error Unobserve(const Handle& handle,
			ulong replyData, int& count);
	private:
		template<typename Ty>
		static inline code::Error Setter(const Handle& handle, const String& name,
			const Format& format, Ty data,
			Int replyData = -1LL);

		template<typename Ty>
		static inline code::Error Getter(const Handle& handle, const String& name,
			const Format& format, Ty& data);

		static inline code::Error Getter(const Handle& handle, const String& name,
			ulong replyData, const Format& format = code::Format::Node);
	private:
		template<typename Ty>
		static inline code::Error Apply(const Handle&, const std::string&, const Ty&) = delete;
	};

	inline code::Error Property::Apply(const Handle& handle, const String& name, const Int& data, Int replyData) {
		return Property::Setter(handle, name, Format::Int, data, replyData);
	}
	inline code::Error Property::Apply(const Handle& handle, const String& name, const String& data, Int replyData) {
		return Property::Setter(handle, name, Format::String, data.c_str(), replyData);
	}
	inline code::Error Property::Apply(const Handle& handle, const String& name, const char* data, Int replyData) {
		return Property::Apply(handle, name, String(data), replyData);
	}
	inline code::Error Property::Apply(const Handle& handle, const String& name, const double& data, Int replyData) {
		return Property::Setter(handle, name, Format::Double, data, replyData);
	}
	inline code::Error Property::Apply(const Handle& handle, const String& name, const bool& data, Int replyData) {
		return Property::Setter(handle, name, Format::BooleanInt, data ? 1 : 0, replyData);
	}
	inline code::Error Property::Apply(const Handle& handle, const String& name, const Node& data, Int replyData) {
		return Property::Setter(handle, name, Format::Node, data.translate(), replyData);
	}

	inline code::Error Property::Acquire(const Handle& handle, const String& name, Int& data) {
		return Property::Getter(handle, name, Format::Int, data);
	}
	inline code::Error Property::Acquire(const Handle& handle, const String& name, String& data) {
		char* storage = nullptr;

		auto error = Property::Getter(handle, name,
			Format::String, storage);
		if (storage) data.assign(storage);
		mpv_free(storage);
		return error;
	}
	inline code::Error Property::Acquire(const Handle& handle, const String& name, double& data) {
		return Property::Getter(handle, name, Format::Double, data);
	}
	inline code::Error Property::Acquire(const Handle& handle, const String& name, bool& data) {
		int storage = 0;

		auto error = Property::Getter(handle, name,
			Format::BooleanInt, storage);
		data = (storage == 1) ? true : false;
		return error;
	}
	inline code::Error Property::Acquire(const Handle& handle, const String& name, Node& data) {
		mpv_node storage{};

		auto error = Property::Getter(handle, name,
			Format::Node, storage);
		data = storage;

		mpv_free_node_contents(&storage);
		return error;
	}
	inline code::Error Property::Acquire(const Handle& handle, const String& name, ulong replyData,
		const Format& format)
	{
		return Property::Getter(handle, name,
			replyData, format);
	}
	inline code::Error Property::AcquireOSDString(const Handle& handle, const String& name, String& data) {
		char* storage = nullptr;

		auto error = Property::Getter(handle, name,
			Format::OSDString, storage);
		if (storage) data.assign(storage);
		mpv_free(storage);
		return error;
	}

	inline code::Error Property::Observe(const Handle& handle, const String& name,
		ulong replyData, const Format& format)
	{
		if (!handle || name.empty()) return code::Error::InvalidParameter;
		return code::toError(mpv_observe_property(handle.Data, replyData,
			name.c_str(), code::fromFormat(format)));
	}
	inline code::Error Property::Unobserve(const Handle& handle,
		ulong replyData, int& count)
	{
		if (!handle) return code::Error::InvalidParameter;

		int storage = mpv_unobserve_property(handle.Data, replyData);
		if (storage < 0) return code::toError(storage);

		count = storage;
		return code::Error::Success;
	}

	template<typename Ty>
	inline code::Error Property::Setter(const Handle& handle, const String& name,
		const Format& format, Ty data, Int replyData)
	{
		if (!handle || name.empty()) return code::Error::InvalidParameter;

		if (replyData < 0) return code::toError(mpv_set_property(
			handle.Data, name.c_str(), code::fromFormat(format),
				&data));

		return code::toError(mpv_set_property_async(handle.Data,
			static_cast<unsigned long long>(replyData),
			name.c_str(), code::fromFormat(format),
			&data));
	}

	template<typename Ty>
	inline code::Error Property::Getter(const Handle& handle, const String& name,
		const Format& format, Ty& data)
	{
		if (!handle || name.empty()) return code::Error::InvalidParameter;

		return code::toError(mpv_get_property(handle.Data, name.c_str(),
			code::fromFormat(format), &data));
	}

	inline code::Error Property::Getter(const Handle& handle,
		const String& name, ulong replyData,
		const Format& format)
	{
		if (!handle || name.empty()) return code::Error::InvalidParameter;

		return code::toError(mpv_get_property_async(handle.Data, replyData,
			name.c_str(), code::fromFormat(format)));
	}
}
