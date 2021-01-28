#include <stdio.h>
void strcpy(char *des,char *src)
{
  while(*src)
  {
    *des++ = *src++;
  }
  *des = '\0';
}
