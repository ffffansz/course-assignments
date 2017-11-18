/*
The following code on the implementation of using UDP to transmit data
refers to the link:
https://msdn.microsoft.com/en-us/library/windows/desktop/ms738545(v=vs.85).aspx
*/

/* 
GBN Client: send message and receive ACK
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string.h>
#include "Segment.h"
#include <fstream>
#include <string>
#include <list>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_SLDWIND_LEN 10   //Sliding window size
#define WSVERS MAKEWORD(2, 2)

using std::ifstream;
using std::string;
using std::list;

/*
// Convert type 'char *' to type 'LPCTSTR'
LPWSTR char2LPCTSTR(char * c) {
	int n = MultiByteToWideChar(0, 0, c, -1, NULL, 0);
	wchar_t * wide = new wchar_t[n];
	MultiByteToWideChar(0, 0, c, -1, wide, n);
	return wide;
}
*/

int __cdecl main(int argc, char **argv) {
	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	addrinfo *results = NULL, 
			 *addrptr = NULL,
			 hints;
	char *servername = NULL,
		*port = NULL,
		sendbuf[DEFAULT_BUFLEN],
		recvbuf[DEFAULT_BUFLEN],
		hoststr[NI_MAXHOST],
		servstr[NI_MAXSERV];

	int retval,
		sldwinlen = DEFAULT_SLDWIND_LEN;

	ifstream infile;
	list<Segment> msglist;
	string msgstr;
	unsigned msgseq = 0;

	// Validate the parameters
	if (argc != 3) {
		printf("Error: Requiring a server-name and a server-port.\n");
		return 1;
	}
	servername = argv[1];
	port = argv[2];

	// Initialize Winsock
	retval = WSAStartup(WSVERS, &wsaData);
	if (retval != 0) {
		fprintf(stderr, "WSAStartup failed with error %d\n", retval);
		WSACleanup();
		return -1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	retval = getaddrinfo(servername, port, &hints, &results);
	if (retval != 0) {
		fprintf(stderr, "getaddrinfo failed: %d\n", retval);
		goto cleanup;
	}

	// Make sure we got at least one address
	if (results == NULL) {
		fprintf(stderr, "Server (%s) name could not be resolved!\n", servername);
		goto cleanup;
	}

	addrptr = results;
	while (addrptr) {
		sock = socket(addrptr->ai_family, addrptr->ai_socktype, addrptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
			goto cleanup;
		}

		retval = getnameinfo(
							addrptr->ai_addr,
							(socklen_t)addrptr->ai_addrlen,
							hoststr,
							NI_MAXHOST,
							servstr,
							NI_MAXSERV,
							NI_NUMERICHOST | NI_NUMERICSERV);
		if (retval != 0) {
			fprintf(stderr, "getnameinfo failed: %d\n", retval);
			goto cleanup;
		}

		// We use connect() function to specify the remote address and port
		// Then can use send() and recv() instead of sendto() and recvfrom()
		retval = connect(sock, addrptr->ai_addr, (int)addrptr->ai_addrlen);
		if (retval == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			addrptr = addrptr->ai_next;
		}
		else break;
	}
	freeaddrinfo(results);
	results = NULL;
	
	if (sock == INVALID_SOCKET) {
		printf("Unable to establish connection...\n");
		goto cleanup;
	}

	/*
	// Send five test msg
	for (int i = 0; i < 5; i++) {
		sprintf_s(sendbuf, sizeof(sendbuf), "This is a test msg with ÖÐÎÄ No. %d", i);
		retval = send(sock, sendbuf, strlen(sendbuf) + 1, 0);
		if (retval == SOCKET_ERROR) {
			fprintf(stderr, "send failed: error %d\n", WSAGetLastError());
			goto cleanup;
		}
		printf("Sent %d bytes\n", retval);
	}
	*/

	// Read messages from file and store them in a list
	infile.open("msgs.txt");
	
	if (!infile.is_open()) {
		printf("Can not open the file 'msgs.txt'.");
		goto cleanup;
	}
	while (getline(infile, msgstr))
		msglist.push_back(Segment(msgstr, msgseq));
	
	infile.close();

	// Send every Msg and receive ACK from server
	while (!msglist.empty()) {

	}

	// Clean up the client connection
	cleanup:

	if (sock != INVALID_SOCKET) {
		// Indicate no more data to send
		retval = shutdown(sock, SD_SEND);
		if (retval == SOCKET_ERROR) {
			fprintf(stderr, "shutdown failed: %d\n", WSAGetLastError());
		}

		// Close the socket
		retval = closesocket(sock);
		if (retval == SOCKET_ERROR) {
			fprintf(stderr, "closesocket failed: %d\n", WSAGetLastError());
		}

		sock = INVALID_SOCKET;
	}

	if (results != NULL) {
		freeaddrinfo(results);
		results = NULL;
	}

	WSACleanup();

	return 0;

}