#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include <string.h>
#include <openssl/sha.h>
#include "utilities.h"
#include <math.h>
void read_entire_file (FILE * fileptr, char * obuf){
  while (fgets (obuf + strlen (obuf), PARAM_LENGTH, fileptr));
}

void F (unsigned char * ibuf, unsigned char * obuf){
  // Pseudorandom Generator
  SHA256 (ibuf, strlen (ibuf), obuf);
}

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

unsigned long int R (unsigned char * ibuf){ //utility function
  //Pseudorandom number Generator
  mpz_t r,temp;
  mpz_init (r);
  mpz_init (temp);
  mpz_set_str (temp, ibuf, 20);//check the base
  gmp_randstate_t state;
  gmp_randinit_mt (state);
  gmp_randseed_ui (state, mpz_get_ui (temp));
  mpz_urandomb (r, state, LAMBDA);
  return mpz_get_ui(r);
}

FILE * get_id_file(char * keyword){
  //takes keyword as input and returns the file pointer for the file containing the ids
  char buff[100 + MAX_KEYWORD_LENGTH];
  strcpy (buff, "inv_index/");
  strcat (buff, keyword);
  strcat (buff, ".ids");
  FILE * id_file = fopen (buff,"r+");
  return id_file;
}

FILE * create_new_id_file(char * keyword){
    char buff[100 + MAX_KEYWORD_LENGTH];
    strcpy (buff, "inv_index/");
    strcat (buff, keyword);
    strcat (buff, ".ids");
    FILE * id_file = fopen (buff,"w");
    return id_file;
}

void binary2bytes(unsigned char * ibuf,unsigned char * obuf){
    int seperator = 1;
    int temp = 0;
    int j = 0;
    int i = 0;
    for (; i < strlen (ibuf); i++){
        temp += (int)(pow (2, (8 - seperator) % 8)) * (ibuf[i] - '0');

        if (seperator == 0){
            //write to buffer
            obuf[j++] = temp;
            temp = 0;
        }
        seperator = (seperator + 1) % 8;
    }
    obuf [j] = 0;
}

void set_q (unsigned char * key){
    FILE * param_file = fopen ("param.txt", "r");
    fscanf (param_file, "%*s %s", key);
    fscanf (param_file, "%*s %s", key);
}

void byte2binary_total (unsigned char * ibuf, unsigned char * obuf){
        int seperator = 1;
        int temp = 0;
        int j = 0;
        int i = 0;
        for (; i < strlen (ibuf); i++){
            sprintf (obuf + i * 8, "%s", byte_to_binary (ibuf[i]));
        }
        obuf [i * 8] = 0;


}

int print_to_binary(int length,FILE * secret_file,char * secret_type, char * sec){
    fprintf(secret_file,"%s ",secret_type);
    for(int i=0;i<length;i++){
        fprintf(secret_file,"%s",byte_to_binary(*(sec + i)));
    }
    fprintf(secret_file,"\n");
}
