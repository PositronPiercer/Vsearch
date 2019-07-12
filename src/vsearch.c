#include <stdio.h>
#include <stdlib.h>
#include "vsearch.h"
#include <stdbool.h>


int main(int argc, char *argv[]){
    char buff[500];
    int choice;

    if (argc>1){
        choice = atoi(argv[1]);
    }else{
        printf("Gives at least one argument\n");
        printf ("1. Keygen\n2. Build\n3. Search\n4. Update\n");
        exit(0);
    }

    while(1){
    switch (choice) {
            case 0:
                printf("Exit successfully\n" );
                printf("---------------------------------\n" );
                exit(0);
            case 1:
                printf("---------------Key generation starts-------\n" );
                printf("Key generation starts\n" );
                keygen();
                printf("Key generated successfully\n" );
                printf("---------------------------------\n" );
                exit(0);
            case 2:
                printf("---------------Build index starts-------\n" );
                printf("Build index starts\n");
                build();
                printf("Build successful\n" );
                printf("---------------------------------\n" );
                exit(0);
            case 3:
                printf("--------------- search begins-------\n" );
                printf ("Enter search keyword : ");
                memset(buff,0,500);
                scanf ("%s", buff);
                token s_token = search_token(buff);
                search_server (s_token);
                owner_search(buff);
                if (auditor_verification (buff))
                    printf ("Accept\n");
                else printf ("Reject\n");
                break;
            case 4:
                printf("--------------- Update begins-------\n" );
                printf ("Update: Enter file id : ");
                memset(buff,0,500);
                scanf ("%s", buff);
                //fgets (buff, MAX_ID_LENGTH, stdin);
                update (buff);
                break;
            default: printf ("Invalid choice\n"); exit(0);
        }
        printf("---------------------------------\n" );
        printf ("0. Exit\n3. Search\n4. Update\n");
        printf ("Enter choice : ");
        scanf ("%d", &choice);
    }
}
