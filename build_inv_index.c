#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"

bool keyword_is_present (int count, char ** keyword_all, char * buff){
    for (int i = 0; i < count; i++){
        if (!strcmp (keyword_all[i], buff)) {
            strcpy (keyword_all[i],"");
            return true;
        }
    }
    return false;
}

int get_keywords_from_file (FILE * input_file, int count, char ** keyword_present, char ** keyword_all, int * kcount){
    //returns the number of keywords present
    //fills keyword_all , kcount and keyword_present
    char ** keyword_all_duplicate;
    keyword_all_duplicate = malloc(count * sizeof(char*));
    for(int i = 0; i < count; i++) {
        keyword_all_duplicate[i] = malloc((MAX_KEYWORD_LENGTH) * sizeof(char));
    }
    FILE * keywords = fopen ("keywords", "r");
    char buff[MAX_KEY_LENGTH];
    int tmpInt;


    for (int i = 0; i < count; i++){
        fscanf (keywords, "%s %d", buff, &tmpInt);
        strcpy (keyword_all [i], buff);
        strcpy (keyword_all_duplicate [i], buff);
        kcount[i] = tmpInt;
    }
    fclose (keywords);
    int j = 0;
    while (fscanf (input_file, "%s", buff) == 1){
        if (keyword_is_present (count, keyword_all_duplicate, buff)){
            strcpy (keyword_present[j], buff);
            j++;
        }
    }
    free (keyword_all_duplicate);
    return j;

}
