/******************************************************************************\
* simplec.c - Simple TCP/UDP client using Winsock 1.1
* 
*       This is a part of the Microsoft Source Code Samples.
*       Copyright 1996 - 2000 Microsoft Corporation.
*       All rights reserved.
*       This source code is only intended as a supplement to
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the
*       Microsoft samples programs.
\******************************************************************************/
#include "simplec.h"

#ifdef _IA64_
	#pragma warning(disable:4127)
#endif 

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strsafe.h>
#include <iostream>

#define DEFAULT_PORT            "5001"          // Default server port
#define DEFAULT_PROTO           SOCK_STREAM     // Default protocol (TCP)
#define DEFAULT_BUFFER_LEN      4096            // Default send/recv buffer length

SOCKET conn_socket = INVALID_SOCKET;
struct addrinfo *results = NULL;

using namespace std;

//
// Function: Usage
// 
// Description:
//    Print the parameters and exit.
//
void Usage(char *progname)
{
	fprintf(stderr, "Usage\n"
		"%s -p [protocol] -n [server] -e [endpoint] -l [iterations] [-4] [-6]\n"
		"Where:\n"
		"\t-p protocol   - is one of TCP or UDP\n"
		"\t-n server     - is the string address or name of server\n"
		"\t-e endpoint   - is the port to listen on\n"
		"\t-4            - force IPv4\n"
		"\t-6            - force IPv6\n"
		"\n"
		"Defaults are TCP, localhost and 5001\n",
		progname
		);
	WSACleanup();
	exit(1);
}

void CleanUp()
{
	//
	// clean up the client connection
	//

	if (conn_socket != INVALID_SOCKET)
	{
		// Indicate no more data to send
		int retval = shutdown(conn_socket, SD_SEND);
		if (retval == SOCKET_ERROR)
		{
			fprintf(stderr, "shutdown failed: %d\n", WSAGetLastError());
		}

		// Close the socket
		retval = closesocket(conn_socket);
		if (retval == SOCKET_ERROR)
		{
			fprintf(stderr, "closesocket failed: %d\n", WSAGetLastError());
		}

		conn_socket = INVALID_SOCKET;
	}

	if (results != NULL)
	{
		freeaddrinfo(results);
		results = NULL;
	}

	WSACleanup();

	exit(0);
}

