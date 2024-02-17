# ft_irc

This project is a group project up to 3 person but I started with a mate called mariya. we belive we will learn a lot from this project.

see the subject in the root repository

here is what we did 
first read about the [concept, meaning and history about IRC](https://en.wikipedia.org/wiki/IRC)

[beej's guid](https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf)

[read](https://doc.lagout.org/programmation/unix/Unix%20Network%20Programming%20Volume%201.pdf)


FD_SET:

    FD_SET is a macro used in conjunction with the select() function in C and C++. It's part of the POSIX standard for handling I/O multiplexing.
    It's used to add a file descriptor to a set of file descriptors for monitoring read, write, or error conditions using select().
    It's typically used in server applications to wait for activity on multiple sockets simultaneously without having to block on each socket individually.
    Example usage:
		fd_set read_fd;
		FD_ZERO(&read_fd);
		FD_SET(socket_fd, &read_fd);


./ircserv 1337 asd

nc 127.0.01 1337
