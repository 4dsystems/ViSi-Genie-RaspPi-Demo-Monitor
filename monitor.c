/*
 * genieMon.c:
 *	Monitor all inputs from a Visi-Genie display and print
 *	them to the scree.
 *
 *	Gordon Henderson, December 2012
 ***********************************************************************
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include <geniePi.h>

char *genieCommandNames [] =
{
  "Read Obj",
  "Write Obj", 
  "Write Str",
  "Write StrU",
  "Write Contrast",
  "Report Obj",
  "Command #6",
  "Report Event",
} ;

char *genieObjectNames [] =
{
  "DIP Switch",		// 0
  "Knob",		// 1
  "Rocker Switch",	// 2
  "Rotary Switch",
  "Slider",
  "Track Bar",
  "Win Button",
  "Angular Meter",
  "Cool Gauge",
  "Custom Digits",
  "Form",
  "Gauge",
  "Image",
  "Keyboard",
  "LED",
  "LED Digits",
  "Meter",
  "Strings",
  "Thermometer",
  "User LED",
  "Video",
  "Static Text",
  "Sound",
  "Timer",
} ;


/*
 *********************************************************************************
 * main:
 *********************************************************************************
 */

int main ()
{
  struct genieReplyStruct reply ;
  int    seqNum = 0 ;
  char *objStr, *cmdStr ;

  printf ("\n\n\n\n") ;
  printf ("Visi-Genie Monitor\n") ;
  printf ("==================\n") ;


// Genie display setup
//	Using the Raspberry Pi's on-board serial port.

  if (genieSetup ("/dev/ttyAMA0", 115200) < 0)
  {
    fprintf (stderr, "rgb: Can't initialise Genie Display: %s\n", strerror (errno)) ;
    return 1 ;
  }

// Now just poll the display for an event

  for (;;)
  {
    while (genieReplyAvail ())
    {
      genieGetReply    (&reply) ;
      printf ("(%4d) ", seqNum++) ;
      printf ("[%02X %02X %02X %04X] ", reply.cmd, reply.object, reply.index, reply.data) ;

      if (reply.cmd > 7)
	cmdStr = "Unknown";
      else
	cmdStr = genieCommandNames [reply.cmd] ;

      if (reply.object > 23)
	objStr = "Unknown";
      else
	objStr = genieObjectNames [reply.object] ;

      printf ("%12s: %-20s %2d, %5d [%04X]",
	cmdStr, objStr, reply.index, reply.data, reply.data) ;

      if (isprint (reply.data))
	printf (" (%c)", reply.data) ;

      printf ("\n") ;
    }

    usleep (10000) ;
  }

  return 0 ;
}
