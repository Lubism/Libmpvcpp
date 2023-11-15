#pragma once
#include"mpvHandle.hpp"
#include"mpvNode.hpp"
#include<memory>

namespace mpv
{
	class Command
	{
		using String = std::string;
		using ulong = unsigned long long;
		using Args = std::vector<std::string>;
		using charArgs = std::unique_ptr<const char*>;
	public:
		static inline code::Error Sync(const Handle& handle, const Args& value);
		static inline code::Error SyncNode(const Handle& handle, Node args, Node& result);
		static inline code::Error SyncString(const Handle& handle, const String& stringArgs);

		static inline code::Error Async(const Handle& handle, const Args& value, ulong replyData);
		static inline code::Error AsyncNode(const Handle& handle, Node args, ulong replyData);

		static inline code::Error Acquire(const Handle& handle, const Args& value, Node& result);
		static inline code::Error AbortAsync(const Handle& handle, ulong replyData);
	private:
		static inline void ConvertArgs(const Args& value);
		static charArgs ArgPointer;
		static ulong ArgLength;
	};

	inline code::Error Command::Sync(const Handle& handle, const Args& value)
	{
		if (!handle || value.empty()) return code::Error::InvalidParameter;
		Command::ConvertArgs(value);

		return code::toError(mpv_command(handle.Data,
			ArgPointer.get()));
	}
	inline code::Error Command::SyncNode(const Handle& handle, Node args, Node& result)
	{
		if (!handle || args.Format == code::Format::None)
			return code::Error::InvalidParameter;
		mpv_node input = args.translate();
		mpv_node output{};

		auto error = code::toError(mpv_command_node(handle.Data, &input, &output));
		if (error == code::Error::Success) result = output;
		mpv_free_node_contents(&output);
		return error;
	}
	inline code::Error Command::SyncString(const Handle& handle, const String& stringArgs)
	{
		if (!handle || stringArgs.empty()) return code::Error::InvalidParameter;
		return code::toError(mpv_command_string(handle.Data,
			stringArgs.c_str()));
	}

	inline code::Error Command::Async(const Handle& handle, const Args& value, ulong replyData)
	{
		if (!handle || value.empty()) return code::Error::InvalidParameter;
		Command::ConvertArgs(value);

		return code::toError(mpv_command_async(
			handle.Data, replyData,
			ArgPointer.get()));
	}
	inline code::Error Command::AsyncNode(const Handle& handle, Node args, ulong replyData)
	{
		if (!handle || args.Format == code::Format::None)
			return code::Error::InvalidParameter;
		mpv_node storage = args.translate();

		return code::toError(mpv_command_node_async(
			handle.Data, replyData,
			&storage));
	}

	inline code::Error Command::Acquire(const Handle& handle, const Args& value, Node& result)
	{
		if (!handle || value.empty()) return code::Error::InvalidParameter;
		Command::ConvertArgs(value);
		mpv_node output{};

		auto error = code::toError(mpv_command_ret(
			handle.Data, ArgPointer.get(),
			&output));

		if (error == code::Error::Success) result = output;
		mpv_free_node_contents(&output);
		return error;
	}
	inline code::Error Command::AbortAsync(const Handle& handle, ulong replyData)
	{
		if (!handle) return code::Error::InvalidParameter;
		mpv_abort_async_command(handle.Data, replyData);
		return code::Error::Success;
	}

	inline void Command::ConvertArgs(const Args& value)
	{
		size_t index = 0;
		std::string storage = "";
		if (ArgLength <= value.size())
		{
			while (ArgLength <= value.size())
				ArgLength += ArgLength;
			ArgPointer.reset(new const char* [ArgLength]);
		}

		auto isNull = [&](const std::string& value) {
			storage = value;

			for (auto it = storage.begin(); it != storage.end(); it++)
				*it = std::tolower(*it);
			return (storage == "nullptr" || storage == "null");
		};

		for (auto it = value.begin(); it != value.end(); it++)
		{
			index = std::distance(value.begin(), it);

			if (it->empty() || isNull(*it))
				ArgPointer.operator->()[index] = nullptr;
			else
				ArgPointer.operator->()[index] = it->c_str();
		}

		ArgPointer.operator->()[++index] = nullptr;
	}

	Command::charArgs Command::ArgPointer = Command::charArgs(new const char* [16]);
	Command::ulong Command::ArgLength = 16;
}
