#pragma once

#include "base.hpp"

template <typename T>
struct Array
{
private:
    T *ptr = nullptr;
    size_t len = 0;
    size_t cap = 0;

    inline void grow(size_t wanted_cap)
    {
        if (wanted_cap >= this->cap)
        {
            size_t new_cap = this->cap * 2;
            if (new_cap < wanted_cap) new_cap = wanted_cap;
            this->cap = new_cap;
            this->ptr = (T *)ASDF_REALLOC(this->ptr, this->cap * sizeof(T));
        }
    }

public:
    void destroy()
    {
        this->len = 0;
        this->cap = 0;
        ASDF_FREE(this->ptr);
    }

    void push_back(const T &t)
    {
        this->grow(this->len + 1);
        this->ptr[this->len++] = t;
    }

    void pop_back()
    {
        this->ptr[--this->len];
    }

    inline size_t size()
    {
        return this->len;
    }

    inline T &operator[](size_t index)
    {
        return ptr[index];
    }

    inline const T &operator[](size_t index) const
    {
        return ptr[index];
    }

    inline T* begin() {
        return this->ptr;
    }

    inline T* end() {
        return this->ptr + this->len;
    }
};

template <typename T>
struct ArrayView
{
    T *ptr;
    size_t len;

    ArrayView(T *ptr, size_t len)
    {
        this->ptr = ptr;
        this->len = len;
    }

    ArrayView(const Array<T> &arr)
    {
        this->ptr = &arr[0];
        this->len = arr.size();
    }

    inline size_t size()
    {
        return this->len;
    }

    inline T &operator[](size_t index)
    {
        return ptr[index];
    }

    inline const T &operator[](size_t index) const
    {
        return ptr[index];
    }

    inline T* begin() {
        return this->ptr;
    }

    inline T* end() {
        return this->ptr + this->len;
    }
};
