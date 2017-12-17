# !usr/bin/env python
# -*- coding:utf-8 -*-

import random
import socket
import time
import netstream

INF = float(pow(2, 32))
ABORT = -1
TIMEOUT = -2

class Robot:

    RobotsCnt = 0                     # 生成的机器人总数
    RegisteredRobotsName = list()     # 已注册过的账户名

    # connect to server
    ConnectCnt = 0                    # 尝试连接的次数
    ConnectionFailureCnt = 0          # 连接失败的次数
    ConnectDelaySum = 0.              # 连接延迟的总和
    MinConnectDelay = INF             # 最小连接延迟
    MaxConnectDelay = -INF            # 最大连接延迟

    # receive sid
    RecvSidCnt = 0
    RecvSidFaliureCnt = 0
    RecvSidDelaySum = 0.
    MinRecvSidDelay = INF
    MaxRecvSidDelay = -INF

    # Sign up
    SignUpCnt = 0
    SignUpDelaySum = 0.
    MinSignUpDelay = INF
    MaxSignUpDelay = -INF

    # Log in
    LoginCnt = 0
    LoginDelaySum = 0.
    MinLoginDelay = INF
    MaxLoginDelay = -INF

    # Notice
    NoticeCnt = 0
    NoticeDelaySum = 0.
    MinNoticeDelay = INF
    MaxNoticeDelay = -INF

    # Request rank
    RequestRankCnt = 0
    RequestRankDelaySum = 0.
    MinRequestRankDelay = INF
    MaxRequestRankDelay = -INF

    # Aborted Robots Counter
    ConnectAbortCnt = 0                  # 在连接步骤上退出的Robot
    RecvSidAbortCnt = 0
    SignUpAbortCnt = 0                   # 在注册步骤上退出的Robot
    LoginAbortCnt = 0
    NoticeAbortCnt = 0
    RequestRankAbortCnt = 0

    def __init__(self, targetServer, robotsNameRange):
        Robot.RobotsCnt += 1
        self.targetServer = targetServer     # a tuple (host, port)
        self.username = 'tester_' + str(random.randint(0, robotsNameRange))
        self.password = self.username
        self.registered_before = True
        if self.username not in Robot.RegisteredRobotsName:
            self.registered_before = False
            Robot.RegisteredRobotsName.append(self.username)
        # self.randomActionCnt = randomActionsCnt
        self.actions = list()
        self.sock = socket.socket()
        self.connected = False
        self.sid = None
        self.errcode = ''          # Robot运行状态

    def initActionsSeq(self, randomActionsNum):
        # 初始化当前robot的动作序列
        self.actions.append(self.connect)
        self.actions.append(self.recvsid)
        if not self.registered_before:
            self.actions.append(self.signUp)
        else:
            self.actions.append(self.login)

        # randomActionsNum次随机动作，可选动作包括：notice, requestRank, singleGame
        for i in range(randomActionsNum):
            op = random.randint(0, 2)
            if op == 0:
                self.actions.append(self.notice)
            elif op == 1:
                self.actions.append(self.requestRank)
            elif op == 2:
                self.actions.append(self.singleGame)

        # self.actions.append(self.logout)
        # self.actions.append(self.shutdown)

    @staticmethod
    def resetData():
        # 重置统计数据
        Robot.RobotsCnt = 0
        Robot.RegisteredRobotsName = list()

        # connect to server
        Robot.ConnectCnt = 0
        Robot.ConnectionFailureCnt = 0
        Robot.ConnectSuccessCnt = 0
        Robot.ConnectDelaySum = 0.
        Robot.MinConnectDelay = INF
        Robot.MaxConnectDelay = -INF

        # receive sid
        Robot.RecvSidCnt = 0
        Robot.RecvSidFaliureCnt = 0
        Robot.RecvSidDelaySum = 0.
        Robot.MinRecvSidDelay = INF
        Robot.MaxRecvSidDelay = -INF

        # Sign up
        Robot.SignUpCnt = 0
        Robot.SignUpDelaySum = 0.
        Robot.MinSignUpDelay = INF
        Robot.MaxSignUpDelay = -INF

        # Log in
        Robot.LoginCnt = 0
        Robot.LoginDelaySum = 0.
        Robot.MinLoginDelay = INF
        Robot.MaxLoginDelay = -INF

        # Notice
        Robot.NoticeCnt = 0
        Robot.NoticeDelaySum = 0.
        Robot.MinNoticeDelay = INF
        Robot.MaxNoticeDelay = -INF

        # Request rank
        Robot.RequestRankCnt = 0
        Robot.RequestRankDelaySum = 0.
        Robot.MinRequestRankDelay = INF
        Robot.MaxRequestRankDelay = -INF

        # Aborted Robots Counter
        Robot.ConnectAbortCnt = 0
        Robot.RecvSidAbortCnt = 0
        Robot.SignUpAbortCnt = 0
        Robot.LoginAbortCnt = 0
        Robot.NoticeAbortCnt = 0
        Robot.RequestRankAbortCnt = 0

    @staticmethod
    def exportData():
        # 导出统计数据，返回类型：Dict
        return {
            'RobotsCnt': Robot.RobotsCnt,
            'RegisteredRobotsName': Robot.RegisteredRobotsName,

            # connect to server
            'ConnectCnt': Robot.ConnectCnt,
            'ConnectFailureCnt': Robot.ConnectionFailureCnt,
            'ConnectDelaySum': Robot.ConnectDelaySum,
            'MinConnectDelay': Robot.MinConnectDelay,
            'MaxConnectDelay': Robot.MaxConnectDelay,

            # receive sid
            'RecvSidCnt': Robot.RecvSidCnt,
            'RecvSidFaliureCnt': Robot.RecvSidFaliureCnt,
            'RecvSidDelaySum': Robot.RecvSidDelaySum,
            'MinRecvSidDelay': Robot.MinRecvSidDelay,
            'MaxRecvSidDelay': Robot.MaxRecvSidDelay,

            # Sign up
            'SignUpCnt': Robot.SignUpCnt,
            'SignUpDelaySum': Robot.SignUpDelaySum,
            'MinSignUpDelay': Robot.MinSignUpDelay,
            'MaxSignUpDelay': Robot.MaxSignUpDelay,

            # Log in
            'LoginCnt': Robot.LoginCnt,
            'LoginDelaySum': Robot.LoginDelaySum,
            'MinLoginDelay': Robot.MinLoginDelay,
            'MaxLoginDelay': Robot.MaxLoginDelay,

            # Notice
            'NoticeCnt': Robot.NoticeCnt,
            'NoticeDelaySum': Robot.NoticeDelaySum,
            'MinNoticeDelay': Robot.MinNoticeDelay,
            'MaxNoticeDelay': Robot.MaxNoticeDelay,

            # Request rank
            'RequestRankCnt': Robot.RequestRankCnt,
            'RequestRankDelaySum': Robot.RequestRankDelaySum,
            'MinRequestRankDelay': Robot.MinRequestRankDelay,
            'MaxRequestRankDelay': Robot.MaxRequestRankDelay,

            # Aborted Robots Counter
            'ConnectAbortCnt': Robot.ConnectAbortCnt,
            'RecvSidAbortCnt': Robot.RecvSidAbortCnt,
            'SignUpAbortCnt': Robot.SignUpAbortCnt,
            'LoginAbortCnt': Robot.LoginAbortCnt,
            'NoticeAbortCnt': Robot.NoticeAbortCnt,
            'RequestRankAbortCnt': Robot.RequestRankAbortCnt
        }

    def run(self, numOfRetries):
        # start running the robot
        for i in range(len(self.actions)):
            actRet = self.actions[i](numOfRetries)
            if actRet is not True:
                self.errcode = actRet
                break

    def connect(self, numOfRetries):
        if self.connected:
            return True
        # start the timer
        end = None
        start = time.time()
        for i in range(numOfRetries):
            Robot.ConnectCnt += 1
            try:
                self.sock.connect(self.targetServer)
                self.connected = True
                break
            except:
                Robot.ConnectionFailureCnt += 1

        if self.connected is False:
            Robot.ConnectAbortCnt += 1
            return 'ConnectError'
        else:
            end = time.time()
            delay = end - start
            Robot.ConnectDelaySum += delay
            Robot.MinConnectDelay = min(Robot.MinConnectDelay, delay)
            Robot.MaxConnectDelay = max(Robot.MaxConnectDelay, delay)
            return True

    def recvsid(self, numOfRetries):
        end = None
        # start the timer
        start = time.time()
        for i in range(numOfRetries):
            Robot.RecvSidCnt += 1
            rd = self.recv()
            if rd == netstream.CLOSED:
                break
            elif rd == netstream.EMPTY or rd == netstream.TIMEOUT:
                continue
            elif 'sid' in rd:
                # end the timer
                end = time.time()
                self.sid = rd['sid']
                break
        if end is None:
            Robot.RecvSidAbortCnt += 1
            return 'RecvSidError'
        else:
            delay = end - start
            Robot.RecvSidDelaySum += delay
            Robot.MinRecvSidDelay = min(Robot.MinRecvSidDelay, delay)
            Robot.MaxRecvSidDelay = max(Robot.MaxRecvSidDelay, delay)
            return True

    def recv(self):
        if not self.connected:
            return
        data = netstream.read(self.sock)
        return data
        # return CLOSED, TIMEOUT, EMPTY, or true data

    def login(self, numOfRetries):
        end = None
        # start the timer
        start = time.time()
        for i in range(numOfRetries):
            Robot.LoginCnt += 1
            sd = {
                'sid': self.sid,
                'type': "login",
                'username': self.username,
                'password': self.password
            }
            netstream.send(self.sock, sd)
            rd = self.recv()
            if rd == netstream.CLOSED:
                break
            elif rd == netstream.EMPTY or rd == netstream.TIMEOUT:
                continue
            elif rd['type'] == 'loginResult' and rd['result'] == 'success':
                # end the timer
                end = time.time()
                break
        if end is None:
            Robot.LoginAbortCnt += 1
            return 'LoginError'
        else:
            delay = end - start
            Robot.LoginDelaySum += delay
            Robot.MinLoginDelay = min(Robot.MinLoginDelay, delay)
            Robot.MaxLoginDelay = max(Robot.MaxLoginDelay, delay)
            return True

    def signUp(self, numOfRetries):
        end = None
        # start the timer
        start = time.time()
        rd = None
        for i in range(numOfRetries):
            Robot.SignUpCnt += 1
            sd = {
                'sid': self.sid,
                'type': "signUp",
                'username': self.username,
                'password': self.password
            }
            netstream.send(self.sock, sd)
            rd = self.recv()
            if rd == netstream.CLOSED:
                break
            elif rd == netstream.EMPTY or rd == netstream.TIMEOUT:
                continue
            elif rd['type'] == 'signUpResult' and rd['result'] == 'success':
                # end the timer
                end = time.time()
                break
        if end is None:

            # debug
            # print rd
            # debug

            Robot.SignUpAbortCnt += 1
            return 'SignUpError'
        else:
            delay = end - start
            Robot.SignUpDelaySum += delay
            Robot.MinSignUpDelay = min(Robot.MinSignUpDelay, delay)
            Robot.MaxSignUpDelay = max(Robot.MaxSignUpDelay, delay)
            return True

    def singleGame(self, numOfRetries):
        # send several recent scores and then send final score which means game over
        return True

    def logout(self):
        pass

    def shutdown(self):
        # disconnect from the server
        self.sock.close()

    def notice(self, numOfRetries):
        end = None
        start = time.time()
        for i in range(numOfRetries):
            Robot.NoticeCnt += 1
            sd = {
                'sid': self.sid,
                'type': "notice"
            }
            netstream.send(self.sock, sd)
            rd = self.recv()
            if rd == netstream.CLOSED:
                break
            elif rd == netstream.EMPTY or rd == netstream.TIMEOUT:
                continue
            elif rd['type'] == 'notice' and rd['content'] == 'Sever is connected':
                # end the timer
                end = time.time()
                break
        if end is None:
            Robot.NoticeAbortCnt += 1
            return 'NoticeError'
        else:
            delay = end - start
            Robot.NoticeDelaySum += delay
            Robot.MinNoticeDelay = min(Robot.MinNoticeDelay, delay)
            Robot.MaxNoticeDelay = max(Robot.MaxNoticeDelay, delay)
            return True

    def requestRank(self, numOfRetries):
        end = None
        # start the timer
        start = time.time()
        for i in range(numOfRetries):
            Robot.RequestRankCnt += 1
            sd = {
                'sid': self.sid,
                'type': "notice",
            }
            netstream.send(self.sock, sd)
            rd = self.recv()
            if rd == netstream.CLOSED:
                break
            elif rd == netstream.EMPTY or rd == netstream.TIMEOUT:
                continue
            elif rd['type'] == 'notice' and rd['content'] == 'Sever is connected':
                # end the timer
                end = time.time()
                break
        if end is None:
            Robot.RequestRankAbortCnt += 1
            return 'RequestRankError'
        else:
            delay = end - start
            Robot.RequestRankDelaySum += delay
            Robot.MinRequestRankDelay = min(Robot.MinRequestRankDelay, delay)
            Robot.MaxRequestRankDelay = max(Robot.MaxRequestRankDelay, delay)
            return True
