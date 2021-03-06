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
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include "Datagram.h"
#include "Timer.h"
#include "DatagramTimers.h"


#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 2048
#define WSVERS MAKEWORD(2, 2)

boost::thread recvThrd;
boost::mutex timer_mutex;

// Thread receives ACK
void recvAckThrd(SOCKET * s, boost::shared_ptr<DatagramTimers> pTimers) {
	SOCKET sock = *s;
	char recvbuf[DEFAULT_BUFLEN];
	int retval;
	while (true) {
		boost::this_thread::interruption_point();
		//recv ACK
		retval = recv(sock, recvbuf, sizeof(recvbuf), 0);
		if (!(retval >= 0)) continue;
		unsigned rAck = boost::lexical_cast<unsigned>(recvbuf);
		boost::mutex::scoped_lock lock(timer_mutex);
		pTimers->recvAck(rAck);
		//show recv info
		std::cout << "Recv-No." + boost::lexical_cast<std::string>(rAck)
			+ " Receive No." + boost::lexical_cast<std::string>(rAck) + " ACK." << std::endl;
	}
	return;
}

// Close the receive ACK Thread
void closeAckThread() {
	std::cout << "Closing the receive-ACK thread...." << std::endl;

	recvThrd.interrupt();
	recvThrd.join();
	std::cout << "Receive-ACK thread closed." << std::endl;
}
int main(int argc, char **argv) {
	WSADATA wsaData;
	SOCKET sock = INVALID_SOCKET;
	addrinfo *results = NULL,
		*addrptr = NULL,
		hints;
	char *servername = NULL,
		*port = NULL,
		*filename = NULL,
	    hoststr[NI_MAXHOST],
		servstr[NI_MAXSERV];
	NewDatagram sendbuf;

	int retval;

	boost::shared_ptr<std::map<unsigned, NewDatagram>> pDataGrams;
	boost::shared_ptr<DatagramTimers> pTimers;
	std::map<unsigned, NewDatagram>::iterator dgIt;
	std::string msgstr;
	unsigned msgseq = 0;
	unsigned curDgCnt, nextDgCnt;
	unsigned timeout;
	std::string curDgCntStr, nextDgCntStr;
	std::string sendDgCnt;

	// Validate the parameters
	if (argc != 5) {
		printf("Usage: ./client.exe <servername> <port> <timeout_value> <path-message-file>.\n");
		system("pause");
		return 1;
	}
	servername = argv[1];
	port = argv[2];
	timeout = boost::lexical_cast<unsigned>(argv[3]);
	filename = argv[4];

	// Initialize Winsock
	retval = WSAStartup(WSVERS, &wsaData);
	if (retval != 0) {
		fprintf(stderr, "WSAStartup failed with error %d\n", retval);
		WSACleanup();
		system("pause");
		return -1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	retval = getaddrinfo(servername, port, &hints, &results);
	if (retval != 0) {
		fprintf(stderr, "getaddrinfo failed: %d\n", retval);
		WSACleanup();
		system("pause");
		return 1;
	}

	// Make sure we got at least one address
	if (results == NULL) {
		fprintf(stderr, "Server (%s) name could not be resolved!\n", servername);
		WSACleanup();
		system("pause");
		return 1;
	}

	addrptr = results;
	while (addrptr) {
		sock = socket(addrptr->ai_family, addrptr->ai_socktype, addrptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return 1;
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
			closesocket(sock);
			WSACleanup();
			return 1;
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
		closesocket(sock);
		WSACleanup();
		system("pause");
		return 1;
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
	pDataGrams = boost::make_shared<std::map<unsigned, NewDatagram>>();

	// Read messages from file and store them in a list
	// stream is freed when work is finished
	
	{
		std::ifstream infile;
		infile.open(filename);
		if (!infile.is_open()) {
			printf("Can not open the file 'testmsg.txt', please make sure it's in the same directory with this file.\n");
			closesocket(sock);
			WSACleanup();
			system("pause");
			return 1;
		}
	
		for (; getline(infile, msgstr); msgseq++) {
			NewDatagram dg(msgstr.c_str(), msgseq);
			pDataGrams->insert(std::pair<unsigned, NewDatagram>(msgseq, NewDatagram(msgstr.c_str(), msgseq)));
		}
		infile.close();
	}

	//Initial the timers
	pTimers = boost::make_shared<DatagramTimers>(pDataGrams, timeout);

	// Start to send datagrams and receive ACK from server, start corresponding timers at the same time
	dgIt = pDataGrams->begin();
	recvThrd = boost::thread(boost::bind(&recvAckThrd, &sock, pTimers));

	Sleep(1000);
	while (dgIt != pDataGrams->end() || !pTimers->empty()) {
		//record next to-send datagram's number

		boost::unique_lock<boost::mutex> lock1(timer_mutex);
		if (dgIt != pDataGrams->end()) {
			curDgCnt = dgIt->first;
			curDgCntStr = boost::lexical_cast<std::string>(curDgCnt);
		}
		//check whether timeout, if it is , roll back the iterator
		dgIt = pTimers->checkTimeout(dgIt);
		if (dgIt != pDataGrams->end()) {
			//check whether the iterator changed
			nextDgCnt = dgIt->first;
			nextDgCntStr = boost::lexical_cast<std::string>(nextDgCnt);

			if (pTimers->is_timeout()) {
				curDgCntStr = boost::lexical_cast<std::string>(curDgCnt);
				nextDgCntStr = boost::lexical_cast<std::string>(nextDgCnt);
				std::cout << "Tout-No." + pTimers->get_timeout_info()
					+ ": Timeout for No." + pTimers->get_timeout_info() + " datagram." << std::endl;
				std::cout << "Gobn-No." + pTimers->get_timeout_info()
					+ ": Go back to No." + pTimers->get_timeout_info() + " datagram." << std::endl;
				std::cout << std::endl;
				pTimers->clearTimers();
			}

			retval = send(sock, (char*)&(dgIt->second), sizeof(dgIt->second), 0);
			if (retval == SOCKET_ERROR) {
				fprintf(stderr, "send failed: error %d\n", WSAGetLastError());
				break;
			}
			//Show send info

			std::cout << "Send-No." + nextDgCntStr + " Sent No." + nextDgCntStr + " datagram." << std::endl;

			//lock1.unlock();
			//set new timer
			//boost::unique_lock<boost::mutex> lock2(timer_mutex);
			pTimers->addTimer(dgIt->first);
			dgIt++;
		}
		pTimers->updateTimers();
		lock1.unlock();

		Sleep(100);
	}

	closeAckThread();
	WSACleanup();
	system("pause");
	return 0;

}