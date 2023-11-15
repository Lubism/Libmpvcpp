#pragma once
#include"node/mpvStringArray.hpp"
#include"node/mpvByteArray.hpp"
#include"node/mpvString.hpp"
#include"code/mpvFormat.hpp"
#include"code/mpvError.hpp"
#include<unordered_map>

namespace mpv
{
	class Node
	{
		using ulong = unsigned long long;
		using uchar = unsigned char;
	public:
		code::Format Format = code::Format::None;
		std::string String = "";
		double Double = 0.0;
		long long Int = 0;
		bool Bool = false;

		std::unordered_map<std::string, Node> Map;
		std::vector<uchar> ByteArray;
		std::vector<Node> Array;
	public:
		inline Node() {}
		inline ~Node() { this->clearNode(); }
		inline Node(const Node& right) { this->assign(right); }
		inline Node(const mpv_node& right) { this->assign(right); }
		inline Node(Node&& right) noexcept { this->assign(std::move(right)); }
		inline Node& operator=(const Node& right) { return this->assign(right); }
		inline Node& operator=(const mpv_node& right) { return this->assign(right); }
		inline Node& operator=(Node&& right) noexcept { return this->assign(std::move(right)); }

		inline void clear();
		inline mpv_node translate();
		inline Node& assign(const Node& right);
		inline Node& assign(Node&& right) noexcept;
		inline Node& assign(const mpv_node& right);
	private:
		inline mpv_node_list* translateArray();
		inline mpv_node_list* translateMap();
		inline void clearNode();

		mpv_node_list mpvNodelist{};
		mpv_node mpvNode{};
	};

	inline void Node::clear()
	{
		std::unordered_map<std::string, Node>().swap(Map);
		std::vector<uchar>().swap(ByteArray);
		std::vector<Node>().swap(Array);

		Format = code::Format::None;
		Double = 0.0;
		Bool = false;
		String = "";
		Int = 0;
	}

	inline mpv_node Node::translate()
	{
		this->clearNode();
		ulong it = 0;

		mpvNode.format = code::fromFormat(Format);
		auto& data = mpvNode.u;
		switch (Format)
		{
		case code::Format::BooleanInt:
			data.flag = Bool ? 1 : 0;
			break;
		case code::Format::ByteArray:
			data.ba = node::ByteArray::Create(ByteArray);
			break;
		case code::Format::NodeArray:
			data.list = this->translateArray();
			break;
		case code::Format::NodeMap:
			data.list = this->translateMap();
			break;
		case code::Format::OSDString:
		case code::Format::String:
			data.string = node::String::Create(String);
			break;
		case code::Format::Double:
			data.double_ = Double;
			break;
		case code::Format::Int:
			data.int64 = Int;
			break;
		}

		return mpvNode;
	}

	inline Node& Node::assign(const Node& right)
	{
		if (&right == this) return *this;

		ByteArray.assign(right.ByteArray.begin(),
			right.ByteArray.end());
		Array.assign(right.Array.begin(),
			right.Array.end());
		Format = right.Format;
		String = right.String;
		Double = right.Double;
		Bool = right.Bool;
		Int = right.Int;
		Map = right.Map;

		return *this;
	}

	inline Node& Node::assign(Node&& right) noexcept
	{
		if (&right == this) return *this;

		ByteArray = std::move(right.ByteArray);
		String = std::move(right.String);
		Array = std::move(right.Array);
		Map = std::move(right.Map);
		Format = right.Format;
		Double = right.Double;
		Bool = right.Bool;
		Int = right.Int;

		right.Format = code::Format::None;
		right.Double = 0.0;
		right.Bool = false;
		right.String = "";
		right.Int = 0;

		return *this;
	}

	inline Node& Node::assign(const mpv_node& right)
	{
		if (right.format == MPV_FORMAT_NONE)
			return *this;
		this->clear();

		Format = code::toFormat(right.format);
		auto& data = right.u;
		ulong it = 0;

		switch (Format)
		{
		case code::Format::BooleanInt:
			Bool = (data.flag == 1) ? true : false;
			break;
		case code::Format::ByteArray:
			node::ByteArray::Assign(ByteArray,
				*data.ba);
			break;
		case code::Format::NodeArray:
			for (it = 0; it < data.list->num; it++)
				Array.push_back(data.list->values[it]);
			break;
		case code::Format::NodeMap:
			for (it = 0; it < data.list->num; it++)
				Map[data.list->keys[it]] = data.list->values[it];
			break;
		case code::Format::OSDString:
		case code::Format::String:
			String = data.string;
			break;
		case code::Format::Double:
			Double = data.double_;
			break;
		case code::Format::Int:
			Int = data.int64;
			break;
		}

		return *this;
	}

	inline mpv_node_list* Node::translateArray()
	{
		if (Array.empty()) return &mpvNodelist;

		mpvNodelist.num = static_cast<int>(Array.size());
		mpvNodelist.values = new mpv_node[Array.size()];
		mpvNodelist.keys = nullptr;
		ulong index = 0;

		for (auto it = Array.begin(); it != Array.end(); it++)
		{
			index = std::distance(Array.begin(), it);
			mpvNodelist.values[index] = it->translate();
		}

		return &mpvNodelist;
	}

	inline mpv_node_list* Node::translateMap()
	{
		if (Map.empty()) return &mpvNodelist;
		std::vector<std::string> storage;
		for (auto& it : Map) {
			storage.push_back(it.first);
		}

		mpvNodelist.keys = node::StringArray::Create(storage);
		mpvNodelist.num = static_cast<int>(Map.size());
		mpvNodelist.values = new mpv_node[Map.size()];
		ulong index = 0;

		for (auto it = Map.begin(); it != Map.end(); it++)
		{
			index = std::distance(Map.begin(), it);
			mpvNodelist.values[index] = it->second.translate();
		}

		return &mpvNodelist;
	}

	inline void Node::clearNode()
	{
		auto format = code::toFormat(mpvNode.format);
		if (format == code::Format::None)
			return;

		mpvNode.format = MPV_FORMAT_NONE;
		auto& data = mpvNode.u;
		ulong it = 0;

		switch (format)
		{
		case code::Format::BooleanInt:
		case code::Format::Double:
		case code::Format::Int:
			break;
		case code::Format::ByteArray:
			node::ByteArray::Delete(data.ba);
			break;
		case code::Format::NodeArray:
			delete[] mpvNodelist.values;

			mpvNodelist.values = nullptr;
			data.list = nullptr;
			break;
		case code::Format::NodeMap:
			node::StringArray::Delete(mpvNodelist.keys,
				mpvNodelist.num);
			delete[] mpvNodelist.values;

			mpvNodelist.values = nullptr;
			data.list = nullptr;
			break;
		case code::Format::OSDString:
		case code::Format::String:
			node::String::Delete(data.string);
			break;
		}
	}
}
