# CHAT

这个项目是一个服务器客户端架构的基础练习

大致流程:
客户端将指令打包成json格式发出，服务器解析json包，根据cmd来完成工作再将结果返回给客户端。

技术栈:
客户端:
QT 信号与槽

服务器:
jsoncpp
IO多路复用 libevent
Linux环境编程

reference:
《UNIX网络编程》
《APUE》
