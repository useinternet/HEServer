#pragma once


/* 서버 실행
// 1. 클라 -> 플레이 용도, 하청 용도인지 서버한테 보냄
// if 플레이 용도일 경우
session

    else 하청 용도 일 경우
partition class 등록



CreateIoCompletionPort
leHandle ->                         장치에 대한 핸들 (파일,소켓,메일..등의 핸들)
 
 ExistingCompletionPort ->         I/O Completion Port로 생성된 Handle 
 
 CompletionKey->                    사용자가 임의로 전달할 수 있는 값. 함수는 오로지 값의 전달만 한다.
                                            (구조체 형식으로 통신에 필요한 데이터를 넣을 수 있다.)
 NumberOfConcurrentThread ->  I/O Completion Port에게 동일 시간에 동시에 수행할 수 있는 스레드의
                                            최대 개수를 알려주는 역할을 한다.
                                            (0을 전달하면 머신에 설치된 CPU의 개수를 동시에 수행 가능한 스레드의
                                             최대 개수로 설정한다.)

HANDLE CreateNewCompletionPort(DWORD dwNumberOfConcurrentThreads) {
 
    return CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0,
        dwNumberOfConcurrentThreads);
}
 
//사용
//...
HANDLE hIOCP = CreateNewCompletionPort(0);


// 장치와의 연계
BOOL AssociateDeviceWithCompletionPort(HANDLE hCompletionPort, HANDLE hDevice,
    DWORD dwCompletionKey)
{
    HANDLE h = CreateIoCompletionPort(hDevice, hCompletionPort, dwCompletionKey, 0);
    return (h == hCompletionPort);
}
 
// 사용
//..
// hIOCP는 이미 생성됨. clntSocket은 accept로 받은 소켓. IOCPstruct는 사용자 임의 구조체
AssociateDeviceWithCompletionPort(hIOCP,clntSocket,IOCPstruct);
*/
//
//GetCompletionStatus




namespace he
{
    // 프로세스 단위로 실행 시키는 클래스
    // 여기에 싱글톤 
    class partition
    {
    public:
        // 다른 컴퓨터에 
    };
}

