#pragma once
#include <winsock2.h>

namespace he
{
    // 서버 시작
    // 서버 소켓, ICOP 생성
    // ICOP 수신 (메인 스레드에서 동작), 0.1666 틱마다, 모아놧던 Message 스레드에 풀고, 완료된 메세지 Send

    /*  accept 오면, 일단 너, 하청인지, 클라인지 묻는다.
     *  if 클라일 경우
     *  message 수집
     *
     * else 하청일 경우
     * TO DO
     */
    class socket
    {
    private:
        SOCKET _socket;
    
    public:
        static std::unique_ptr<socket> create();

        bool bind();
        bool listen();
        std::unique_ptr<socket> accept(const socket& in_socket);
        bool recive(uint8_t* out_buffer, uint64_t out_buffer_size);
        bool send(uint8_t* in_buffer, uint64_t in_buffer_size);
    };
}

