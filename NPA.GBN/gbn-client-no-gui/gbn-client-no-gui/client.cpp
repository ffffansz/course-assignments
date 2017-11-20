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
#include <fstream>
#include <string>
#include <list>
#include <sstream>
#include <queue>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include "Datagram.h"
#include "Timer.h"
#include "DatagramTimers.h"


#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define WSVERS MAKEWORD(2, 2)

int main(int argc, char **argv) {
	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	addrinfo *results = NULL,
		*addrptr = NULL,
		hints;
	char *servername = NULL,
		*port = NULL,
		*filename = NULL,
	    sendbuf[DEFAULT_BUFLEN],
		recvbuf[DEFAULT_BUFLEN],
	    hoststr[NI_MAXHOST],
		servstr[NI_MAXSERV];

	int retval;

	boost::shared_ptr<std::map<unsigned, char*>> pDataGrams;
	boost::shared_ptr<DatagramTimers> pTimers;
	std::map<unsigned, char*>::iterator dgIt;
	std::string msgstr;
	unsigned msgseq = 0;
	unsigned curDgCnt, nextDgCnt;
	std::string curDgCntStr, nextDgCntStr;

	// Validate the parameters
	if (argc != 4) {
		printf("Usage: client.exe <servername> <port> <path-message-file>.\n");
		return 1;
	}
	servername = argv[1];
	port = argv[2];
	filename = argv[3];

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
	sprintf_s(sendbuf, sizeof(sendbuf), "This is a test msg with ���� No. %d", i);
	retval = send(sock, sendbuf, strlen(sendbuf) + 1, 0);
	if (retval == SOCKET_ERROR) {
	fprintf(stderr, "send failed: error %d\n", WSAGetLastError());
	goto cleanup;
	}
	printf("Sent %d bytes\n", retval);
	}
	*/

	//Initialize the buffer of datagram
	pDataGrams = boost::make_shared<std::map<unsigned, char*>>();

	// Read messages from file and store them in a list
	// Datagram class -> serialize  -> char * -> into list<char*>
	// stream is freed when work is finished
	{
		std::ifstream infile;
		infile.open(filename);
		if (!infile.is_open()) {
			printf("Can not open the file 'testmsg.txt', please make sure it's in the same directory with this file.\n");
			goto cleanup;
		}
		std::ostringstream oss;
		boost::archive::binary_oarchive boa(oss);
		for (; getline(infile, msgstr); msgseq++) {
			Datagram dg(msgstr, msgseq);
			boa << dg;
			char * ptr = new char[oss.str().length() + 1];
			strcpy_s(ptr, oss.str().length(), oss.str().c_str());
			pDataGrams->insert(std::pair<unsigned, char*>(msgseq, ptr));
			oss.clear();
		}
		infile.close();
	}

	//Initial the timers
	pTimers = boost::make_shared<DatagramTimers>(pDataGrams);

	// Start to send datagrams and receive ACK from server, start corresponding timers at the same time
	dgIt = pDataGrams->begin();
	
	while (dgIt != pDataGrams->end()) {
		//record next to-send datagram's number
		curDgCnt = dgIt->first;
		//check whether timeout, if it is , roll back the iterator
		dgIt = pTimers->checkTimeout(dgIt);
		//check whether the iterator changed
		nextDgCnt = dgIt->first;
		//show gbn info
		if (curDgCnt != nextDgCnt) {
			curDgCntStr = boost::lexical_cast<std::string>(curDgCnt);
			nextDgCntStr = boost::lexical_cast<std::string>(nextDgCnt);
			std::cout << "Tout-No." + curDgCntStr
				+ ": Timeout for No." + nextDgCntStr + " datagram." << std::endl;
			std::cout << "Gobn-No." + nextDgCntStr
				+ ": Go back to No." + nextDgCntStr + " datagram." << std::endl;
			std::cout << std::endl;
		}
		//send data
		sprintf_s(sendbuf, sizeof(sendbuf), dgIt->second);
		std::string test(sendbuf); //debug
		//TODO
		retval = send(sock, sendbuf, strlen(sendbuf) + 1, 0);
		if (retval == SOCKET_ERROR) {
			fprintf(stderr, "send failed: error %d\n", WSAGetLastError());
			break;
		}
		//Show send info
		std::cout << "Send-No. " + boost::lexical_cast<std::string>(dgIt->first) 
			+ " Sent No." + boost::lexical_cast<std::string>(dgIt->first) + " datagram." << std::endl;
		//set new timer
		pTimers->addTimer(dgIt->first);
		//recv ACK
		recv(sock, recvbuf, sizeof(recvbuf), 0);
		unsigned rAck = boost::lexical_cast<unsigned>(recvbuf);
		pTimers->recvAck(rAck);
		//show recv info
		std::cout << "Recv-No. " + boost::lexical_cast<std::string>(rAck)
			+ " Receive No." + boost::lexical_cast<std::string>(rAck) + " ACK." << std::endl;
		//update timers
		pTimers->updateTimers();
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
	system("pause");
	return 0;

}