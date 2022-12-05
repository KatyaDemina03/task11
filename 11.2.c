#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define BELLS "\007\007\007"  /* Sound signal ASCII */

sig_atomic_t alarm_flag = FALSE;

/* Signal SIGALRM handler */
void setFlag(int sig) {
    alarm_flag = TRUE;
}

int main(int argc, char **argv) {
    int nsecs, j;
    pid_t pid;
    static struct sigaction act;

    if (argc < 2) {
        fprintf(stderr, "Wrong strating command.\nMust be: Task2 seconds_number message\n");
        exit(EXIT_FAILURE);
    }

    if ((nsecs = atoi(argv[1])) <= 0) {
        fprintf(stderr, "Task2: invalid time\n");
        exit(EXIT_FAILURE);
    }

    switch(pid = fork()) {
        case -1: /* Error */
            perror("Task2");
            exit(EXIT_FAILURE);
        case 0: /* Child process */
            break;
        default: /* Parent process */
            printf("Chile process Task2 with id %d is working\n", pid);
            printf("Wait a signal after %d second\n", nsecs);
            exit(EXIT_SUCCESS);
    }

    /* Set timer handler */
    act.sa_handler = setFlag;
    sigaction(SIGALRM, &act, NULL);

    /* Set timer */
    alarm(nsecs);

    /* Stop execution antil signal is catched */
    pause();

    /* If signal SIGALRM was catched put message */
    if (alarm_flag == TRUE) {
        printf(BELLS);
        for (j = 2; j < argc; j++) {
            printf("%s ", argv[j]);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}


