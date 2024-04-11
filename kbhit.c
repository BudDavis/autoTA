// Kbhit Library
// Computer Engineering IoT/Networks Course
// Jason Losh

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdio.h>           // stdin
#include <stdbool.h>         // bool
#include <unistd.h>

#include <sys/time.h>        // timeval struct
#include <sys/types.h>       // fd_set
#include <sys/time.h>        // timeval struct
#include <sys/select.h>

//#include <fcntl.h>           // file number for stdin
#include "kbhit.h"

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

// returns true if keyboard has been hit and the key is in input buffer
bool kbhit(void)
{
    bool success;
    // Look for data
    
    fd_set fd;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;    // = {tv_sec:0, tv_usec:0};
    FD_ZERO(&fd);
    FD_SET(0, &fd);
    select(1, &fd, NULL, NULL, &tv);
    success = FD_ISSET(0, &fd);
    return success;
}


