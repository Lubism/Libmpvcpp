# Command 类  

## 类型注明  

```
String = std::string;
ulong = unsigned long long;
Args = std::vector<std::string>;
charArgs = std::unique_ptr<const char*>;
```

## <h2 id = "0">函数列表</h2>

[Sync(const Handle&, const Args&): static inline code::Error](#1)  

[SyncNode(const Handle&, Node, Node&): static inline code::Error](#2)  

[SyncString(const Handle&, const String&): static inline code::Error](#3)  

[Async(const Handle&, const Args&, ulong): static inline code::Error](#4)  

[AsyncNode(const Handle&, Node, ulong): static inline code::Error](#5)  

[Acquire(const Handle&, const Args&, Node&): static inline code::Error](#6)  

[AbortAsync(const Handle&, ulong): static inline code::Error](#7)  

## 函数介绍及用法

### <h2 id = "1">Sync(const Handle& handle, const Args& value)</h2>

执行同步字符串集命令, 执行后返回错误码  

#### 用法

```
std::vector<std::string> arg = 
	{
		"loadfile",
		(filename),
	};

code::Error error = code::Error::Success;
error = Command::Sync(handle, arg);
error = Command::Sync(handle,
	{
		"loadfile",
		(filename),
	});
```

### <h2 id = "2">SyncNode(const Handle& handle, Node args, Node& result)</h2>

执行同步的容器所承载的命令, 函数执行完毕前会对当前线程阻塞, 执行后返回错误码  

#### 用法

```
```

### <h2 id = "3">SyncString(const Handle& handle, const String& stringArgs)</h2>

执行字符串命令, 函数执行完毕前会对当前线程阻塞, 关键字需要手动分割  

#### 用法

```
```

### <h2 id = "4">Async(const Handle& handle, const Args& value, ulong replyData)</h2>

执行异步字符串集命令, 函数执行无阻塞, 执行后返回错误码  

#### 用法

```
unsigned long long ID = handle.clientID();
std::vector<std::string> arg = 
	{
		"loadfile",
		(filename),
	};

code::Error error = code::Error::Success;
error = Command::Sync(handle, arg);
error = Command::Sync(handle,
	{
		"loadfile",
		(filename),
	}, ID);
```

### <h2 id = "5">AsyncNode(const Handle& handle, Node, ulong replyData)</h2>

执行异步的容器所承载的命令, 函数执行无阻塞, 执行后返回错误码  

#### 用法

```
```

### <h2 id = "6">Acquire(const Handle& handle, const Args& value, Node& result)</h2>

向句柄核心发送请求命令, 并以容器承载请求结果, 函数执行完毕前会对当前线程阻塞, 执行后返回错误码  

#### 用法

```
```

### <h2 id = "7">AbortAsync(const Handle& handle, ulong replyData)</h2>

中止与反馈ID匹配的异步命令, 执行后返回错误码  

#### 用法

```
```
