# Libmpvcpp

## 个人用mpv媒体播放库C++框架

---

## 依赖

框架依赖 ***mpv*** 媒体播放库(版本 0.36.0)  

使用 C++ 14 编写  

---

## 基本

框架对使用频率高的结构体及函数进行封装, 尽可能使用错误码以规避异常抛出  

使用 **mpv** 作为命名空间  

主要有以下封装类:  

| 类别(mpv::~) | 说明 |
| :---:	| :---:	|
| Node | 框架中的集成数据容器 |
| Event | 框架中的事件容器 |
| Handle | 播放底层的句柄 |
| Command | 命令函数集	|
| Property | 属性设定及获取函数集 |

## 用法

函数用法与官方样例类似  

构造 Handle 类的对象, 对对象创建实例及初始化实例:  

```
#include"mpvcpp/Command.hpp"

mpv::Handle handle;
handle.create();
handle.initialize();
```

调用 Command 类中的函数, 传入参数:  

```
mpv::Command::Sync(handle, {
	"loadfile",
	"(filename)"
	});
```

即可播放媒体("filename"处为所需播放的文件)  

---

## 其它

[开发文档(施工中...)](Libmpvcpp/document/Document.md)  
