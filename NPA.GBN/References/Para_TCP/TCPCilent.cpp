#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <process.h>

#define SENDBUF 2000
#define RECVBUF 2000
#define WSVERS MAKEWORD(2, 0)
#pragma comment(lib, "ws2_32.lib")

char recvBuf[RECVBUF + 1]; //接收缓冲区，设置为全局变量，方便recv子线程修改

unsigned __stdcall recvMsg(LPVOID para)
{
	SOCKET sock = *((SOCKET *)para);
	
	while (true) {
		recvBuf[0] = '\0';
		int recvRst = recv(sock, recvBuf, RECVBUF, 0);
		if (recvRst == SOCKET_ERROR) {
			printf("Sorry, the server is not avalible, please check the server or restart the client.\n");
			break;
		}

		else {
			recvBuf[recvRst] = '\0';
			printf("%s", recvBuf);
		}
	}
	
	return 0;
}


void main(int argc, char * argv[]) {
	char * host = "127.0.0.1";
	char * service = "50520";
	struct sockaddr_in sin;
	char sendBuf[SENDBUF + 1];    /* send buffer */
	SOCKET sock;
	HANDLE hRecvThread;    //用于接收信息的子线程
	int cntRst = -1;	//用于判断connect请求是否成功
	//int p = 1;    //_beginthreadex的第四个参数 - 意义不明
	

	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	//创建套接字

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(host);
	sin.sin_port = htons((u_short)atoi(service));
	//printf("flag1\n");    //debug
	printf("Connecting...\n");
	do {
		/* 不断向服务器发送连接请求，直到连接到服务器 */
		/* 目的是实现连接请求的阻塞 */
		cntRst = connect(sock, (struct sockaddr *)&sin, sizeof(sin));	//成功连接则返回0，否则返回SOCKET_ERROR
	} while (cntRst != 0);

	//printf("flag2\n");    //debug
	printf("Connected.\nYou enter a chat room, now enjoy it!\n");
	printf("If you want to exit, please press ctrl + z, and then press any key to exit.\n");

	/* 建立子线程，用于接收消息 */
	hRecvThread = (HANDLE)_beginthreadex(NULL, 0, &recvMsg, (void *)&sock, 0, NULL);

	/* 发送消息并显示反馈 */
	while (fgets(sendBuf, SENDBUF, stdin)) {    //使用fgets()可读取一行输入
		int sendRst = send(sock, sendBuf, strlen(sendBuf), 0);
		/*
		if (sendRst == 0) {
			printf("Input is empty, please type something and retry.\n");
		}
		*/
		if (sendRst == SOCKET_ERROR) {
			printf("Send Error: %d\n", GetLastError());
			printf("Sorry, message was sent unsuccessfully, please be sure whether server is avalible.\n");
		}
		/*
		else {
			printf("Message has been sent successfully.\n");
		}
		*/
	}

	/* 关闭线程句柄，释放线程资源 */
	CloseHandle(hRecvThread);

	/* 关闭监听套接字*/
	closesocket(sock);
	WSACleanup(); //卸载winsock lib
	printf("Press any key to exit...");
	getchar();
}

