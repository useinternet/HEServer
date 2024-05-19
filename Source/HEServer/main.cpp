#include "PCH/PCH.h"
#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS // winsock c4996 처리

#include <iostream>
#include <winsock2.h>
#include <thread>
using namespace std;
#define MAXLINE 1024
#pragma comment(lib,"ws2_32.lib") // ws2_32.lib 라이브러리를 링크

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[MAXLINE] = {};
	int32_t recvBytes = 0;
};

enum IO_TYPE
{
	READ,
	WRITE,
	ACCEPT,
	CONNECT
};

struct OverlappedEx
{
	WSAOVERLAPPED overlapped = {};
	int32_t type = 0; // read, write, accept, connect ...
};

void WorkerThreadMain(HANDLE iocpHandle)
{
	while (true)
	{
		DWORD bytesTransferred = 0;
		Session* session = nullptr;
		OverlappedEx* overlappedEx = nullptr;

		BOOL ret = ::GetQueuedCompletionStatus(iocpHandle, &bytesTransferred,
			(ULONG_PTR*)&session, (LPOVERLAPPED*)&overlappedEx, INFINITE);

		if (ret == FALSE || bytesTransferred == 0)
		{
			// TODO : 연결 끊김
			continue;
		}

		struct  sockaddr_in cli;
		int     cli_len = sizeof(sockaddr_in);
		getpeername(session->socket, (LPSOCKADDR)&cli, &cli_len);

		std::string cliip = inet_ntoa(cli.sin_addr);
		
		switch(overlappedEx->type)
		{
			case IO_TYPE::READ:
				{
					

					WSABUF wsaBuf;
					wsaBuf.buf = session->recvBuffer;
					wsaBuf.len = 10;

					DWORD recvLen = 10;
					DWORD flags = 0;

					std::to_string(session->socket);
					::WSARecv(session->socket, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);
					cout << cliip + " : Recv Data IOCP = " << bytesTransferred  + " "<< wsaBuf.buf  << endl;
					
					/*				std::string sss = wsaBuf.buf;
									std::string sed = "compelete recv data in server";
									wsaBuf.buf = (char*)sed.c_str();*/
			
					// ::WSASend(session->socket, &wsaBuf, 1, &recvLen, flags, &overlappedEx->overlapped, NULL);
					//
					// cout << "Send Data IOCP = " << wsaBuf.buf << endl;
				}
			break;

		case IO_TYPE::WRITE:
			{
				cout << "Send Data IOCP = " << bytesTransferred << endl;

				WSABUF wsaBuf;
				wsaBuf.buf = session->recvBuffer;
				wsaBuf.len = MAXLINE;

				DWORD recvLen = 0;
				DWORD flags = 0;
			
				::WSASend(session->socket, &wsaBuf, 1, &recvLen, flags, &overlappedEx->overlapped, NULL);
			}
			break;

		case IO_TYPE::ACCEPT:
			cout << "ACCEPT Data IOCP = " << bytesTransferred << endl;
			break;

		case IO_TYPE::CONNECT:
			cout << "CONNECT Data IOCP = " << bytesTransferred << endl;
			break;
		}
        
		
	}
}

int main()
{
	WSADATA wsadata;
	SOCKET  serv_sd,
		cli_sd;
	struct  sockaddr_in  serv, cli;
	int     cli_len, n;
	char    str[MAXLINE];

	int nResult = WSAStartup(0x101, (LPWSADATA)&wsadata);
	if (nResult != 0) {
		printf("WSA Initialization failed: %d\n", nResult);
		WSACleanup();
		return 1;
	}

	serv_sd = socket(AF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_port = htons(8000);
	serv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int result = bind(serv_sd, (LPSOCKADDR)&serv, sizeof(serv));
	if (result < 0) {
		printf("get hp error, code: %d\n", WSAGetLastError());
	}

	printf("What is local host's name?\n");
	result = gethostname((LPSTR)str, MAXLINE);
	if (result != 0) {
		printf("get hp error, code: %d\n\n", WSAGetLastError());
	}
	else {
		printf("local host name: %s\n", str);

	}
	
	printf("What is %s's IP?\n", str);
	auto hp = gethostbyname(str);
	if (hp == NULL) {
		printf("get hp error, code: %d\n", WSAGetLastError());
	}
	else {
		printf("host name: %s\n", hp->h_name);
		printf("host nickname: %s\n", hp->h_aliases[0]);
		printf("host IP: %s\n", inet_ntoa(*(LPIN_ADDR)(hp->h_addr)));
	}

	if (listen(serv_sd, 5) < 0) {
		fprintf(stderr, "\necho_srv: listen() error!!!\n");
		exit(1);
	}

	cli_len = sizeof(cli);

	HANDLE iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	
	std::thread test = std::thread([&]() {WorkerThreadMain(iocpHandle); });

	while (1) {
		printf("\nserver: waiting for client...\n");

		cli_sd = accept(serv_sd, (LPSOCKADDR)&cli, &cli_len);

		getpeername(cli_sd, (LPSOCKADDR)&cli, &cli_len);
		printf("client's IP: %s\n", inet_ntoa(cli.sin_addr));
		printf("client's port: %d\n\n", ntohs(cli.sin_port));


		Session* session = new Session();
		session->socket = cli_sd;
		
		// 소켓을 CP에 등록
		::CreateIoCompletionPort((HANDLE)cli_sd, iocpHandle, /*Key*/(ULONG_PTR)session, 0);

		WSABUF wsaBuf;
		wsaBuf.buf = session->recvBuffer;
		wsaBuf.len = MAXLINE;

		OverlappedEx* overlappedEx = new OverlappedEx();
		overlappedEx->type = IO_TYPE::READ;

		DWORD recvLen = 0;
		DWORD flags = 0;

		::WSARecv(cli_sd, &wsaBuf, 1, &recvLen, &flags, &overlappedEx->overlapped, NULL);

		
		// n = recv(cli_sd, str, MAXLINE, 0);
		// str[n] = '\0';
		// // 顯示從 client 傳來的字串
		// printf("server: client->server: %s\n", str);
		// ///////////////////////////////////////////////TO DO >>>
		// if (strcmp(str, "How are you?\0") == 0)
		// 	strcpy(str, "Fine, thank you!");
		// else strcpy(str, "What?");
		// ///////////////////////////////////////////////<<< TO DO
		// send(cli_sd, str, strlen(str), 0);
		// // 顯示送去client 的字串
		// printf("server: server->client: %s\n", str);
	}
	test.join();

	closesocket(serv_sd);
	closesocket(cli_sd);
	WSACleanup();
	return 0;
}