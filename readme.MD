Socket Server/Client Program
2803ICT - Assignment 1

AUTHOR

    Ethan Leet
    s5223103


ABOUT

    This program creates a server which accepts multiple client connections. Players (clients) connect to the server
    and take turns playing a game called 'numbers'. Each player types a number between 1 and 9, the first player
    to reach a score of at least 30 wins. A minimum of two players are required to play the game, with no maximum. 
    Players can win by either reaching 30 first or by being the last remaining player in the game. There is a turn time
    limit of 30 seconds, players who exceed this time limit will be removed from the game. Players can also leave
    at any time by typing 'quit'.


Compiling

    To compile this program the following files need to be in the same directory:

    client_includes.h
    server_includes.h
    LL_queue.h
    game_client.c
    game_server.c
    makefile.client
    makefile.server
    makefile

    Next, run 'make all -B' to force a new build of the program.


Run Time

    Server 
        The server needs to be run first. The server requires the following command line arguements to be passed:

        <port number> <game type> <max players>

        As such, the server can be run in the following way:

        ./game_server <port number> <game type> <max players>

    Client

        The client/s require the following command line arguements to be passed:

        <game type> <host name> <port number>

        As such, the client/s can be run in the following way:

        ./game_client <game type> <host name> <port number>
    
    Argument Description
    
        Port number: This is the port the server will set up and the client/s will connect to.

        Game type: This is always 'numbers'.

        Max players: This stipulates how many players the server would like to accept (minimum 2).

        Host name: This is the host name of the server system. The program will convert this host name
                   to an IP address which is used to connect to the server.



