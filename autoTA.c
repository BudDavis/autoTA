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
struct studentInfo 
{
  char ip[100];
  char name[100];
  int idx;
} si[100];

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
	usleep(100000);
        // send text
#if 0
        if ( 0  && kbhit())
        {
            fgets(str, sizeof(str), stdin);
            if (!sendUdpData(remoteIp, REMOTE_PORT, str))
                printf("Could not send message");
            quit = (strncmp(str, "QUIT", 4) == 0);
        }
#endif
        // receive text 
	char *ip =  receiveUdpDataFrom(str, sizeof(str), 50) ;
        if ( ip )
        {
            printf("\n\t\t\t\t\t\t%s", str);
	    int firstOpenSlot = -1;
	    int found = -1;
            for (unsigned int i=0;i<100;i++) 
            {
               if (firstOpenSlot==-1 &&  si[i].idx ==0)
               {
                   // found an open slot
		   firstOpenSlot = i;
               }
	       if (found==-1 && strcmp(ip,si[i].ip)==0)
               {
                  found = i;
               }
            }
	    if (found==-1 && firstOpenSlot!=-1)
            {
	       si[firstOpenSlot].idx = 1;
	       strcpy(si[firstOpenSlot].ip,ip);
	       found = firstOpenSlot;
            }
	    if (found!=-1)
            {
                 switch (si[found].idx)
                 {
                    case 0: printf("error\n");
                            exit(1);
                            break;
                    case 1:
                           // The user must say hello in the begining.
			   if (strstr(str,"ello"))
                           {
                              si[found].idx++;
                              sendUdpData(si[found].ip, REMOTE_PORT, "hello");
                              sendUdpData(si[found].ip, REMOTE_PORT, "what is your name?\n");
                              sendUdpData(si[found].ip, REMOTE_PORT, "(answer politely, with \'my name is\'\n");
			      memset(str,0,sizeof(str));
                           }
                           else
                           {
                              sendUdpData(si[found].ip, REMOTE_PORT, "excuse me ? is that a greeting?\n");
                           }
			   break;
                    case 2:
			   if (strstr(str,"y name is "))
                           {
                              si[found].idx++;
			      char *reply = malloc(200);
			      strcpy(reply,"nice to meet you, ");
			      strcat(reply,strstr(str,"my name is ")+11);
                              sendUdpData(si[found].ip, REMOTE_PORT, reply);
                              sendUdpData(si[found].ip, REMOTE_PORT, "I am known as the autoTA");
                              sendUdpData(si[found].ip, REMOTE_PORT,"Please send me a quit command, lowercase.\n");
			      memset(str,0,sizeof(str));
			      free(reply);
                           }
                           else
                           {
                              sendUdpData(si[found].ip, REMOTE_PORT, "i do not catch your name. please be polite.\n");
                           }
			   break;
                     case 3:
			   if (strstr(str,"quit"))
                           {
                              si[found].idx=1;  // wait for name
                              sendUdpData(si[found].ip, REMOTE_PORT,"got it. Now sending one to you.  Goodbye.\n");
                              sendUdpData(si[found].ip, REMOTE_PORT, "QUIT\n");
                           }
                           else
                           {
                              sendUdpData(si[found].ip, REMOTE_PORT, "waiting for you to send me a QUIT command.\n");
                           }
			   break;
                     case 4:
                           break;
                     case 5:
                           break;
                     case 6:
                           break;
                     case 7:
                           break;
                    default: break;
                 }
            }
        }
     }

    // Close listener port
    closeUdpListenerPort();
    return EXIT_SUCCESS;
}
