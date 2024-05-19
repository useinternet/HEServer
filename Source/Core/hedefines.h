#pragma once
#include <assert.h>

namespace he
{
    using thread_id = uint64_t;
}



#define HE_LOG(Category, LogLevel, Message, ...)
#define HE_ASSERT(X) assert(X);
#define HE_CHECK(expression) assert(expression);
enum
{
    INDEX_NONE = -1
};
