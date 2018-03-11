#ifndef IO_H
#define IO_H

int socket_setup(void);
int socket_connect(void);
int socket_cleanup(void);
int socket_sendfile(void);

#endif