//
// Function: InitAndConn
//
// Description:
//    Parse the command line and attempt to connect to the given server.
//    The client will attempt to connect to each address returned by the
//    getaddrinfo call until one succeeds afterwhich it will initiate
//    echoing the data. Once the requested number of sends are issued,
//    the connection is closed. For UDP, this is simply connecting the
//    UDP socket to and endpoint and issuing the requested number of
//    datagram sends and receives.
//
int InitAndConn(int argc, char **argv)
{
	WSADATA wsaData;
	struct addrinfo *addrptr = NULL;
	struct addrinfo hints;
	char *server_name = "localhost";
	char *port = DEFAULT_PORT;
	char hoststr[NI_MAXHOST];
	char servstr[NI_MAXSERV];
	int address_family = AF_UNSPEC;
	int socket_type = DEFAULT_PROTO;
	int i;
	
	// Parse the command line
	if (argc >1)
	{
		for (i=1; i < argc; i++)
		{
			if ( (strlen(argv[i]) == 2) && ((argv[i][0] == '-') || (argv[i][0] == '/') ) )
			{
				switch (tolower(argv[i][1]))
				{
				case '4':           // Force IPv4
					address_family = AF_INET;
					break;
				case '6':           // Force IPv6
					address_family = AF_INET6;
					break;
				case 'p':           // Protocol (UDP or TCP)
					if (!_strnicmp(argv[i+1], "TCP", 3) )
						socket_type = SOCK_STREAM;
					else if (!_strnicmp(argv[i+1], "UDP", 3) )
						socket_type = SOCK_DGRAM;
					else
						Usage(argv[0]);
					i++;
					break;
				case 'n':           // Server name to connect to
					server_name = argv[++i];
					break;
				case 'e':           // Port number to connect to
					port = argv[++i];
					break;
				default:
					Usage(argv[0]);
					break;
				}
			}
			else
				Usage(argv[0]);
		}
	}

	// Load Winsock
	int retval;
	if ((retval = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
	{
		fprintf(stderr,"WSAStartup failed with error %d\n",retval);
		WSACleanup();
		return -1;
	}

	// Make sure the wildcard port wasn't specified
	if (_strnicmp(port, "0", 1) == 0)
		Usage(argv[0]);

	//
	// Resolve the server name
	//
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = address_family;
	hints.ai_socktype = socket_type;
	hints.ai_protocol = ((socket_type == SOCK_STREAM) ? IPPROTO_TCP : IPPROTO_UDP);

	retval = getaddrinfo(
		server_name,
		port,
		&hints,
		&results
		);
	if (retval != 0)
	{
		fprintf(stderr, "getaddrinfo failed: %d\n", retval);
		CleanUp();
	}

	// Make sure we got at least one address
	if (results == NULL)
	{
		fprintf(stderr, "Server (%s) name could not be resolved!\n", server_name);
		CleanUp();
	}

	//
	// Walk through the list of addresses returned and connect to each one.
	//    Take the first successful connection.
	//
	addrptr = results;
	while (addrptr)
	{
		conn_socket = socket(addrptr->ai_family, addrptr->ai_socktype, addrptr->ai_protocol);
		if (conn_socket == INVALID_SOCKET)
		{
			fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
			CleanUp();
		}

		//
		// Notice that nothing in this code is specific to whether we 
		// are using UDP or TCP.
		// We achieve this by using a simple trick.
		//    When connect() is called on a datagram socket, it does not 
		//    actually establish the connection as a stream (TCP) socket
		//    would. Instead, TCP/IP establishes the remote half of the
		//    ( LocalIPAddress, LocalPort, RemoteIP, RemotePort) mapping.
		//    This enables us to use send() and recv() on datagram sockets,
		//    instead of recvfrom() and sendto()

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
			CleanUp();
		}

		printf("Client attempting connection to: %s port: %s\n", hoststr, servstr);

		retval = connect(conn_socket, addrptr->ai_addr, (int)addrptr->ai_addrlen);
		if (retval == SOCKET_ERROR)
		{
			closesocket(conn_socket);
			conn_socket = INVALID_SOCKET;

			addrptr = addrptr->ai_next;
		}
		else
		{
			break;
		}
	}

	freeaddrinfo(results);
	results = NULL;

	// Make sure we got a connection established
	if (conn_socket == INVALID_SOCKET)
	{
		printf("Unable to establish connection...\n");
		CleanUp();
	}
	else
	{
		printf("Connection established...\n");
	}
	
	return 0;
}

string SendAndWaitResponse(string str)
{
	//
	// cook up a string to send
	//
	bool showVerbose = false;
	char Buffer[DEFAULT_BUFFER_LEN];
	string str2 = ReplaceAll(str, "%", "%%");

	StringCbPrintf(Buffer, sizeof(Buffer),
		//"This is a small test message [number %d]",
		str2.c_str()
		);

	// Send the data
	int retval = send(conn_socket, Buffer, lstrlen(Buffer)+1, 0);
	if (retval == SOCKET_ERROR)
	{
		if (showVerbose)
			fprintf(stderr,"send failed: error %d\n", WSAGetLastError());
		CleanUp();
	}

	if (showVerbose)
		printf("wrote %d bytes\n", retval);

	// Receive the data back
	retval = recv(conn_socket, Buffer, DEFAULT_BUFFER_LEN, 0);
	if (retval == SOCKET_ERROR)
	{
		if (showVerbose)
			fprintf(stderr, "recv failed: error %d\n", WSAGetLastError());
		CleanUp();
	}
	//
	// We are not likely to see this with UDP, since there is no
	// 'connection' established. 
	//
	if (retval == 0)
	{
		if (showVerbose)
			printf("Server closed connection\n");

		return "";
	}

	if (showVerbose)
		printf("read %d bytes, data [%s] from server\n", retval, Buffer);
	
	return Buffer;
}

string ReplaceAll(string str, const string& from, const string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}
