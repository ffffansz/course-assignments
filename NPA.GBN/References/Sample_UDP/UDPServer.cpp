/* UDPServer.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include "conio.h"
#include <time.h>


#define	RECVBUFLEN		2000              // 接收缓冲区大小
#define SENDBUFLEN      1500              // 发送缓冲区大小
#define WSVERS		MAKEWORD(2, 2)        // 指明版本2.2 
#pragma comment(lib,"ws2_32.lib")         // 加载winsock 2.2 Llibrary

/*------------------------------------------------------------------------
 * main - TCP client for Echo service
 *------------------------------------------------------------------------
 */
void
main(int argc, char *argv[])
{
	char   *host = "127.0.0.1";	    /* server IP Address to connect */
	char   *service = "50510";  	/* server port to connect       */
	struct sockaddr_in sin;	        /* an Internet endpoint address	*/
	struct sockaddr_in from;        /* sender address               */
	int    fromsize = sizeof(from);
	char   recvBuf[RECVBUFLEN +1];   	    /* buffer for one line of text	*/
	char sendBuf[SENDBUFLEN + 1];
	sendBuf[0] = 0;
	SOCKET	sock;		  	        /* socket descriptor	    	*/
	//memset(sendBuf, 0, 1500);
	time_t now;

	WSADATA wsadata;
    WSAStartup(WSVERS, &wsadata);   /* 加载winsock library，WSVERS为请求版本,wsadata返回系统实际支持的最高版本。    */		
    sock = socket(PF_INET, SOCK_DGRAM,IPPROTO_UDP); // 创建UDP套接字, 参数：因特网协议簇(family)，数据报套接字，UDP协议号， 返回：要监听套接字的描述符或INVALID_SOCKET
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;                     // 绑定(监听)所有的接口。
	sin.sin_port = htons((u_short)atoi(service));         // 绑定指定接口。atoi--把ascii转化为int，htons -- 主机序(host)转化为网络序(network), 为short类型。 
	bind(sock, (struct sockaddr *)&sin, sizeof(sin));     // 绑定本地端口号（和本地IP地址)

 	while(!_kbhit()){                                    //检测是否有按键
	    int recvResult = recvfrom(sock, recvBuf, RECVBUFLEN, 0, (SOCKADDR *)&from, &fromsize);  //接收客户数据。返回结果：recvResult为接收的字符数，from中将包含客户IP地址和端口号。
        if (recvResult == SOCKET_ERROR){
            printf("recvfrom() failed; %d\n", WSAGetLastError());
            break;
        }
        else if (recvResult == 0)
            break;
		else {
			recvBuf[recvResult] = '\0';
			printf("Message from Client:\n%s\n", recvBuf);
			char message_ip[100] = "The Client IP Address: ";
			strcat(message_ip, inet_ntoa(from.sin_addr));
			char message_port[50] = "The Client Port: ";
			char tmp[10];
			itoa(ntohs(from.sin_port), tmp, 10);
			strcat(message_port, tmp);
			(void)time(&now);
			sendBuf[0] = 0;
			strcat(sendBuf, ctime(&now)); 
			strcat(sendBuf, message_ip);
			strcat(sendBuf, "\n");
			strcat(sendBuf, message_port);
			strcat(sendBuf, "\n");
			strcat(sendBuf, recvBuf);

			int sendResult = sendto(sock, sendBuf, strlen(sendBuf), 0, (SOCKADDR *)&from, fromsize);
			if (sendResult == 0 || sendResult == SOCKET_ERROR) {
				printf("Error: %d.\n", WSAGetLastError());
				printf("Sorry, echo was sent unsuccessfully.\n");
			}
			else if (sendResult > 0) {
				printf("The following message has been sent:\n%s\n", sendBuf);
			}
		}
		//closesocket(sock);
	}

	GlobalFree(recvBuf);
	WSACleanup();       	          /* 卸载某版本的DLL */
}



