//* OOP lib_com.cpp *
#include "lib_com.h"

void textlink::puts(const char* text)
{
    while(*text) 
    {
        put(*text);
        ++text;
    }
}

void textlink::gets(      char* text, size_t max_size)
{
    while(max_size>1)
    {
        *text=get();
        if ('\r'==*text)
            break;
        max_size--;
    }
    *text='\0';
}

void console::puts(const char* text)
{
    while(*text)
    {
        put(*text);
        ++text;
    }
}

void console::edit(      char* text, size_t max_size) // with edit capabilities and echo
{
    while(max_size>1)
    {
        *text=get();
        if ('\r'==*text) {
            // enter received
            put('\r');
            put('\n');
            break;
        } else if ('\b'==*text||127==*text) {
            // erase received
            --text;
            ++max_size;
            put('\b');
            put(' ');
            put('\b');
        } else {
            // regular character received
            max_size--;
            put(*text);
        }            
    }
    *text='\0';
}

void binarylink::write(const char* buffer, size_t length)
{
    while(length)
    {
        put(*buffer);
        ++buffer;
        --length;
    }    
}

void binarylink::read (      char* buffer, size_t length)
{
    while(length)
    {
        *buffer=get();
        ++buffer;
        --length;
    }
}
