#pragma once

#include <stddef.h>
#include <string.h>

struct String
{
    const char *ptr;
    size_t len;

    String(const char *c_str)
    {
        this->ptr = c_str;
        this->len = strlen(c_str);
    }

    inline size_t length()
    {
        return this->len;
    }

    inline char operator[](size_t index) const
    {
        return ptr[index];
    }

    inline const char *c_str() const
    {
        return this->ptr;
    }

    inline const char *begin() const
    {
        return this->ptr;
    }

    inline const char *end() const
    {
        return this->ptr + this->len;
    }
};
