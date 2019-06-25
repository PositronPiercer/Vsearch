#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include <string.h>
#include <openssl/sha.h>
#include "utilities.h"

void build (){
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
  element_init_Zr(sw_seed, pairing);
  element_init_Zr(tag_seed, pairing);
  element_init_Zr(m, pairing);
  element_init_Zr(pbc_id_hash, pairing);


  char key_[MAX_KEY_LENGTH] = "";

  //read the keys from file
  printf ("Loading Keys...");
  fscanf (secret_file,"%*s %s",key_);
  unsigned char * g_bytes = (unsigned char *)malloc (strlen (key_) * sizeof (unsigned char));
  binary2bytes (key_, g_bytes);
  element_from_bytes_compressed (g, g_bytes);
  free (g_bytes);
  fscanf (secret_file,"%*s %s",key_);
  element_set_str(secret_key, key_, 2);
  fscanf (secret_file,"%*s %s",key_);
  //skip public_key
  fscanf (secret_file,"%*s %s",key_);
  element_set_str(sw_seed, key_, 2);
  fscanf (secret_file,"%*s %s",key_);
  element_set_str(tag_seed, key_, 2);
  printf("Done.\n");

  unsigned char w[MAX_KEYWORD_LENGTH] = "";
  unsigned char w2[MAX_KEYWORD_LENGTH] = "";
  char id[MAX_ID_LENGTH] = "";
  unsigned char pos[SHA_DIGEST_LENGTH] = "";
  unsigned char sw[SHA_DIGEST_LENGTH + MAX_NID] = ""; //has to store i during concat
  unsigned char tag[SHA_DIGEST_LENGTH + MAX_ID_LENGTH + MAX_NID] = ""; //has to store id and i during concat
  unsigned char tag_temp[SHA_DIGEST_LENGTH + MAX_ID_LENGTH + MAX_NID] = "";
  FILE * id_file;
  int c;

  mpz_t q, gmp_id, m_temp, r, gmp_temp;
  mpz_init (q);
  mpz_init (gmp_id);
  mpz_init (m_temp);
  mpz_init (r);
  mpz_init (gmp_temp);
  set_q (key_); //get the group order
  mpz_set_str (q, key_, 10);



  printf ("Building...\n");

  while (fscanf (W,"%s %d", w, &c) == 2){
    printf ("Keyword : %s\n", w);
    id_file = get_id_file (w);
    strcpy (w2,w);
    //appending sw_seed to the end of w
    element_to_bytes (w + strlen (w), sw_seed);
    element_to_bytes (w2 + strlen (w2), tag_seed);
    F (w, sw);
    F (w2, tag);
    strcpy (tag_temp,tag);
    for (int i = 1; i <= c; i++){
      strcpy(tag,tag_temp);
      fgets (id, MAX_ID_LENGTH, id_file);
	  //remove \n from end
	  id[strlen (id) - 1] = 0;
      //calculate r
      //append i to the end of sw
      sprintf (sw + strlen (sw), "%d", i);
      unsigned long int rtemp = R (sw);
	  //remove end digit from sw
	  sw[strlen (sw) - 1] = 0;
      mpz_set_ui (r, rtemp);

      byte2binary_total (id, key_);
      mpz_set_str (gmp_id, key_, 2);

      //calculate m
      mpz_mul (gmp_temp, r, gmp_id);
      mpz_mod (m_temp, gmp_temp, q);
      element_set_mpz (m, m_temp);

      //  sign
      element_pow_zn (temp, g, m);
      element_pow_zn (sigma, temp, secret_key);
      //calculate pos
      strcat (tag, id);
      sprintf (tag + strlen (tag), "%d", i);

      F (tag, pos);
      tag[strlen (tag) - 1] = 0;
      int sigma_length = element_length_in_bytes_compressed (sigma);

      unsigned char * sigma_bytes = (unsigned char *)malloc (sigma_length * sizeof (unsigned char));
      element_to_bytes_compressed (sigma_bytes, sigma);
      //print to file
      for (int t = 0; t < strlen (pos); t++){
        fprintf (Tsig, "%s", byte_to_binary (pos[t]));
      }

      fprintf(Tsig, " ");

			if (sigma_length == 0){
				printf ("Failed to sign\n");
			}
      for (int t = 0; t < sigma_length; t++){
        fprintf (Tsig, "%s", byte_to_binary (sigma_bytes[t]));;
      }
      fprintf(Tsig, "\n");

    }
    fclose (id_file);
	}
  fclose (W);
  fclose (Tsig);
  fclose (secret_file);


}


int main (){
  //get the Keys

	build ();
}
