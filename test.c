#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

int main (){
    unsigned char i[800] = "";
    strcpy(i,"ball�m��j���D�9��B���_:��l��ro�����");
    unsigned char o[800]="";
    F (i,o);
    printf ("%s\n",o);
    strcpy (i,"ball�m��j���D�9��B���_:��l��ro����");
    F (i,o);
    printf ("%s\n",o);
}
