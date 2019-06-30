#include <stdio.h>
#include <stdlib.h>
#include "vsearch.h"
#include <stdbool.h>
//#include "utilities.h"

int main(){
    char buff[500];
    printf ("1. Keygen\n");
    printf ("2. Build\n");
    printf ("3. Search\n");
    printf ("4. Update\n");
    int choice;
    while(1){
        printf ("Enter choice : ");
        scanf ("%d", &choice);
        switch (choice) {
            case 1:
                keygen();
                break;
            case 2:
                build();
                break;
            case 3:
                printf ("Enter keyword : ");
                scanf ("%s", buff);
                token s_token = search_token(buff);
                search_server (s_token);
                owner_search(buff);
                if (auditor_verification (buff))
                    printf ("Accept\n");
                else printf ("Reject\n");
                break;
            case 4:
                printf ("Enter file id : ");
                fgets (buff, MAX_ID_LENGTH, stdin);
                update (buff);
                break;
            default: printf ("Invalid choice\n");
        }
    }
}
