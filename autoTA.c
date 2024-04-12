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
#include <unistd.h>          // usleep
#include <assert.h>          // assert

#include "kbhit.h"           // kbhit
#include "udp3.h"            // udp3 library

#define LOCAL_PORT  5000     // port from which this program receives data
#define REMOTE_PORT 5000     // port to which this program sends data

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------
#define MAXSI  50

struct studentInfo 
{
    char ip[100];
    char name[100];
    int step;  // the step in each conversation
} si[MAXSI];  // si is "student info"

void dumpClientInfo()
{
    int numActive = 0;
    for (unsigned int i=0;i<MAXSI;i++)
    {
       if (si[i].step)
       {
           numActive++;
           printf("%i > ip %s name %s step %d\n",i,si[i].ip,si[i].name,si[i].step);
       }
    }
    printf("Data for %d active clients\n",numActive);
}

int activeConnections()
{
  int numActive = 0;
  for (unsigned int i=0;i<MAXSI;i++)
  {
     if (si[i].step)
     {
        numActive++;
     }
  }
  return numActive;
}

int findSlot(char *ip)
{
    // returns the slot number
    // or the slot number if it is found
    // the first free slot is allocated
    int slot = -1;  // hope this causes an error

    for (unsigned int i=0;i<MAXSI;i++)
    {
        if (strcmp(ip,si[i].ip)==0)
        {
           assert(slot!=-1);
           slot = i;
printf("new ip is %s  slot is %d \n",ip,slot);
           break;
        }
    }

    if (slot==-1) // it was not found
    {
        for (unsigned int i=0;i<MAXSI;i++)
        {
printf("in the loop i = %d slot = %d\n",i,slot);
            if (!si[i].step)
            {
printf("i is %d slot is being set %d\n",i,slot);
                assert(slot==-1);
                slot = i;
                si[i].step = 1;
                strcpy(si[i].ip,ip);
                break;
            }
        }
    }

    assert (slot!=-1 && slot<MAXSI);
    return slot;
}

void talkingScript()
{
}

int main(int argc, char* argv[])
{
    char str[100] = { '\0' };
    bool quit = false;

    // Open listener port
    openUdpListenerPort(LOCAL_PORT);

    // Display warning to not use spaces
    printf("Sent\t\t\t\t\t\tReceived\n");
    
    // Start chat loop 
    while(!quit)
    {
        usleep(100000); // 1/10 of a second. give the machine a break
        // send text
#if 0
        if (kbhit())
        {
            fgets(str, sizeof(str), stdin);
            if (!sendUdpData(remoteIp, REMOTE_PORT, str))
                printf("Could not send message");
            quit = (strncmp(str, "QUIT", 4) == 0);
        }
#endif
        // receive text 
        char *ip =  receiveUdpDataFrom(str, sizeof(str), 100) ;
        if ( ip )
        {
            printf("\n\t\t\t %s \t\t\t%s", ip,str);
            int slot = findSlot(ip);
            printf("found it in slot %d\n",slot);
            if (activeConnections())
            {
                dumpClientInfo();
            }
            int firstOpenSlot = -1;
            int found = -1;
            for (unsigned int i=0;i<100;i++) 
            {
               if (firstOpenSlot==-1 &&  si[i].step ==0)
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
         si[firstOpenSlot].step = 1;
         strcpy(si[firstOpenSlot].ip,ip);
         found = firstOpenSlot;
            }
      if (found!=-1)
            {
                 switch (si[found].step)
                 {
                    case 0: printf("error\n");
                            exit(1);
                            break;
                    case 1:
                           // The user must say hello in the begining.
         if (strstr(str,"ello"))
                           {
                              si[found].step++;
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
                              si[found].step++;
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
                              si[found].step=1;  // wait for name
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
