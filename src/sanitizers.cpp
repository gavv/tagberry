/*
 * Copyright (C) 2020 Tagberry authors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#ifdef __clang__

#define SANITIZER_HOOK                                                                   \
    extern "C" /**/                                                                      \
        __attribute__((no_sanitize("undefined", "address"))) /**/                        \
        __attribute__((visibility("default"))) /**/                                      \
        __attribute__((used))

SANITIZER_HOOK const char* __lsan_default_options();
SANITIZER_HOOK const char* __lsan_default_options()
{
    return "print_suppressions=0";
}

SANITIZER_HOOK const char* __lsan_default_suppressions();
SANITIZER_HOOK const char* __lsan_default_suppressions()
{
    return "leak:libfontconfig\n";
}

#endif // __clang__
