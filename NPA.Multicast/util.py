# !usr/bin/env python
# -*- coding:utf-8 -*-

"""
从构建树的角度出发
每台主机是一个Node
这个 Node需要提供的服务有：接收信息、发送信息、以自身为根节点构建组播树、并且知晓自己的 parent节点
整个组播树的根节点需要维护整个树的结构

class Node

    每个Node可以是自由的、也可以属于某个组
    一台主机可以运行多个Node，多个Node属于不同的组
    Node的行为仅针对组播树

    data 当前组播树的根节点
    data 子节点列表
    data 父节点
    data 为组播树服务的后台服务器，加入组之后启动，负责接收/转发根节点发来的消息、处理根节点发来的消息
         例如根节点发来消息，要求该节点向某台主机发送邀请，如果该邀请被接受，则那台主机成为一个新的子节点
         如果该节点自身就是组播树的根节点，则也要负责接收、发送相应的数据（承担自己的责任）
    data 一个简易的 chat客户端，可以发送/接收消息

    ### 规定：如果当前不在任何组，然后邀请一台主机加入，则该结点成为组播树的根节点 ###

    func join      向某个组的根节点发送加入组的申请
    func leave     注意：leave后，要处理好自己的子节点
    func invite    向某台主机发送邀请
    func startBgServerForMulticastTree   加入组后启动组播树后台服务器
    func startChatClient     加入组后启动，加入一个聊天室，室内的成员是组播树的成员
    func multicast    整个树内广播

    ### 信息类型规定：
        1. invite
        2. join
        3. leave
        4. multicast (向整棵组播树）
        5. message 普通信息
    ###

class BaseServer:
    data recvbuf
    data sendbuf
    data sendthread
    data recvthread

    func run

class MtBgServer(BaseServer):
    func process         # 处理跟组播树有关的信息
"""

