# OperatingSystems

This repository contains code for the assignments I completed for my course CS232 - Operating Systems. The language I use is C along with bash scripting.

Shell.c provides an interface for my own mini shell.

malloc contains all the files to implement my own malloc function with the following functions:
- int my_init();
- void* my_malloc(int size);
- void my_free(void* ptr);
- void* my_calloc(int num, int size);
- void* my_realloc(void *, int size);
- void my_coalesce();
- void my_showfreelist();
- void my_uninit();
- void my_uninit();

Lastly, communicate contains all the files to implement a rudimentary chatting application using socket programming, threads, and synchronization. The application will run on a client/server model. For simplicity we’d say that one instance of the server will be running and multiple clients can connect to the server and chat with each other using this server program as an intermediary. The server and clients may run on the same PC or on different ones.
