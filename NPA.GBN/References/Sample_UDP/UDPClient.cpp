/* UDPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>

#define	BUFLEN		2000                  // 缓冲区大小
#define WSVERS		MAKEWORD(2, 2)        // 指明版本2.2
#define MAX_INPUT_SIZE 1500
#pragma comment(lib,"ws2_32.lib")         // 加载winsock 2.2 Llibrary

void
main(int argc, char *argv[])
{
	char	*host = "172.18.187.11";	/* server IP to connect         */
	char	*service = "50510";  	    /* server port to connect       */
	struct sockaddr_in toAddr;	        /* an Internet endpoint address	*/
	//struct sockaddr_in fromAddr;
	//int fromSize = sizeof(fromAddr);
	char	recvBuf[BUFLEN+1];   		/* buffer for one line of text	*/
	SOCKET	sock;		  	            /* socket descriptor	    	*/
	time_t	now;			            /*     current time			    */
	char sendBuf[MAX_INPUT_SIZE];

	WSADATA wsadata;
    WSAStartup(WSVERS, &wsadata);       /* 启动某版本Socket的DLL        */

	printf("Now you can input message.\nAfter the first message is input, if you want to exit, please press Ctrl + Z.\n");
	while (scanf("%s", &sendBuf)) {
		sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

		memset(&toAddr, 0, sizeof(toAddr));
		toAddr.sin_family = AF_INET;
		toAddr.sin_port = htons((u_short)atoi(service));    //atoi：把ascii转化为int. htons：主机序(host)转化为网络序(network), s--short
		toAddr.sin_addr.s_addr = inet_addr(host);           //如果host为域名，需要先用函数gethostbyname把域名转化为IP地址
		//bind(sock, (struct sockaddr *)&toAddr, sizeof(sin));     // 绑定本地端口号（和本地IP地址)

		int sendResult = sendto(sock, sendBuf, strlen(sendBuf), 0, (SOCKADDR *)&toAddr, sizeof(toAddr));
		//send = sendto(sock, buf, 1000, 0, (SOCKADDR *)&toAddr, sizeof(toAddr));
		if (sendResult == SOCKET_ERROR) {
			printf("Error：%d.\n", WSAGetLastError());
		}
		else {
			printf("Message has been sent successfully.\n");
			int sizeOfToAddr = sizeof(toAddr);
			int recvResult = recvfrom(sock, recvBuf, BUFLEN, 0, (SOCKADDR *)&toAddr, &sizeOfToAddr);
			if (recvResult == SOCKET_ERROR) {
				printf("Reception failed.\nError: %d.\n", WSAGetLastError());
				break;
			}
			else  if (recvResult == 0) {
				break;
			}
			else {
				recvBuf[recvResult] = '\0';
				printf("%s\n", recvBuf);
			}
		}
		closesocket(sock);
	}
    

	GlobalFree(recvBuf);
	GlobalFree(sendBuf);
	WSACleanup();       	          /* 卸载某版本的DLL */  

	getchar();

}
