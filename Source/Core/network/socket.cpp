#include "PCH/PCH.h"
#include "socket.h"


namespace he
{
    std::unique_ptr<socket> socket::create()
    {
        return nullptr;
    }

    bool socket::bind()
    {
        return false;
    }
    
    bool socket::listen()
    {
        return false;
    }
    
    std::unique_ptr<socket> socket::accept(const socket& in_socket)
    {
        return nullptr;
    }
    
    bool socket::recive(uint8_t* out_buffer, uint64_t out_buffer_size)
    {
        return false;
    }
    
    bool socket::send(uint8_t* in_buffer, uint64_t in_buffer_size)
    {
        return false;
    }
}

