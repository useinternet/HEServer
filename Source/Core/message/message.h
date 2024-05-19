#pragma once
#include "hedefines.h"

namespace he
{
    // messagequeue는 messagebus 참조 순서
    
    // 클라 하나당 messagebus 하나
    // messagebus에서 메모리 할당
    // 여기에 한번에 보냄
    // message_data 크기
    enum class EClient
    {
        Unknown,
        Client,
        Worker
    };
    
    class message
    {
        EClient _client_type;
        
    public:
        message();
        virtual ~message() = default;

        
    };
}

