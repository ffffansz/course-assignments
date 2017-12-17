# !usr/bin/env python
# -*- coding:utf-8 -*-

import robot
import json
import os

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
    print mat.format('ConnectSuccessRate', round(float(data['ConnectSuccessCnt']) / data['ConnectCnt'], 5))
    # 最大Connect耗时
    print mat.format('MaxConnectDelay', round(data['MaxConnectDelay'], 5))
    # 最小Connect耗时
    print mat.format('MinConnectDelay', round(data['MinConnectDelay'], 5))
    # 平均Connect耗时（仅针对成功的Connect，下同）
    print mat.format('AvgConnectDelay', round(data['ConnectDelaySum'] / data['ConnectSuccessCnt'], 5))

    # Recv SID Info
    print '----------------------------------------'
    print mat.format('RecvSIDSuccessNum', data['RecvSidSuccessCnt'])
    print mat.format('RecvSIDSuccessRate', round(float(data['RecvSidSuccessCnt']) / data['RecvSidCnt'], 5))
    print mat.format('MaxRecvSIDDelay', round(data['MaxRecvSidDelay'], 5))
    print mat.format('MinRecvSIDDelay', round(data['MinRecvSidDelay'], 5))
    print mat.format('AvgRecvSIDDelay', round(data['RecvSidDelaySum'] / data['RecvSidSuccessCnt']), 5)

    # Sign up or Login Info
    print '----------------------------------------'
    print mat.format('SignUp/LoginSuccessNum', data['SignUpOrLoginSuccessCnt'])
    print mat.format('SignUp/LoginSuccessRate', round(float(data['SignUpOrLoginSuccessCnt']) / data['SignUpOrLoginCnt'], 5))
    print mat.format('MaxSignUp/LoginDelay', round(data['MaxSignUpOrLoginDelay'], 5))
    print mat.format('MinSignUp/LoginDelay', round(data['MinSignUpOrLoginDelay'], 5))
    print mat.format('AvgSignUp/LoginDelay', round(data['SignUpOrLoginDelaySum'] / data['SignUpOrLoginSuccessCnt']), 5)

    # Notice Info
    print '----------------------------------------'
    print mat.format('NoticeSuccessRate', round(float(data['NoticeSuccessCnt']) / data['NoticeCnt'], 5))
    print mat.format('MaxNoticeDelay', round(data['MaxNoticeDelay'], 5))
    print mat.format('MinNoticeDelay', round(data['MinNoticeDelay'], 5))
    print mat.format('AvgNoticeDelay', round(data['NoticeDelaySum'] / data['NoticeSuccessCnt']), 5)

    # Request Rank Info
    print '----------------------------------------'
    print mat.format('RequestRankSuccessRate', round(float(data['RequestRankSuccessCnt']) / data['RequestRankCnt'], 5))
    print mat.format('MaxRequestRankDelay', round(data['MaxRequestRankDelay'], 5))
    print mat.format('MinRequestRankDelay', round(data['MinRequestRankDelay'], 5))
    print mat.format('AvgRequestRankDelay', round(data['RequestRankDelaySum'] / data['RequestRankSuccessCnt']), 5)

    print '----------------------------------------'
    print mat.format('MissionCompleteRate', round(float(data['MissionCompRobotsCnt']) / data['RobotsCnt']))

def main():
    if os.path.exists('F:\DevFiles\course-assignments\MIPP.FlappyBird\Modified\FlappyBirdServer\user.dat'):
        os.remove('F:\DevFiles\course-assignments\MIPP.FlappyBird\Modified\FlappyBirdServer\user.dat')
    host = "127.0.0.1"
    port = 9234
    nameRange = 1000
    robotsNum = 50
    randomActionsNum = 20
    eachActionRetries = 32

    # 生成robotsNum个机器人，机器人的用户名取值范围是(0, nameRange)
    robots = [robot.Robot((host, port), nameRange) for i in range(robotsNum)]
    for r in robots:
        r.initActionsSeq(randomActionsNum)
    for r in robots:
        r.run(eachActionRetries)
    for r in robots:
        r.shutdown()
    ret = exportData()
    formatPrint(ret)

def exportData():
    return robot.Robot.exportData()

if __name__ == '__main__':
    main()
