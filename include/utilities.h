#define MAX_KEYWORD_LENGTH 50+8*40 //this has to accomodate Ks or Kt during concat
#define SHA_DIGEST_LENGTH 500
#define MAX_KEY_LENGTH 8*100
#define MAX_ID_LENGTH 120
#define MAX_NID 10
#define LAMBDA 256
#define PARAM_LENGTH 250
#define SIGNATURE_LENGTH 8*100

#include <stdbool.h>

typedef struct n{
  char * tag;
  char * tau_sigma_f;
}token;


extern void read_entire_file (FILE * fileptr, char * obuf);
extern void F (unsigned char * ibuf, unsigned char * obuf);
extern const char *byte_to_binary(int x);
extern  unsigned long int R (unsigned char * ibuf);
extern FILE * get_id_file(char * keyword);
extern void binary2bytes(unsigned char * ibuf,unsigned char * obuf);
extern void set_q (unsigned char * key);
extern void byte2binary_total (unsigned char * ibuf, unsigned char * obuf);
extern int get_keywords_from_file (FILE * input_file, int count, char ** keyword_present, char ** keyword_all, int * kcount);
extern bool keyword_is_present (int count, char ** keyword_all, char * buff);
extern FILE * create_new_id_file(char * keyword);
extern int print_to_binary(int length,FILE * secret_file,char * secret_type, char * sec);
extern FILE *common_file_open(char * scrFileName, char * mode);
extern int common_file_close(FILE *filePtr);
