#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string>

#define BUFLEN 2000
#define WSVERS MAKEWORD(2, 0)
#define MAX_CNT_SIZE 20    //聊天室最多可容纳20人
#pragma comment(lib, "ws2_32.lib")    //winsock lib 2.2 version

struct user {
	SOCKET ssock;
	int id;
};

user users[MAX_CNT_SIZE];

std::string users_name[20] = { "Amy", "Adam", "Andy", "Bob", "Bill",
							   "Cindy", "David", "Denny", "Eason", "Frank",
							   "Gates", "Halo", "Henry", "Ivan", "Jack",
							   "James", "Jim", "Ken", "Kevin", "Lee"};//二十个用户名，会依次分配给加入聊天室的用户

SOCKET sockList[MAX_CNT_SIZE];    //socket数组作为全局变量，方便传递
HANDLE hRecvAndSendThread[MAX_CNT_SIZE];        //负责接受和发送的子线程的句柄


int cnt_count = 0;    //计算当前连接总数

unsigned __stdcall recvAndSendMsg(LPVOID para) {
	user local = *((user *)para);
	char recvBuf[BUFLEN + 1];    //接受缓冲区，接收用户发来的数据
	char sendBuf[BUFLEN + 1];    //发送缓冲区，对接受到的信息做一定的格式处理，再发送给所有用户
	recvBuf[0] = 0;
	sendBuf[0] = 0;
	
	while (true) {
		/* clear buffer */
		recvBuf[0] = '\0';
		sendBuf[0] = '\0';

		int recvRst = recv(local.ssock, recvBuf, BUFLEN, 0);
		if (recvRst == 0 || recvRst == SOCKET_ERROR) {
			printf("Users %s exit, whose ID: %d.\n", users_name[local.id].c_str(), local.id);
			(void)closesocket(local.ssock);
			break;
		}
		else {
			recvBuf[recvRst] = '\0';
			strcat(sendBuf, users_name[local.id].c_str());
			strcat(sendBuf, ": ");
			strcat(sendBuf, recvBuf);
			/* send to all */
			for (int i = 0; i < MAX_CNT_SIZE; i++) {
				if (users[i].ssock != NULL && i != local.id) {
					send(users[i].ssock, sendBuf, strlen(sendBuf), 0);
				}
			}
		}
	}

	return 0;
}

void main(int argc, char * argv[]) {
	struct sockaddr_in fsin;
	SOCKET msock;
	WSADATA wsadata;
	char * service = "50520";
	struct sockaddr_in sin;
	int addrLen;
	//对于每个进程有一个缓冲区

	WSAStartup(WSVERS, &wsadata);
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons((u_short)atoi(service));
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));

	listen(msock, MAX_CNT_SIZE);    //监听连接请求
	printf("Server is ready.\n");

	while (cnt_count < MAX_CNT_SIZE) {
		/* 主线程负责accept，每accept一个连接请求，count+1，对应的sockList[i]为该连接的socket，建立子线程，负责recv and send to all. */
		SOCKET tmpSock = accept(msock, (struct sockaddr *)&fsin, &addrLen);
		if (tmpSock == INVALID_SOCKET)
			printf("Accept Error: %d.\n", GetLastError());
		else {
			users[cnt_count].ssock = tmpSock;
			users[cnt_count].id = cnt_count;
			hRecvAndSendThread[cnt_count] = (HANDLE)_beginthreadex(NULL, 0, &recvAndSendMsg, (void *)&users[cnt_count], 0, NULL);
			cnt_count++;
		}
	}
	(void)closesocket(msock);
	WSACleanup();
}

