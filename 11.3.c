#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int signo);

int main() {
    pid_t pid;
    static struct sigaction act;
    sigset_t set1, set2;

    sigfillset(&set1);
    sigdelset(&set1, SIGUSR1);
    sigdelset(&set1, SIGINT); // To Stop By Ctrl+C
    sigfillset(&set2);
    sigdelset(&set2, SIGINT); // To Stop By Ctrl+C
    sigprocmask(SIG_SETMASK, &set2, NULL);
    act.sa_handler = handler;
    sigfillset(&act.sa_mask);
    sigdelset(&act.sa_mask, SIGUSR1);
    act.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &act, NULL);
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork ERROR!\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { /*Child*/
        fprintf(stderr, "Child pid %d\n", (int) getpid());
        while (1) {
            kill(getppid(), SIGUSR1);
            sigsuspend(&set1);
        }
    } else { /*parent*/
        fprintf(stderr, "Parent pid %d\n", (int) getpid());
        while (1) {
            sigsuspend(&set1);
            kill(pid, SIGUSR1);
        }
    }

    return EXIT_SUCCESS;
}

void handler(int signo) {
    fprintf(stderr, "The pid: %d. ", (int) getpid());
    psignal(signo, "The received");
}

