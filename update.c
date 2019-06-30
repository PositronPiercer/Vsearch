#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include <string.h>
#include "utilities.h"

int getIndex (int count, char * ibuf, char set[count][MAX_KEYWORD_LENGTH]){
    int index = -1;
    for (;index < count; index++){
        if (!strcmp (set[index], ibuf))
            return index;
    }

}


void update (char * id){
    //--------------
    FILE * secret_file = fopen ("secrets", "r");
    FILE * param_file = fopen ("param.txt", "r");
    FILE * Tsig = fopen ("signatures", "a");
    char param_buf[PARAM_LENGTH];
    param_buf[0] = 0; //null char at beginning


    pbc_param_t param;
    pairing_t pairing;

    //get parameters
    read_entire_file (param_file, param_buf);
    fclose (param_file);

    pbc_param_init_set_str(param, param_buf);
    pairing_init_pbc_param (pairing, param);

    element_t secret_key, g, sw_seed, tag_seed, m, h;
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


    char buff[MAX_KEY_LENGTH] = "";

    //read the keys from file
    printf ("Loading Keys...");
    fscanf (secret_file,"%*s %s",buff);
    unsigned char * g_bytes = (unsigned char *)malloc (strlen (buff) * sizeof (unsigned char));
    binary2bytes (buff, g_bytes);
    element_from_bytes_compressed (g, g_bytes);
    free (g_bytes);
    fscanf (secret_file,"%*s %s",buff);
    element_set_str(secret_key, buff, 2);
    fscanf (secret_file,"%*s %s",buff);
    //skip public_key
    fscanf (secret_file,"%*s %s",buff);
    element_set_str(sw_seed, buff, 2);
    fscanf (secret_file,"%*s %s",buff);
    element_set_str(tag_seed, buff, 2);
    printf("Done.\n");

    unsigned char w[MAX_KEYWORD_LENGTH] = "";
    unsigned char w2[MAX_KEYWORD_LENGTH] = "";
    unsigned char pos[SHA_DIGEST_LENGTH] = "";
    unsigned char sw[SHA_DIGEST_LENGTH + MAX_NID] = ""; //has to store i during concat
    unsigned char tag[SHA_DIGEST_LENGTH + MAX_ID_LENGTH + MAX_NID] = ""; //has to store id and i during concat
    //unsigned char tag_temp[SHA_DIGEST_LENGTH + MAX_ID_LENGTH + MAX_NID] = "";

    mpz_t q, gmp_id, m_temp, r, gmp_temp;
    mpz_init (q);
    mpz_init (gmp_id);
    mpz_init (m_temp);
    mpz_init (r);
    mpz_init (gmp_temp);
    set_q (buff); //get the group order
    mpz_set_str (q, buff, 10);
    //--------------
    FILE * newFile = fopen (id, "r");
    FILE * keyword_file = fopen ("keywords", "r");
    int nKeywords = 0;
    //count number of nKeywords
    while (fscanf (keyword_file, "%s %*d", buff) == 1) nKeywords++;
    printf ("#keywords %d\n",nKeywords);
    char keywordPresent[nKeywords][MAX_KEYWORD_LENGTH];
    char keywordAll[nKeywords][MAX_KEYWORD_LENGTH];
    int kcount[nKeywords]; //stores the count of each keyword
    int nKeywordsPresent = get_keywords_from_file (newFile, nKeywords, keywordPresent, keywordAll, kcount);
    printf ("#keywords present %d\n", nKeywordsPresent);
    for (int t = 0; t < nKeywordsPresent; t++){
        printf ("%s\t", keywordPresent[t]);
    }
    printf ("\n");

    fclose (keyword_file);
    for (int i = 0; i < nKeywordsPresent; i++){
        //update the count
        strcpy (buff, keywordPresent[i]);
        int index = getIndex (nKeywordsPresent, buff, keywordAll);
        kcount[index]++;

        //add the id to the id file
        if (kcount[index] == 1){
            //new file needs to be created
            FILE * id_file = create_new_id_file(buff);
            fprintf (id_file, "%s\n", id);
            fclose (id_file);
        }
        else{
            // should check whether the id is already present or not
            FILE * id_file = get_id_file (buff);
            fseek (id_file, 0 , SEEK_END);
            fprintf (id_file, "%s\n", id);
            fclose (id_file);
        }

        //create the pos and signature pair

        strcpy (w, buff);
        strcpy (w2,w);
        //appending sw_seed to the end of w
        element_to_bytes (w + strlen (w), sw_seed);
        element_to_bytes (w2 + strlen (w2), tag_seed);
        F (w, sw);
        F (w2, tag);
        // strcpy (tag_temp,tag)
        // strcpy(tag,tag_temp);

        //calculate r
        //append i to the end of sw
        sprintf (sw + strlen (sw), "%d", kcount[index]);
        unsigned long int rtemp = R (sw);
        //remove end digit from sw
        int c_copy = kcount[index];
        while (c_copy){
        sw[strlen (sw) - 1] = 0;
        c_copy /= 10;
        }
        mpz_set_ui (r, rtemp);

        byte2binary_total (id, buff);
        mpz_set_str (gmp_id, buff, 2);

        //calculate m
        mpz_mul (gmp_temp, r, gmp_id);
        mpz_mod (m_temp, gmp_temp, q);
        element_set_mpz (m, m_temp);



        //  sign
        element_pow_zn (temp, g, m);
        element_pow_zn (sigma, temp, secret_key);

        //calculate pos
        strcat (tag, id);
        sprintf (tag + strlen (tag), "%d", kcount[index]);

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

    //write back
    FILE * k = fopen ("keywords", "w");
    for (int i = 0; i < nKeywords; i++){
        fprintf (k, "%s %d\n", keywordAll[i], kcount[i]);
    }
    fclose (k);
}
