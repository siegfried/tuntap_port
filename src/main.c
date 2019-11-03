#include <pthread.h>
#include <stdio.h>
#include "tuntap.h"

int
main(int argc, char **argv)
{
        int fd;
        pthread_t thread;

        if (argc != 2) {
                perror("INCORRECT ARGUMENT NUMBER");
                return 1;
        }

        fd = tuntap_open(argv[1]);

        if (pthread_create(&thread, NULL, tuntap_read, &fd) != 0) {
                perror("CANNOT CREATE THREAD");
                return 1;
        }

        tuntap_write(&fd);

        return 1;
}
