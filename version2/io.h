#ifndef IO_H
#define IO_H

int decrypt(const char *string, char result[]);
int socket_setup(void);
int socket_connect(void);
int socket_cleanup(void);
int socket_sendfile(void);

#endif
