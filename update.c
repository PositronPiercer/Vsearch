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
    char buff[MAX_KEY_LENGTH] = "";
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


    fclose (keyword_file);
    for (int i = 0; i < nKeywordsPresent; i++){
        //update the count
        strcpy (buff, keywordPresent[i]);
        int index = getIndex (nKeywordsPresent, buff, keywordAll);
        kcount[index]++;
    }

    //write back
    FILE * k = fopen ("keywords", "w");
    for (int i = 0; i < nKeywords; i++){
        fprintf (k, "%s %d\n", keywordAll[i], kcount[i]);
    }
    fclose (k);
}

int main(){
    char * id = "input_files/input1.txt";
    update (id);
}
