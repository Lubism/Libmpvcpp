#pragma once
#include"code/mpvFormat.hpp"
#include"code/mpvError.hpp"
#include<vector>
#include<memory>
#include<map>

namespace mpv
{
	class Node
	{
		template<typename Ty>
		using Ptr = std::unique_ptr<Ty>;

		using ulong = unsigned long long;
		using uchar = unsigned char;
		using Fmt = code::Format;

		using listptr = Ptr<mpv_node_list>;
		using nodeptr = Ptr<mpv_node>;
		using keyptr = Ptr<char*>;
	public:
		code::Format Format = code::Format::None;
		std::string String = "";
		double Double = 0.0;
		long long Int = 0;
		bool Bool = false;

		std::map<std::string, mpv::Node> Map;
		std::vector<uchar> ByteArray;
		std::vector<Node> Array;
	public:
		inline Node() {}
		inline ~Node() { this->clearPtr(); }
		inline Node(const char* right) { this->assign(right); }
		inline Node(const Node& right) { this->assign(right); }
		inline Node(Node&& right) noexcept { this->assign(std::move(right)); }
		template<typename Ty> inline Node(const Ty& right) { this->assign(right); }

		inline operator mpv_node() const { return this->translate(); }
		inline Node& operator=(const char* right) { return this->assign(right); }
		inline Node& operator=(const Node& right) { return this->assign(right); }
		inline Node& operator=(Node&& right) noexcept { return this->assign(std::move(right)); }
		template<typename Ty> inline Node& operator=(const Ty& right) { return this->assign(right); }

		inline void clear();
		inline mpv_node translate() const;
		inline Node& assign(const Node& right);
		inline Node& assign(const mpv_node& right);
		inline Node& assign(Node&& right) noexcept;
		template<typename Ty> inline Node& assign(const Ty&) = delete;

		inline Node& assign(const long long& right) { Format = Fmt::Int; Int = right; return *this; }
		inline Node& assign(const char* right) { Format = Fmt::String; String = right; return *this; }
		inline Node& assign(const bool& right) { Format = Fmt::BooleanInt; Bool = right; return *this; }
		inline Node& assign(const double& right) { Format = Fmt::Double; Double = right; return *this; }
		inline Node& assign(const std::string& right) { Format = Fmt::String; String = right; return *this; }
		inline Node& assign(const std::vector<Node>& right) { Format = Fmt::NodeArray; Array = right; return *this; }
		inline Node& assign(const std::vector<uchar>& right) { Format = Fmt::ByteArray; ByteArray = right; return *this; }
		inline Node& assign(const std::map<std::string, Node>& right) { Format = Fmt::NodeMap; Map = right; return *this; }
	private:
		inline mpv_node_list* translateArray() const;
		inline mpv_node_list* translateMap() const;
		inline void clearPtr() const;

		listptr Listptr = listptr(new mpv_node_list{});
		nodeptr Nodeptr = nodeptr(new mpv_node{});

		nodeptr Valueptr = nodeptr();
		keyptr Keyptr = keyptr();
	};

	inline void Node::clear()
	{
		std::map<std::string, Node>().swap(Map);
		std::vector<uchar>().swap(ByteArray);
		std::vector<Node>().swap(Array);
		std::string().swap(String);
		Format = Fmt::None;
		Double = 0.0;
		Bool = false;
		Int = 0;
	}

	inline mpv_node Node::translate() const
	{
		this->clearPtr();
		auto& data = Nodeptr->u;
		Nodeptr->format = code::fromFormat(Format);

		switch (Format)
		{
		case Fmt::BooleanInt:
			data.flag = Bool ? 1 : 0;
			break;
		case Fmt::ByteArray:
			data.ba->size = ByteArray.size();
			data.ba->data = const_cast<uchar*>(&ByteArray.front());
			break;
		case Fmt::NodeArray:
			data.list = this->translateArray();
			break;
		case Fmt::NodeMap:
			data.list = this->translateMap();
			break;
		case Fmt::OSDString:
		case Fmt::String:
			data.string = const_cast<char*>(String.data());
			break;
		case Fmt::Double:
			data.double_ = Double;
			break;
		case Fmt::Int:
			data.int64 = Int;
			break;
		default:
			break;
		}

		return *Nodeptr;
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

	inline Node& Node::assign(const mpv_node& right)
	{
		if (right.format == Fmt::None)
			return *this;
		this->clear();

		ulong it = 0;
		auto& data = right.u;
		auto& list = data.list;
		Format = code::toFormat(right.format);
		switch (Format)
		{
		case Fmt::BooleanInt:
			Bool = (data.flag == 1) ? true : false;
			break;
		case Fmt::ByteArray:
			ByteArray.reserve(data.ba->size);
			for (it = 0; it < data.ba->size; it++)
				ByteArray.push_back(static_cast<uchar*>(
					data.ba->data)[it]);
			break;
		case Fmt::NodeArray:
			Array.reserve(list->num);
			for (it = 0; it < list->num; it++)
				Array.push_back(list->values[it]);
			break;
		case Fmt::NodeMap:
			for (it = 0; it < list->num; it++)
				Map[list->keys[it]] = list->values[it];
			break;
		case Fmt::OSDString:
		case Fmt::String:
			String = data.string;
			break;
		case Fmt::Double:
			Double = data.double_;
			break;
		case Fmt::Int:
			Int = data.int64;
			break;
		default:
			break;
		}

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

		right.Format = Fmt::None;
		right.Double = 0.0;
		right.Bool = false;
		right.Int = 0;

		return *this;
	}

	inline mpv_node_list* Node::translateArray() const
	{
		ulong index = 0;
		Listptr->keys = nullptr;
		Listptr->num = static_cast<int>(Array.size());
		const_cast<keyptr*>(&Keyptr)->reset(new char* [Listptr->num]);
		const_cast<nodeptr*>(&Valueptr)->reset(new mpv_node[Listptr->num]);

		for (auto it = Array.begin(); it != Array.end(); it++)
		{
			index = std::distance(Array.begin(), it);
			Valueptr.operator->()[index] = it->translate();
		}

		Listptr->values = Valueptr.get();
		return Listptr.get();
	}

	inline mpv_node_list* Node::translateMap() const
	{
		ulong index = 0;
		Listptr->num = static_cast<int>(Map.size());
		const_cast<keyptr*>(&Keyptr)->reset(new char* [Listptr->num]);
		const_cast<nodeptr*>(&Valueptr)->reset(new mpv_node[Listptr->num]);

		for (auto it = Map.begin(); it != Map.end(); it++)
		{
			std::pair<std::string, Node> temp = *it;
			index = std::distance(Map.begin(), it);
			Valueptr.operator->()[index] = temp.second.translate();
			Keyptr.operator->()[index] = const_cast<char*>(it->first.data());
		}

		Listptr->values = Valueptr.get();
		Listptr->keys = Keyptr.get();
		return Listptr.get();
	}

	inline void Node::clearPtr() const
	{
		Listptr->num = 0;
		*Nodeptr = mpv_node{};

		Listptr->keys = nullptr;
		Listptr->values = nullptr;
	}
}
