# Libmpvcpp 中文版本

## 介绍

Libmpvcpp 是基于mpv媒体库所提供的接口进行封装的C++框架, 旨在避免指针操作, 同时确保组件的生命周期可管控。  

为提高兼容性, 该框架以CMake作为平台, 使用C++11的特性编写。  

基于提供的接口, 本框架分为两个部分, 为 client 部分和 render 部分。client 部分用于基础的媒体播放控制, render 部分用于控制媒体的渲染层。

关于 client 部分, 则分为功能部分和代码部分, 其中功能部分为以下五类:  
1. 命令行类 Command
1. 属性类 Property
1. 句柄类 Handle
1. 事件类 Event
1. 容器类 Node

同时还有反馈代码部分, 该部分共有五类:  
1. 文件结束原因 EndFileReason
1. 日志等级 LogLevel
1. 事件编号 EventID
1. 参数类型 Format
1. 错误代码 Error

关于 render 部分, 则分为五类:  
1. 渲染帧信息类别 RenderFrameInfoType
1. 渲染参数类别 RenderParamInfo
1. 渲染帧信息 RenderFrameInfo
1. 渲染上下文 RenderContext
1. 渲染参数 RenderParam

处于**安全**考虑, 该框架不对所提供的**媒体流回调接口**部分进行封装。

## 使用文档

### client 部分
1. [命令行类 Command](./Chinese/client/Command.md)
1. [属性类 Property](./Chinese/client/Property.md)
1. [句柄类 Handle](./Chinese/client/Handle.md)
1. [事件类 Event](./Chinese/client/Event.md)
1. [容器类 Node](./Chinese/client/Node.md)
1. [反馈代码部分](./Chinese/client/Code.md)

### render 部分
1. [渲染帧信息类别 RenderFrameInfoType](./Chinese/render/RenderFrameInfoType.md)
1. [渲染参数类别 RenderParamInfo](./Chinese/render/RenderParamInfo.md)
1. [渲染帧信息 RenderFrameInfo](./Chinese/render/RenderFrameInfo.md)
1. [渲染上下文 RenderContext](./Chinese/render/RenderContext.md)
1. [渲染参数 RenderParam](./Chinese/render/RenderParam.md)
