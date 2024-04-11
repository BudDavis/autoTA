// Chat Network Application (not for distribution to students)
// Jason Losh

//-----------------------------------------------------------------------------
// Compile notes for C99 code
//-----------------------------------------------------------------------------

// gcc -std=c99 -o chat chat.c udp3.c kbhit.c

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdlib.h>          // EXIT_ codes
#include <stdbool.h>         // bool
#include <stdio.h>           // printf, scanf
#include <string.h>          // strlen, strcmp

#include "kbhit.h"           // kbhit
#include "udp3.h"            // udp3 library

#define LOCAL_PORT  5000     // port from which this program receives data
#define REMOTE_PORT 5000     // port to which this program sends data

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    char* remoteIp;
    char str[100];
    bool quit = false;

    // Verify arguments are good
    bool goodArguments = (argc == 2);
    if (goodArguments)
    {
        remoteIp = argv[1];
    }
    else
    {
        printf("usage: chat IPV4_ADDRESS\n");
        printf("  where:\n");
        printf("  IPV4_ADDRESS is address of the remote machine\n");
        exit(EXIT_FAILURE);
    }

    // Open listener port
    openUdpListenerPort(LOCAL_PORT);

    // Display warning to not use spaces
    printf("Sent\t\t\t\t\t\tReceived\n");
    
    // Start chat loop until someone types a string starting with QUIT
    while(!quit)
    {
        // send text
        if (kbhit())
        {
            fgets(str, sizeof(str), stdin);
            if (!sendUdpData(remoteIp, REMOTE_PORT, str))
                printf("Could not send message");
            quit = (strncmp(str, "QUIT", 4) == 0);
        }

        // receive text
        if (receiveUdpData(str, sizeof(str), 1))
        {
            printf("\n\t\t\t\t\t\t%s", str);
            quit = (strncmp(str, "QUIT", 4) == 0);
        }

    }

    // Close listener port
    closeUdpListenerPort();
    return EXIT_SUCCESS;
}
