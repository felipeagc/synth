#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#define ASDF_ALLOC(size) (::malloc(size))
#define ASDF_REALLOC(ptr, size) (::realloc(ptr, size))
#define ASDF_FREE(ptr) (::free(ptr))
