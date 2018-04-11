/*
 * ESP AT firmware library
 *
 */

#include <stdlib.h>
#include <string.h>
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

      /* Look for "OK\r" or "ERROR\r" that terminates an AT command response */
      for(i=0;i<k-2;i++)
      {
        if ( (buff[i] == 'O') && ( (buff[i+1] == 'K') || (buff[i+1] == 'R') ) && (buff[i+2] == '\r') )
        {
          /* Found ! */
          ok = 1;
          res = buff[i+1] == 'K' ? EXIT_SUCCESS : EXIT_FAILURE ;
        }

        /* Look for "FAIL\r" */
        if ( (buff[i] == 'I') && (buff[i+1] == 'L') && (buff[i+2] == '\r') )
        {
          /* Found ! */
          ok = 1;
          res = EXIT_FAILURE ;
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
int atESP_getCWLAP(SoftwareSerial* ss, char* ssid[], int* n)
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



/* CWMODE? */
int atESP_getCWMODE(SoftwareSerial* ss)
{
  int i,k;
  char c;

  /* Send AT+CWLAP and get a result */
  while ( atESP_Send(ss,(char*)"AT+CWMODE?", str, &k) != EXIT_SUCCESS )
  {
    ;
  }

  /* Parse result */
  for(i=0;i<k-1;i++)
  {
    if (str[i] == ':')
    {
      /* Get number */
      c = str[i+1];
      break;
    }
  }

  switch(c)
  {
    case '1':
      return atESP_STA;

    case '2':
      return atESP_AP;

    case '3':
      return atESP_STAP;

    default:
      return EXIT_FAILURE;
  }

}


/* CWMODE */
int atESP_setCWMODE(SoftwareSerial* ss, int mode)
{
  int k;
  char buffer[12];

  /* Sanity Check */
  if ( (mode!=atESP_STA) && (mode!=atESP_AP) && (mode!=atESP_STAP) )
  {
    return EXIT_FAILURE;
  }

  sprintf(buffer,"AT+CWMODE=%d",mode);

  return atESP_Send(ss,buffer, str, &k);

}


/* CWJAP */
int atESP_setCWJAP(SoftwareSerial* ss, char* ssid, char* password)
{
  int k;
  char* buffer;

  /* Build commnand */
  buffer = (char*)malloc( (strlen("AT+CWJAP=\"\",\"\"")+strlen(ssid)+strlen(password))*sizeof(char) );
  if (buffer == NULL)
  {
    return EXIT_FAILURE;
  }
  sprintf(buffer,"AT+CWJAP=\"%s\",\"%s\"",ssid,password);

  atESP_Send(ss, buffer, str, &k);

  free(buffer);

  return EXIT_SUCCESS;
}


/* CWJAP? */
int atESP_getCWJAP(SoftwareSerial* ss, char* ssid)
{
  int i,j,k;

  if (atESP_Send(ss,"AT+CWJAP?", str, &k) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }

  /* Parse result */
  for(i=0;(i<k)||(str[i]!='"');i++)
  {
    ;
  }

  if (i==k)
  {
    return EXIT_FAILURE;
  }

  i++;
  for(j=0;str[i]!='"';j++,i++)
  {
    ssid[j]=str[i];
  }
  ssid[j]=0;


  return EXIT_SUCCESS;
}


/* CIFSR */
int atESP_getCIFSR(SoftwareSerial* ss, char* ip)
{
  int i,j,k;

  if (atESP_Send(ss,"AT+CIFSR", str, &k) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }

  /* Parse result */
  for(i=0;i<k-4;i++)
  {
    if ( (str[i] == 'S') &&  (str[i+1] == 'T') && (str[i+2] == 'A') && (str[i+3] == 'I') )
    {
      i+=7;
      break;
    }
  }

  if (i==k-4)
  {
    return EXIT_FAILURE;
  }

  for(j=0;str[i]!='"';j++,i++)
  {
    ip[j]=str[i];
  }
  ip[j]=0;


  return EXIT_SUCCESS;
}


/* CWQAP */
int atESP_setCWQAP(SoftwareSerial* ss)
{
  int k;
  return atESP_Send(ss,"AT+CWQAP",str,&k);
}


/* CIPMODE? */
int atESP_getCIPMODE(SoftwareSerial* ss)
{
  int i,k;
  char c;

  /* Send AT+CWLAP and get a result */
  while ( atESP_Send(ss,(char*)"AT+CIPMODE?", str, &k) != EXIT_SUCCESS )
  {
    ;
  }

  /* Parse result */
  for(i=0;i<k-1;i++)
  {
    if (str[i] == ':')
    {
      /* Get number */
      c = str[i+1];
      break;
    }
  }

  switch(c)
  {
    case '0':
      return atESP_NORMAL;

    case '1':
      return atESP_TRANSPARENT;

    default:
      return EXIT_FAILURE;
  }

}



/* CIPMODE */
int atESP_setCIPMODE(SoftwareSerial* ss, int mode)
{
  int k;
  char buffer[16];

  /* Sanity Check */
  if ( (mode!=atESP_NORMAL) && (mode!=atESP_TRANSPARENT) )
  {
    return EXIT_FAILURE;
  }

  sprintf(buffer,"AT+CIPMODE=%d",mode);

  return atESP_Send(ss,buffer, str, &k);

}


/* CIPSTART */
int atESP_setCIPSTART(SoftwareSerial* ss, char* proto, char* addr, int port)
{
  int k,res;
  char* buffer;

  /* Build commnand */
  buffer = (char*)malloc( (strlen("AT+CIPSTART=\"\",\"\",123456")+strlen(proto)+strlen(addr))*sizeof(char) );
  if (buffer == NULL)
  {
    return EXIT_FAILURE;
  }
  sprintf(buffer,"AT+CIPSTART=\"%s\",\"%s\",%d",proto,addr,port);

  res = atESP_Send(ss, buffer, str, &k);

  free(buffer);

  return   res;
}


/* CIPCLOSE */
int atESP_setCIPCLOSE(SoftwareSerial* ss)
{
  int k;
  return atESP_Send(ss,"AT+CIPCLOSE",str,&k);  
}
