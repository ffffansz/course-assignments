# !usr/bin/env python
# -*- coding:utf-8 -*-

import random
import socket
import time


class Robot:

    robots_num = 0            # 机器人总数量
    registered_robot = []     # 注册过的机器人

    login_num = 0             # 执行登陆的次数
    signUp_num = 0
    notice_num = 0
    requestRank_num = 0
    loginDelay_sum = 0.       # 登陆延迟的总和
    signUpDelay_sum = 0.
    noticeDelay_sum = 0.
    requestRankDelay_sum = 0.

    connect_num = 0           # 尝试连接的次数
    connect_failure_num = 0   # 连接失败的次数
    connectDelay_sum = 0.     # 连接延迟总和
    aborted_robot_num = 0     # 尝试连接十次未连接成功的机器人数量
    # averageLoginDelay = 0
    # averageSignUpDelay = 0
    # averageNoticeDelay = 0
    # averageRequestRankDelay = 0

    def __init__(self, targetServer, randomActionsCnt):
        Robot.robots_num += 1
        self.targetServer = targetServer     # a tuple (host, port)
        self.username = 'tester_' + str(random.randint(0, 10000))
        self.password = self.username
        self.registered_before = True
        if self.username not in Robot.registered_robot:
            self.registered_before = False
            Robot.registered_robot.append(self.username)
        self.randomActionCnt = randomActionsCnt
        self.actions = list()
        self.sock = socket.socket()
        self.connected = False
        self.sid = None
        # self.registered = False
        # self.loginDelay = 0
        # self.signUpDelay = 0
        # self.noticeDelay = 0
        # self.requestRankDelay = 0

    def initialize(self):
        # 初始化当前robot的动作序列

        if not self.registered_before:
            self.actions.append(self.signUp)
        self.actions.append(self.login)

        # randomActionCnt次随机动作，可选动作包括：notice, requestRank, singleGame
        for i in range(self.randomActionCnt):
            op = random.randint(0, 2)
            if op == 0:
                self.actions.append(self.notice)
            elif op == 2:
                self.actions.append(self.requestRank)
            elif op == 3:
                self.actions.append(self.singleGame)

        self.actions.append(self.logout)

    @staticmethod
    def resetData():
        # 重置统计数据
        Robot.robots_num = 0  # 机器人总数量
        Robot.registered_robot = []  # 注册过的机器人

        Robot.login_num = 0  # 执行登陆的次数
        Robot.signUp_num = 0
        Robot.notice_num = 0
        Robot.requestRank_num = 0
        Robot.loginDelay_sum = 0.  # 登陆延迟的总和
        Robot.signUpDelay_sum = 0.
        Robot.noticeDelay_sum = 0.
        Robot.requestRankDelay_sum = 0.

        Robot.connect_num = 0  # 尝试连接的次数
        Robot.connect_failure_num = 0  # 连接失败的次数
        Robot.connectDelay_sum = 0.  # 连接延迟总和
        Robot.aborted_robot_num = 0  # 尝试连接十次未连接成功的机器人数量

    @staticmethod
    def exportData():
        # 导出统计数据，返回类型：Dict
        return {
            'Robots_num'
            'SignUp_num': Robot.signUp_num,
            'Login_num': Robot.login_num,
            'Notice_num': Robot.notice_num,
            'Connect_num': Robot.connect_num,
            'AvgSignUpDelay': Robot.signUpDelay_sum / Robot.signUp_num,
            'AvgLoginDelay': Robot.loginDelay_sum / Robot.login_num,
            'AvgNoticeDelay': Robot.noticeDelay_sum / Robot.notice_num,
            'AvgRequestRankDelay': Robot.requestRankDelay_sum / Robot.requestRank_num,
            'AvgConnectFailure_num': float(Robot.connect_failure_num) / Robot.connect_num,
            'AvgSuccessfulConnectDelay': Robot.connectDelay_sum / (Robot.connect_num - Robot.connect_failure_num)
            }

    def run(self):
        # start running the robot

        reconnect_cnt = 0
        while (not self.connected) and (reconnect_cnt is not 10):
            self.connect()
            if self.connected:
                break
            else:
                reconnect_cnt += 1
                time.sleep(3)       # 每隔3秒重新尝试连接
        if reconnect_cnt is 10:
            Robot.connect_num -= 10
            Robot.connect_failure_num -= 1
            Robot.aborted_robot_num += 1
            return
        for action in self.actions:
            action()

    def connect(self):
        if self.connected:
            return True
        Robot.connect_num += 1
        try:
            self.sock.connect(self.targetServer)
        except:
            Robot.connect_failure_num += 1
            return False
        self.connected = True
        return True

    def login(self):
        pass

    def signUp(self):
        pass

    def singleGame(self):
        # send several recent scores and then send final score which means game over
        pass

    def logout(self):
        pass

    def notice(self):
        pass

    def requestRank(self):
        pass