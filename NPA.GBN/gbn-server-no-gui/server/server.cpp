/*
The following code on the implementation of using UDP to transmit data
refers to the link:
https://msdn.microsoft.com/en-us/library/windows/desktop/ms738545(v=vs.85).aspx
*/

/*
GBN Server: receive message and send ACK, using multiple threadss
*/

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

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define WSVERS MAKEWORD(2, 2)

// Receive datagram and send ACK back
DWORD WINAPI ServerThread(LPVOID lpParam);

int main(int argc, char **argv) {
	WSADATA wsaData;
	SOCKET *server_socks = NULL;
	HANDLE *server_threads = NULL;
	char hoststr[NI_MAXHOST],
		servstr[NI_MAXSERV],
		*port = NULL;
	addrinfo hints,
		*results = NULL,
		*addrptr = NULL;
	int socket_cnt = 0,
		retval;

	// Validate the parameters
	if (argc != 2) {
		printf("Usage: server.exe <port>");
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

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	while (true) {

		retval = getaddrinfo(NULL, port, &hints, &results);
		if (retval != 0) {
			fprintf(stderr, "getaddrinfo failed: %d\n", retval);
			goto cleanup;
		}

		// Make sure we got at least one address back
		if (results == NULL) {
			fprintf(stderr, "Unable to resolve interface %s\n", nullptr);
			goto cleanup;
		}

		// Count how many addresses were returned
		addrptr = results;
		while (addrptr)
		{
			socket_cnt++;
			addrptr = addrptr->ai_next;
		}

		// Allocate space for the server sockets
		server_socks = (SOCKET *)HeapAlloc(
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			sizeof(SOCKET) * socket_cnt
		);
		if (server_socks == NULL) {
			fprintf(stderr, "HeapAlloc failed: %d\n", GetLastError());
			goto cleanup;
		}

		// Initialize the socket array first
		for (int i = 0; i < socket_cnt; i++)
			server_socks[i] = INVALID_SOCKET;

		// Create the server sockets - one for each address returned
		socket_cnt = 0;
		addrptr = results;
		while (addrptr) {
			// Create the socket according to the parameters returned
			server_socks[socket_cnt] = socket(
				addrptr->ai_family,
				addrptr->ai_socktype,
				addrptr->ai_protocol
			);
			if (server_socks[socket_cnt] == INVALID_SOCKET) {
				fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
				goto cleanup;
			}

			// Bind the socket to the address returned
			retval = bind(server_socks[socket_cnt],
				addrptr->ai_addr,
				(int)addrptr->ai_addrlen
			);
			if (retval == SOCKET_ERROR)
			{
				fprintf(stderr, "bind failed: %d\n", WSAGetLastError());
				goto cleanup;
			}

			// Print the address this socket is bound to
			retval = getnameinfo(
				addrptr->ai_addr,
				(socklen_t)addrptr->ai_addrlen,
				hoststr,
				NI_MAXHOST,
				servstr,
				NI_MAXSERV,
				NI_NUMERICHOST | NI_NUMERICSERV
			);
			if (retval != 0)
			{
				fprintf(stderr, "getnameinfo failed: %d\n", retval);
				goto cleanup;
			}

			fprintf(stdout, "socket 0x%x bound to address %s and port %s\n",
				server_socks[socket_cnt], hoststr, servstr);

			socket_cnt++;
			addrptr = addrptr->ai_next;
		}

		// We need a server thread per socket so allocate space for the thread handle
		server_threads = (HANDLE *)HeapAlloc(
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			sizeof(HANDLE) * socket_cnt
		);
		if (server_threads == NULL)
		{
			fprintf(stderr, "HeapAlloc failed: %d\n", GetLastError());
			goto cleanup;
		}

		// Create a thread for each address family which will handle that socket
		for (int i = 0; i < socket_cnt; i++)
		{
			server_threads[i] = CreateThread(
				NULL,
				0,
				ServerThread,
				(LPVOID)server_socks[i],
				0,
				NULL
			);
			if (server_threads[i] == NULL)
			{
				fprintf(stderr, "CreateThread failed: %d\n", GetLastError());
				goto cleanup;
			}
		}
	}

	// Wait until the threads exit, then cleanup
	retval = WaitForMultipleObjects(
		socket_cnt,
		server_threads,
		TRUE,
		INFINITE
	);
	if ((retval == WAIT_FAILED) || (retval == WAIT_TIMEOUT))
	{
		fprintf(stderr, "WaitForMultipleObjects failed: %d\n", GetLastError());
		goto cleanup;
	}

cleanup:

	if (results != NULL)
	{
		freeaddrinfo(results);
		results = NULL;
	}

	// Release socket resources
	if (server_socks != NULL)
	{
		for (int i = 0; i < socket_cnt; i++)
		{
			if (server_socks[i] != INVALID_SOCKET)
				closesocket(server_socks[i]);
			server_socks[i] = INVALID_SOCKET;
		}

		// Free the array
		HeapFree(GetProcessHeap(), 0, server_socks);
		server_socks = NULL;
	}

	// Release thread resources
	if (server_threads != NULL)
	{
		for (int i = 0; i < socket_cnt; i++)
		{
			if (server_threads[i] != NULL)
				CloseHandle(server_threads[i]);
			server_threads[i] = NULL;
		}

		// Free the array
		HeapFree(GetProcessHeap(), 0, server_threads);
		server_threads = NULL;
	}

	return 0;
}


DWORD WINAPI ServerThread(LPVOID lpParam) {
	SOCKET s,
		sc = INVALID_SOCKET;
	SOCKADDR_STORAGE from;
	char sendbuf[DEFAULT_BUFLEN],
		recvbuf[DEFAULT_BUFLEN],
		servstr[NI_MAXSERV],
		hoststr[NI_MAXHOST];
	int fromlen,
		retval,
		bytecnt;
	unsigned ackSeq = -1;
	boost::shared_ptr<std::string> pRecvStr;
	boost::shared_ptr<Datagram>  pRecvDg;
	std::string recvDgSeqStr;

	/* Initialize random number seed */
	boost::mt19937 rng(time(0));
	boost::uniform_01<boost::mt19937&> u01(rng);

	// Retrieve the socket handle
	s = (SOCKET)lpParam;

	while (true) {
		fromlen = sizeof(from);
		// Receive and send data
		bytecnt = recvfrom(s, recvbuf, DEFAULT_BUFLEN, 0, (SOCKADDR*)&from, &fromlen);
		if (bytecnt == SOCKET_ERROR) {
			// We may get WSAECONNRESET errors in response to ICMP port unreachable
			//    messages so we'll just ignore those
			if (WSAGetLastError() != WSAECONNRESET) {
				fprintf(stderr, "recvfrom failed; %d\n", WSAGetLastError());
				goto cleanup;
			}
			else continue;
		}

		// Display the source of the datagram
		retval = getnameinfo(
			(SOCKADDR *)&from,
			fromlen,
			hoststr,
			NI_MAXHOST,
			servstr,
			NI_MAXSERV,
			NI_NUMERICHOST | NI_NUMERICSERV
		);
		if (retval != 0)
		{
			fprintf(stderr, "getnameinfo failed: %d\n", retval);
			goto cleanup;
		}

		// Deserialization the received data
		{
			pRecvStr = boost::make_shared<std::string>(recvbuf);
			pRecvDg = boost::make_shared<Datagram>();
			std::istringstream iss(*pRecvStr);
			boost::archive::binary_iarchive bia(iss);
			bia >> *pRecvDg;
		}

		//show recv info
		recvDgSeqStr = boost::lexical_cast<std::string>(pRecvDg->seq());
		std::cout << "Recv-No. " + recvDgSeqStr
			+ " Receive No." + recvDgSeqStr + " datagram." << std::endl;

		// Randomly send ACK
		if (u01() > 0.2) {
			boost::shared_ptr<std::string> str = boost::make_shared<std::string>(boost::lexical_cast<std::string>(pRecvDg->seq()));
			sprintf_s(sendbuf, sizeof(sendbuf), str->c_str());
			bytecnt = sendto(s, sendbuf, sizeof(str->c_str()), 0, (SOCKADDR *)&from, fromlen);
			if (bytecnt == SOCKET_ERROR) {
				fprintf(stderr, "sendto failed: %d\n", WSAGetLastError());
				goto cleanup;
			}
			//show send info
			std::cout << "Send-No. " + recvDgSeqStr
				+ " Send No." + recvDgSeqStr + " ACK to" + std::string(hoststr) + ":" << std::string(servstr) << std::endl;
			/*
			printf("sent %d bytes to host %s and port %s\n",
				bytecnt, hoststr, servstr);
			*/
		}

		

		/*
		printf("read %d bytes from host %s and port %s\n",
			bytecnt, hoststr, servstr);

		bytecnt = sendto(s, recvbuf, bytecnt, 0, (SOCKADDR *)&from, fromlen);
		if (bytecnt == SOCKET_ERROR) {
			fprintf(stderr, "sendto failed: %d\n", WSAGetLastError());
			goto cleanup;
		}

		printf("sent %d bytes to host %s and port %s\n",
			bytecnt, hoststr, servstr);
		*/
	}

cleanup:
	// The server-running-protocol is UDP, so it doesn't need to closesocket

	return 0;
}