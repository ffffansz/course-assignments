# !usr/bin/env python
# -*- coding:utf-8 -*-

import robot
import json
import os
import threading

host = "127.0.0.1"
port = 9234

def VPrint(content):
    print json.dumps(content, encoding='utf-8', ensure_ascii=False, indent=1)

def formatPrint(data):
    # 格式化输出统计数据
    mat = '{:<40}\t{:<20}'
    print mat.format('Item', 'Data')
    print
    print mat.format('RobotsNum', data['RobotsCnt'])

    # Connect Info
    print '----------------------------------------'
    # Connect成功的机器人数量
    print mat.format('ConnectSuccessNum', data['ConnectSuccessCnt'])
    # Connect成功率
    print mat.format('ConnectSuccessRate', float(data['ConnectSuccessCnt']) / data['ConnectCnt'], 5)
    # 最大Connect耗时
    print mat.format('MaxConnectDelay', data['MaxConnectDelay'])
    # 最小Connect耗时
    print mat.format('MinConnectDelay', data['MinConnectDelay'])
    # 平均Connect耗时（仅针对成功的Connect，下同）
    print mat.format('AvgConnectDelay', data['ConnectDelaySum'] / data['ConnectSuccessCnt'])

    # Recv SID Info
    print '----------------------------------------'
    print mat.format('RecvSIDSuccessNum', data['RecvSidSuccessCnt'])
    print mat.format('RecvSIDSuccessRate', float(data['RecvSidSuccessCnt']) / data['RecvSidCnt'])
    print mat.format('MaxRecvSIDDelay', data['MaxRecvSidDelay'])
    print mat.format('MinRecvSIDDelay', data['MinRecvSidDelay'])
    print mat.format('AvgRecvSIDDelay', data['RecvSidDelaySum'] / data['RecvSidSuccessCnt'])

    # Sign up or Login Info
    print '----------------------------------------'
    print mat.format('SignUp/LoginSuccessNum', data['SignUpOrLoginSuccessCnt'])
    print mat.format('SignUp/LoginSuccessRate', float(data['SignUpOrLoginSuccessCnt']) / data['SignUpOrLoginCnt'])
    print mat.format('MaxSignUp/LoginDelay', data['MaxSignUpOrLoginDelay'], 5)
    print mat.format('MinSignUp/LoginDelay', data['MinSignUpOrLoginDelay'])
    print mat.format('AvgSignUp/LoginDelay', data['SignUpOrLoginDelaySum'] / data['SignUpOrLoginSuccessCnt'])

    # Notice Info
    print '----------------------------------------'
    print mat.format('NoticeSuccessRate', float(data['NoticeSuccessCnt']) / data['NoticeCnt'])
    print mat.format('MaxNoticeDelay', data['MaxNoticeDelay'])
    print mat.format('MinNoticeDelay', data['MinNoticeDelay'])
    print mat.format('AvgNoticeDelay', data['NoticeDelaySum'] / data['NoticeSuccessCnt'])

    # Request Rank Info
    print '----------------------------------------'
    print mat.format('RequestRankSuccessRate', float(data['RequestRankSuccessCnt']) / data['RequestRankCnt'])
    print mat.format('MaxRequestRankDelay', data['MaxRequestRankDelay'])
    print mat.format('MinRequestRankDelay', data['MinRequestRankDelay'])
    print mat.format('AvgRequestRankDelay', data['RequestRankDelaySum'] / data['RequestRankSuccessCnt'])

    print '----------------------------------------'
    print mat.format('MissionCompleteNum', data['MissionCompRobotsCnt'])
    print mat.format('MissionCompleteRate', float(data['MissionCompRobotsCnt']) / data['RobotsCnt'])


def serverTester(nameRange, randomActionsNum, eachActionRetries):
    r = robot.Robot((host, port), nameRange)
    r.initActionsSeq(randomActionsNum)
    r.run(eachActionRetries)
    r.shutdown()
    return

def main():
    if os.path.exists('F:\DevFiles\course-assignments\MIPP.FlappyBird\Modified\FlappyBirdServer\user.dat'):
        os.remove('F:\DevFiles\course-assignments\MIPP.FlappyBird\Modified\FlappyBirdServer\user.dat')

    nameRange = 1000
    robotsNum = 800
    randomActionsNum = 50
    eachActionRetries = 512

    # 生成robotsNum个机器人，机器人的用户名会在(0, nameRange)取一个随机数
    robotsThreads = [threading.Thread(target=serverTester, args=(nameRange, randomActionsNum, eachActionRetries))
                     for i in range(robotsNum)]

    # 启动所有线程
    for t in robotsThreads:
        t.start()

    # 等待所有机器人运行完毕
    for t in robotsThreads:
        t.join()
    ret = exportData()
    formatPrint(ret)

def exportData():
    return robot.Robot.exportData()

if __name__ == '__main__':
    main()
