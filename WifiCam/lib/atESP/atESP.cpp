/*
 * ESP AT firmware library
 *
 */

#include <stdlib.h>
#include <SoftwareSerial.h>
#include "atESP.h"

/* Global buffer for command result */
char str[1024];


/* Send command to the ESP */

int atESP_Send(SoftwareSerial* ss, char* cmd, char* buff, int* sz)
{
  int i,n,k,ok,res;

  k=0;
  ok=0;

  /* Send command */
  ss->println(cmd);

  while(!ok)
  {
    if ( (n=ss->available()) )
    {

      /* Fill buffer with new data */
      for(i=0;i<n;i++)
      {
        buff[k+i] = ss->read();
      }
      /* Update counter */
      k+=i;

      /* Look for "OK\r" that terminates an AT command response */
      for(i=0;i<k-2;i++)
      {
        if ( (buff[i] == 'O') && ( (buff[i+1] == 'K') || (buff[i+1] == 'R') ) && (buff[i+2] == '\r') )
        {
          /* Found ! */
          ok = 1;
          res = buff[i+1] == 'K' ? EXIT_SUCCESS : EXIT_FAILURE ;
        }
      }

    }

  }

  /* Set buffer size */
  *sz = k;
  /* Null terminate buffer */
  buff[k] = 0;

  return res;
}


/* CWLAP */
int atESP_CWLAP(SoftwareSerial* ss, char* ssid[], int* n)
{
  int k,i,j,l,id;

  /* Send AT+CWLAP and get a result */
  while ( atESP_Send(ss,(char*)"AT+CWLAP", str, &k) != EXIT_SUCCESS )
  {
    ;
  }

  for(i=0,j=0;i<k-3;i++)
  {
    /* Start of a SSID */
    if ( (str[i]=='(')  && (str[i+2]==',') && (str[i+3]=='"') )
    {
      id = 1;
      l=0;
      i+=4;
    }

    /* End of a SSID */
    if ( (str[i]=='"') && (str[i+1]==',') && (id) )
    {
      id = 0;
      ssid[j][l] = 0;
      l=0;
      j++;
      i+=2;
    }

    /* SSID */
    if (id)
    {
      ssid[j][l] = str[i];
      l++;
    }

  }

  *n = j;

  return EXIT_SUCCESS;

}
