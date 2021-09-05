#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    char *game_type = argv[1], *server_name = argv[2];
    int port_number = atoi(argv[3]);

    printf("%s %s %d\n", game_type, server_name, port_number);

    return 0;
}