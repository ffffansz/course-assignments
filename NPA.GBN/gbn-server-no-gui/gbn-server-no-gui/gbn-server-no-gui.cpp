/* UDPServer.cpp */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string.h>
#include <wspiapi.h>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include "Datagram.h"


#define DEFAULT_BUFLEN 2048
#define WSVERS		MAKEWORD(2, 2)        // 指明版本2.2 
#pragma comment(lib,"ws2_32.lib")         // 加载winsock 2.2 Llibrary

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET sock;
	char   *port = NULL;  	/* bind port */
	struct sockaddr_in sin;	        /* an Internet endpoint address	*/
	struct sockaddr_in from;        /* sender address */
	int    fromsize = sizeof(from);
	NewDatagram recvBuf;   	    /* buffer for one line of text	*/
	char sendBuf[DEFAULT_BUFLEN + 1];
	int retval,
		bytecnt;
	unsigned ackSeq = -1;
	boost::shared_ptr<std::string> pRecvStr;
	boost::shared_ptr<NewDatagram>  pRecvDg;
	std::string recvDgSeqStr,
		recvAddr,
		recvPort;

	/* Initialize random number seed */
	boost::mt19937 rng(time(0));
	boost::uniform_01<boost::mt19937&> u01(rng);

	// Validate the parameters
	if (argc != 2) {
		printf("Usage: ./server.exe <port>\n");
		system("pause");
		return 1;
	}
	port = argv[1];

	// Initialize Winsock
	retval = WSAStartup(WSVERS, &wsaData);
	if (retval != 0) {
		fprintf(stderr, "WSAStartup failed with error %d\n", retval);
		WSACleanup();
		return -1;
	}

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); // 创建UDP套接字, 参数：因特网协议簇(family)，数据报套接字，UDP协议号， 返回：要监听套接字的描述符或INVALID_SOCKET
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;                     // 绑定(监听)所有的接口。
	sin.sin_port = htons((u_short)atoi(port));         // 绑定指定接口。atoi--把ascii转化为int，htons -- 主机序(host)转化为网络序(network), 为short类型。 
	bind(sock, (struct sockaddr *)&sin, sizeof(sin));     // 绑定本地端口号（和本地IP地址)
	std::cout << "Bind to port " << port << "." << std::endl;
	std::cout << "Running..." << std::endl;
	while (true) {                                    //检测是否有按键
		bytecnt = recvfrom(sock, (char*)&recvBuf, DEFAULT_BUFLEN, 0, (SOCKADDR *)&from, &fromsize);  //接收客户数据。返回结果：recvResult为接收的字符数，from中将包含客户IP地址和端口号。
		if (bytecnt == SOCKET_ERROR) {
			printf("recvfrom() failed; %d\n", WSAGetLastError());
			continue;
		}

		recvDgSeqStr = boost::lexical_cast<std::string>(recvBuf.seq_);
		recvAddr = std::string(inet_ntoa(from.sin_addr));
		recvPort = boost::lexical_cast<std::string>(from.sin_port);
		std::cout << "Recv-No. " + recvDgSeqStr
			+ " Receive No." + recvDgSeqStr + " datagram from "
			+ recvAddr + ":" << recvPort << std::endl;

		// Randomly send ACK
		if (u01() > 0.2) {
			boost::shared_ptr<std::string> str = boost::make_shared<std::string>(boost::lexical_cast<std::string>(recvBuf.seq_));
			sprintf_s(sendBuf, sizeof(sendBuf), str->c_str());
			bytecnt = sendto(sock, sendBuf, sizeof(str->c_str()), 0, (SOCKADDR *)&from, fromsize);
			if (bytecnt == SOCKET_ERROR) {
				fprintf(stderr, "sendto failed: %d\n", WSAGetLastError());
				break;
			}
			//show send info
			std::cout << "Send-No. " + recvDgSeqStr
				+ ": Send No." + recvDgSeqStr + " ACK to " + recvAddr + ":" << recvPort << std::endl;
		}
	}
	closesocket(sock);
	WSACleanup();       	          /* 卸载某版本的DLL */
}



