#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include <string.h>
#include <openssl/sha.h>
#include "utilities.h"

int main(){
  FILE * W = fopen ("keywords", "r");
  FILE * Tsig = fopen ("signatures", "w");
  FILE * secret_file = fopen ("secrets", "r");
  FILE * param_file = fopen ("param.txt", "r");
  char param_buf[PARAM_LENGTH];
  param_buf[0] = 0; //null char at beginning


  pbc_param_t param;
  pairing_t pairing;

  //pbc_param_init_a_gen (param, LAMBDA, 128);
  //get parameters
  read_entire_file (param_file, param_buf);
  fclose (param_file);

  pbc_param_init_set_str(param, param_buf);
  pairing_init_pbc_param (pairing, param);

  element_t secret_key, g, sw_seed, tag_seed, m, pbc_id_hash, h;
  element_t sigma, temp, sig_temp;

  element_init_G2(g, pairing);
  element_init_G2(h, pairing);
  element_init_G2(temp, pairing);
  element_init_G2(sig_temp, pairing);
  element_init_G2(sigma, pairing);
  element_init_Zr(secret_key, pairing);
  element_init_G2(sw_seed, pairing);
  element_init_G2(tag_seed, pairing);
  element_init_Zr(m, pairing);
  element_init_Zr(pbc_id_hash, pairing);

  unsigned char ff[500];


  for (int i = 0; i < 1; i++){
      element_random (sw_seed);
      int gg = element_to_bytes_compressed (ff,sw_seed);
      printf ("alloc %d\n",gg);
      ff[element_length_in_bytes_compressed(sw_seed) + 1] = 45;
      for (int i = 0; i <= element_length_in_bytes_compressed(sw_seed); i++){
          printf ("%d\t", ff[i]);
      }
      printf ("\n_______________________\n");

      element_set (tag_seed,sw_seed);
      int len = element_from_bytes_compressed(sw_seed, ff);
      if (element_cmp (sw_seed,tag_seed))
      printf ("Different\n");
  }
  printf ("job end");
}
