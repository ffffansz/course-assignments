/* UDPServer.cpp */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include "conio.h"
#include <time.h>


#define	RECVBUFLEN		2000              // ���ջ�������С
#define SENDBUFLEN      1500              // ���ͻ�������С
#define WSVERS		MAKEWORD(2, 2)        // ָ���汾2.2 
#pragma comment(lib,"ws2_32.lib")         // ����winsock 2.2 Llibrary

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
    WSAStartup(WSVERS, &wsadata);   /* ����winsock library��WSVERSΪ����汾,wsadata����ϵͳʵ��֧�ֵ���߰汾��    */		
    sock = socket(PF_INET, SOCK_DGRAM,IPPROTO_UDP); // ����UDP�׽���, ������������Э���(family)�����ݱ��׽��֣�UDPЭ��ţ� ���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;                     // ��(����)���еĽӿڡ�
	sin.sin_port = htons((u_short)atoi(service));         // ��ָ���ӿڡ�atoi--��asciiת��Ϊint��htons -- ������(host)ת��Ϊ������(network), Ϊshort���͡� 
	bind(sock, (struct sockaddr *)&sin, sizeof(sin));     // �󶨱��ض˿ںţ��ͱ���IP��ַ)

 	while(!_kbhit()){                                    //����Ƿ��а���
	    int recvResult = recvfrom(sock, recvBuf, RECVBUFLEN, 0, (SOCKADDR *)&from, &fromsize);  //���տͻ����ݡ����ؽ����recvResultΪ���յ��ַ�����from�н������ͻ�IP��ַ�Ͷ˿ںš�
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
	WSACleanup();       	          /* ж��ĳ�汾��DLL */
}



