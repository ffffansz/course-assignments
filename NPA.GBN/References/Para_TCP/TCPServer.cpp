#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string>

#define BUFLEN 2000
#define WSVERS MAKEWORD(2, 0)
#define MAX_CNT_SIZE 20    //��������������20��
#pragma comment(lib, "ws2_32.lib")    //winsock lib 2.2 version

struct user {
	SOCKET ssock;
	int id;
};

user users[MAX_CNT_SIZE];

std::string users_name[20] = { "Amy", "Adam", "Andy", "Bob", "Bill",
							   "Cindy", "David", "Denny", "Eason", "Frank",
							   "Gates", "Halo", "Henry", "Ivan", "Jack",
							   "James", "Jim", "Ken", "Kevin", "Lee"};//��ʮ���û����������η�������������ҵ��û�

SOCKET sockList[MAX_CNT_SIZE];    //socket������Ϊȫ�ֱ��������㴫��
HANDLE hRecvAndSendThread[MAX_CNT_SIZE];        //������ܺͷ��͵����̵߳ľ��


int cnt_count = 0;    //���㵱ǰ��������

unsigned __stdcall recvAndSendMsg(LPVOID para) {
	user local = *((user *)para);
	char recvBuf[BUFLEN + 1];    //���ܻ������������û�����������
	char sendBuf[BUFLEN + 1];    //���ͻ��������Խ��ܵ�����Ϣ��һ���ĸ�ʽ�����ٷ��͸������û�
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
	//����ÿ��������һ��������

	WSAStartup(WSVERS, &wsadata);
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons((u_short)atoi(service));
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));

	listen(msock, MAX_CNT_SIZE);    //������������
	printf("Server is ready.\n");

	while (cnt_count < MAX_CNT_SIZE) {
		/* ���̸߳���accept��ÿacceptһ����������count+1����Ӧ��sockList[i]Ϊ�����ӵ�socket���������̣߳�����recv and send to all. */
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

