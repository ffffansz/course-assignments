# !usr/bin/env python
# -*- coding:utf-8 -*-

import robot
import json
import os

def VPrint(content):
    print json.dumps(content, encoding='utf-8', ensure_ascii=False, indent=1)

def formatPrint(dict, numOfRetries):
    # 格式化输出统计数据
    mat = '{:32}{:20}{:'
    print mat.format('统计数据项', '数值', '单位')
    print mat.format('机器人数量', dict['RobotsCnt'])

    # Connect Info
    print '----------------------------------------'
    print mat.format('Connect次数', dict['ConnectCnt'])
    print mat.format('ConnectFailure次数', dict['ConnectionFailureCnt'])
    print mat.format('ConnectAbort的机器人数量', dict['ConnectAbortCnt'])

    print mat.format('Connect成功平均所需的连接次数',
        round(dict['ConnectCnt'] - numOfRetries * dict['ConnectAbortCnt']  / dict['RobotsCnt'] - dict['ConnectAbortCnt'], 2))
    print mat.format('平均Connect响应延迟', round(dict['ConnectDelaySum'] / dict['ConnectCnt'], 2))
    print mat.format('最大Connect响应延迟', round(dict['MaxConnectDelay'], 2))
    print mat.format('最小Connect响应延迟', round(dict['MinConnectDelay'], 2))

    # Recv SID Info
    print '----------------------------------------'
    print mat.format('RecvSID次数', dict['RecvSidCnt'])
    # print mat.format('RecvSIDFailure次数', dict['RecvSidFaliureCnt'])
    print mat.format('RecvSIDAbort的机器人数量', dict['RecvSidAbortCnt'])
    print mat.format('平均RecvSID延迟', round(dict['RecvSidDelaySum'] / dict['RecvSidCnt']), 2)
    print mat.format('最大RecvSID延迟', round(dict['MaxRecvSidDelay'], 2))
    print mat.format('最小RecvSID延迟', round(dict['MinRecvSidDelay'], 2))

    # Sign up Info
    print '----------------------------------------'
    print mat.format()

def main():
    if os.path.exists('F:\DevFiles\course-assignments\MIPP.FlappyBird\Modified\FlappyBirdServer\user.dat'):
        os.remove('F:\DevFiles\course-assignments\MIPP.FlappyBird\Modified\FlappyBirdServer\user.dat')
    host = "127.0.0.1"
    port = 9234
    nameRange = 1000
    robotsNum = 500

    # 生成robotsNum个机器人，机器人的用户名取值范围是(0, nameRange)
    robots = [robot.Robot((host, port), nameRange) for i in range(robotsNum)]
    for r in robots:
        r.initActionsSeq(50)
    for r in robots:
        r.run(50)
    for r in robots:
        r.shutdown()
    ret = exportData()
    VPrint(ret)

def exportData():
    return robot.Robot.exportData()


if __name__ == '__main__':
    main()
