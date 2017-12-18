# !usr/bin/env python
# -*- coding:utf-8 -*-

import random
import socket
import time
import netstream
import threading

INF = float(pow(2, 32))

class Robot:

    RobotsCnt = 0                     # 生成的机器人总数
    RegisteredRobotsName = list()     # 已注册过的账户名

    # connect to server
    ConnectCnt = 0                    # 总连接次数
    ConnectSuccessCnt = 0          # 连接成功的次数 = 连接成功的机器人数量
    ConnectDelaySum = 0.              # 连接延迟的总和
    MinConnectDelay = INF             # 最小连接延迟
    MaxConnectDelay = -INF            # 最大连接延迟

    # receive sid
    RecvSidCnt = 0
    RecvSidSuccessCnt = 0
    RecvSidDelaySum = 0.
    MinRecvSidDelay = INF
    MaxRecvSidDelay = -INF

    # Sign up or login
    SignUpOrLoginCnt = 0             # 注册或登录的总次数
    SignUpOrLoginSuccessCnt = 0      # 成功次数
    SignUpOrLoginDelaySum = 0.
    MinSignUpOrLoginDelay = INF
    MaxSignUpOrLoginDelay = -INF

    # Notice
    NoticeCnt = 0
    NoticeSuccessCnt = 0
    NoticeDelaySum = 0.
    MinNoticeDelay = INF
    MaxNoticeDelay = -INF

    # Request rank
    RequestRankCnt = 0
    RequestRankSuccessCnt = 0
    RequestRankDelaySum = 0.
    MinRequestRankDelay = INF
    MaxRequestRankDelay = -INF

    MissionCompRobotsCnt = 0         # 正常完成所有任务的机器人

    Mutex = threading.Lock()  # 互斥访问Robot的变量

    def __init__(self, targetServer, robotsNameRange):
        Robot.RobotsCnt += 1
        self.targetServer = targetServer     # a tuple (host, port)
        self.username = 'tester_' + str(random.randint(0, robotsNameRange))
        self.password = self.username
        self.registered_before = True
        if self.username not in Robot.RegisteredRobotsName:
            self.registered_before = False
            Robot.RegisteredRobotsName.append(self.username)
        self.actions = list()
        self.sock = socket.socket()
        self.connected = False
        self.sid = None
        self.errcode = ''               # Robot运行状态
        

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

    @staticmethod
    def resetData():
        # 重置统计数据
        Robot.Mutex.acquire()
        Robot.RobotsCnt = 0  # 生成的机器人总数
        Robot.RegisteredRobotsName = list()  # 已注册过的账户名

        # connect to server
        Robot.ConnectCnt = 0  # 总连接次数
        Robot.ConnectSuccessCnt = 0  # 连接成功的次数 = 连接成功的机器人数量
        Robot.ConnectDelaySum = 0.  # 连接延迟的总和
        Robot.MinConnectDelay = INF  # 最小连接延迟
        Robot.MaxConnectDelay = -INF  # 最大连接延迟

        # receive sid
        Robot.RecvSidCnt = 0
        Robot.RecvSidSuccessCnt = 0
        Robot.RecvSidDelaySum = 0.
        Robot.MinRecvSidDelay = INF
        Robot.MaxRecvSidDelay = -INF

        # Sign up or login
        Robot.SignUpOrLoginCnt = 0  # 注册或登录的总次数
        Robot.SignUpOrLoginSuccessCnt = 0  # 成功次数
        Robot.SignUpOrLoginDelaySum = 0.
        Robot.MinSignUpOrLoginDelay = INF
        Robot.MaxSignUpOrLoginDelay = -INF

        # Notice
        Robot.NoticeCnt = 0
        Robot.NoticeSuccessCnt = 0
        Robot.NoticeDelaySum = 0.
        Robot.MinNoticeDelay = INF
        Robot.MaxNoticeDelay = -INF

        # Request rank
        Robot.RequestRankCnt = 0
        Robot.RequestRankSuccessCnt = 0
        Robot.RequestRankDelaySum = 0.
        Robot.MinRequestRankDelay = INF
        Robot.MaxRequestRankDelay = -INF

        Robot.MissionCompRobotsCnt = 0
        Robot.Mutex.release()

    @staticmethod
    def exportData():
        # 导出统计数据，返回类型：Dict
        Robot.Mutex.acquire()
        ret =  {
            'RobotsCnt': Robot.RobotsCnt,
            # 'RegisteredRobotsName': Robot.RegisteredRobotsName,

            # connect to server
            'ConnectCnt': Robot.ConnectCnt,
            'ConnectSuccessCnt': Robot.ConnectSuccessCnt,
            'ConnectDelaySum': Robot.ConnectDelaySum,
            'MinConnectDelay': Robot.MinConnectDelay,
            'MaxConnectDelay': Robot.MaxConnectDelay,

            # receive sid
            'RecvSidCnt': Robot.RecvSidCnt,
            'RecvSidSuccessCnt': Robot.RecvSidSuccessCnt,
            'RecvSidDelaySum': Robot.RecvSidDelaySum,
            'MinRecvSidDelay': Robot.MinRecvSidDelay,
            'MaxRecvSidDelay': Robot.MaxRecvSidDelay,

            # Sign up or login
            'SignUpOrLoginCnt': Robot.SignUpOrLoginCnt,
            'SignUpOrLoginSuccessCnt': Robot.SignUpOrLoginSuccessCnt,
            'SignUpOrLoginDelaySum': Robot.SignUpOrLoginDelaySum,
            'MinSignUpOrLoginDelay': Robot.MinSignUpOrLoginDelay,
            'MaxSignUpOrLoginDelay': Robot.MaxSignUpOrLoginDelay,

            # Notice
            'NoticeCnt': Robot.NoticeCnt,
            'NoticeSuccessCnt': Robot.NoticeSuccessCnt,
            'NoticeDelaySum': Robot.NoticeDelaySum,
            'MinNoticeDelay': Robot.MinNoticeDelay,
            'MaxNoticeDelay': Robot.MaxNoticeDelay,

            # Request rank
            'RequestRankCnt': Robot.RequestRankCnt,
            'RequestRankSuccessCnt': Robot.RequestRankSuccessCnt,
            'RequestRankDelaySum': Robot.RequestRankDelaySum,
            'MinRequestRankDelay': Robot.MinRequestRankDelay,
            'MaxRequestRankDelay': Robot.MaxRequestRankDelay,
            'MissionCompRobotsCnt': Robot.MissionCompRobotsCnt
        }
        Robot.Mutex.release()
        return ret

    def run(self, numOfRetries):
        # start running the robot
        for i in range(len(self.actions)):
            actRet = self.actions[i](numOfRetries)
            if actRet is not True:
                self.errcode = actRet
                break
        if self.errcode is '':
            Robot.Mutex.acquire()
            Robot.MissionCompRobotsCnt += 1
            Robot.Mutex.release()

    def connect(self, numOfRetries):
        if self.connected:
            return True
        # start the timer
        end = None
        start = time.time()
        for i in range(numOfRetries):
            Robot.Mutex.acquire()
            Robot.ConnectCnt += 1
            Robot.Mutex.release()
            try:
                self.sock.connect(self.targetServer)
                self.connected = True
                break
            except:
                time.sleep(0.1)
                pass     # continue to retry

        if self.connected is False:
            # Robot.ConnectAbortCnt += 1
            return 'ConnectError'
        else:
            end = time.time()
            delay = end - start
            Robot.Mutex.acquire()
            Robot.ConnectSuccessCnt += 1
            Robot.ConnectDelaySum += delay
            Robot.MinConnectDelay = min(Robot.MinConnectDelay, delay)
            Robot.MaxConnectDelay = max(Robot.MaxConnectDelay, delay)
            Robot.Mutex.release()
            return True

    def recvsid(self, numOfRetries):
        end = None
        # start the timer
        start = time.time()
        for i in range(numOfRetries):
            Robot.Mutex.acquire()
            Robot.RecvSidCnt += 1
            Robot.Mutex.release()
            rd = self.recv()
            if rd == netstream.CLOSED:
                break
            elif rd == netstream.EMPTY or rd == netstream.TIMEOUT:
                time.sleep(0.1)
                continue
            elif 'sid' in rd:
                # end the timer
                end = time.time()
                self.sid = rd['sid']
                break
        if end is None:
            # Robot.RecvSidAbortCnt += 1
            return 'RecvSidError'
        else:
            delay = end - start
            Robot.Mutex.acquire()
            Robot.RecvSidSuccessCnt += 1
            Robot.RecvSidDelaySum += delay
            Robot.MinRecvSidDelay = min(Robot.MinRecvSidDelay, delay)
            Robot.MaxRecvSidDelay = max(Robot.MaxRecvSidDelay, delay)
            Robot.Mutex.release()
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
            Robot.Mutex.acquire()
            Robot.SignUpOrLoginCnt += 1
            Robot.Mutex.release()
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
                time.sleep(0.1)
                continue
            elif rd['type'] == 'loginResult' and rd['result'] == 'success':
                # end the timer
                end = time.time()
                break
        if end is None:
            # Robot.LoginAbortCnt += 1
            return 'LoginError'
        else:
            delay = end - start
            Robot.Mutex.acquire()
            Robot.SignUpOrLoginSuccessCnt += 1
            Robot.SignUpOrLoginDelaySum += delay
            Robot.MinSignUpOrLoginDelay = min(Robot.MinSignUpOrLoginDelay, delay)
            Robot.MaxSignUpOrLoginDelay = max(Robot.MaxSignUpOrLoginDelay, delay)
            Robot.Mutex.release()
            return True

    def signUp(self, numOfRetries):
        end = None
        # start the timer
        start = time.time()
        rd = None
        for i in range(numOfRetries):
            Robot.Mutex.acquire()
            Robot.SignUpOrLoginCnt += 1
            Robot.Mutex.release()
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
                time.sleep(0.1)
                continue
            elif rd['type'] == 'signUpResult' and rd['result'] == 'success':
                # end the timer
                end = time.time()
                break
        if end is None:
            # Robot.SignUpAbortCnt += 1
            return 'SignUpError'
        else:
            delay = end - start
            Robot.Mutex.acquire()
            Robot.SignUpOrLoginSuccessCnt += 1
            Robot.SignUpOrLoginDelaySum += delay
            Robot.MinSignUpOrLoginDelay = min(Robot.MinSignUpOrLoginDelay, delay)
            Robot.MaxSignUpOrLoginDelay = max(Robot.MaxSignUpOrLoginDelay, delay)
            Robot.Mutex.release()
            return True

    def singleGame(self, numOfRetries):
        # send several recent scores and then send final score which means game over
        return True

    def shutdown(self):
        # disconnect from the server
        self.sock.close()

    def notice(self, numOfRetries):
        end = None
        start = time.time()
        for i in range(numOfRetries):
            Robot.Mutex.acquire()
            Robot.NoticeCnt += 1
            Robot.Mutex.release()
            sd = {
                'sid': self.sid,
                'type': "notice"
            }
            netstream.send(self.sock, sd)
            rd = self.recv()
            if rd == netstream.CLOSED:
                break
            elif rd == netstream.EMPTY or rd == netstream.TIMEOUT:
                time.sleep(0.1)
                continue
            elif rd['type'] == 'notice' and rd['content'] == 'Sever is connected':
                # end the timer
                end = time.time()
                break
        if end is None:
            # Robot.NoticeAbortCnt += 1
            return 'NoticeError'
        else:
            delay = end - start
            Robot.Mutex.acquire()
            Robot.NoticeSuccessCnt += 1
            Robot.NoticeDelaySum += delay
            Robot.MinNoticeDelay = min(Robot.MinNoticeDelay, delay)
            Robot.MaxNoticeDelay = max(Robot.MaxNoticeDelay, delay)
            Robot.Mutex.release()
            return True

    def requestRank(self, numOfRetries):
        end = None
        # start the timer
        start = time.time()
        for i in range(numOfRetries):
            Robot.Mutex.acquire()
            Robot.RequestRankCnt += 1
            Robot.Mutex.release()
            sd = {
                'sid': self.sid,
                'type': "notice",
            }
            netstream.send(self.sock, sd)
            rd = self.recv()
            if rd == netstream.CLOSED:
                break
            elif rd == netstream.EMPTY or rd == netstream.TIMEOUT:
                time.sleep(0.1)
                continue
            elif rd['type'] == 'notice' and rd['content'] == 'Sever is connected':
                # end the timer
                end = time.time()
                break
        if end is None:
            # Robot.RequestRankAbortCnt += 1
            return 'RequestRankError'
        else:
            delay = end - start
            Robot.Mutex.acquire()
            Robot.RequestRankSuccessCnt += 1
            Robot.RequestRankDelaySum += delay
            Robot.MinRequestRankDelay = min(Robot.MinRequestRankDelay, delay)
            Robot.MaxRequestRankDelay = max(Robot.MaxRequestRankDelay, delay)
            Robot.Mutex.release()
            return True
