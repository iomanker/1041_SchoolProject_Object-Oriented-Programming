#include "Main.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strsafe.h>

//
// This structure keeps some useful information
//
typedef struct _socklist
{
	WSAOVERLAPPED *overlap;
	SOCKET sock;
	SOCKET SockAccepted;
	DWORD Op;
	char Buffer[1024];
	WSABUF DataBuf;
} Socklist;

void Usage(char *progname);
int __cdecl main(int argc, char **argv);
void HandleEvent(int , WSAEVENT *, Socklist *);
int DoWait(WSAEVENT *, Socklist *);
