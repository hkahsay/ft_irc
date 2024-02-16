#include "Server.hpp"

Server:: Server(int port, char *password): _port(port), _password(password)
{
	//init everything from zero
	(void)_password;
	_servSockfd = 0;
	memset(&_server_address, 0, sizeof(_server_address));
	bzero(&_hints, sizeof(struct addrinfo));
	_servinfo = 0;

}

Server:: ~Server()
{

}

void	Server::launch()
{
	_hints.ai_family = AF_UNSPEC;
	/*protocol family for socket ipv4. the caller will accept any
    	protocol family supported by the operating system.*/
	_hints.ai_socktype = SOCK_STREAM;	//tcp stream sockets socket type
	_hints.ai_flags = AI_PASSIVE;		//input flags
	int	status = getaddrinfo(NULL, std::to_string(_port).c_str(), &_hints, &_servinfo);
	//see man getaddrinfo(The getaddrinfo() function is used to get a list of IP addresses and port)
	if(status != 0)
	{
		std::string error_msg = "getaddrinfo error: " + std::string(gai_strerror(status));
		throw Server::ServerException( error_msg.c_str() );
	}
	std::cout << "Getting address info successful." << std::endl;
	_servSockfd = socket(PF_INET, SOCK_STREAM, 0);
		//creates an endpoint for communication and returns a descriptor.
	if(_servSockfd < 0)
	{
		std::string error_msg = "Socket error: " + std::string(strerror(errno));
		throw ServerException(error_msg.c_str());
	}
	std::cout << "returns socket descriptor successfully." << std::endl;
	//bind -- bind a name to a socket
	//bind(int socket, const struct sockaddr *address, socklen_t address_len);
	int res = bind(_servSockfd, (const struct sockaddr *)&_server_address, sizeof(_server_address));
	if (res < 0)
	{
		std::string error_msg = "Bind error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
	std::cout	<< "Socket fd " << _servSockfd << " bound successfully."
				<< " Familly "
				<< getProtocolFamilyName(_server_address.sin_family)
				<< " Port "
				<< ntohs(_server_address.sin_port)
				<< " Address "
				<< inet_ntoa(_server_address.sin_addr) << std::endl;
	int	status_listen = ::listen( _servSockfd, MAX_CONNECTIONS );
	if (status_listen < 0)
	{
		std::string error_msg = "Listen error: " + std::string( strerror(errno) );
		throw ServerException( error_msg.c_str() );
	}
	std::cout << "Socket fd " << _servSockfd << " is listening" << std::endl;
	accept();


}

void	Server::accept(void)
{
	int		ret_val, i;
	const int BUFFER_SIZE = 5000;
	char buffer[BUFFER_SIZE];
	/* Initialize all_connections and set the first entry to server fd */
	for (i = 0; i < MAX_CONNECTIONS; ++i)
	{
		all_connections[i] = -1;
	}
	all_connections[0] = _servSockfd;
	while (1)
	{
		FD_ZERO( &read_fd );
		/* Set the fd_set before passing it to the select call */
		for (i = 0; i < MAX_CONNECTIONS; ++i)
		{
			if (all_connections[i] >= 0)
			{
				FD_SET(all_connections[i], &read_fd);
			}
		}
		/* Invoke select() and then wait! */
		printf("\nUsing select() to listen for incoming events\n");
		ret_val = select(_servSockfd, &read_fd, NULL, NULL, NULL);
		/* select() woke up. Identify the fd that has events */
		if (ret_val >= 0)
		{
			printf("Select returned with %d\n", ret_val);
			/* Check if the fd with event is the server fd */
			if(FD_ISSET(_servSockfd, &read_fd))
			{
				/* accept the new connection */
				printf("Returned fd is %d (server's fd)\n", _servSockfd);
				new_fd = ::accept( _servSockfd, ( struct sockaddr * )&new_addr, &addrlen );
				if (new_fd >= 0)
				{
					printf("Accepted a new connection with fd: %d\n", new_fd);
					for ( i = 0; i < MAX_CONNECTIONS; i++ )
					{
						if (all_connections[i] < 0)
						{
							all_connections[i] = new_fd;
							break;
						}
					}
				}
				else
				{
					fprintf(stderr, "accept failed [%s]\n", strerror(errno));
				}
				ret_val--;
				if ( !ret_val ) continue;
			}
			/* Check if the fd with event is a non-server fd */
			for ( i = 1; i < MAX_CONNECTIONS; ++i )
			{
				if ( ( all_connections[i] > 0 ) && ( FD_ISSET( all_connections[i], &read_fd ) ) )
				{
					/* read incoming data */
					printf("Returned fd is %d [index, i: %d]\n", all_connections[i], i);
					ret_val = recv( all_connections[i], buffer, BUFFER_SIZE, 0 );
					if ( ret_val == 0 )
					{
						printf("Closing connection for fd:%d\n", all_connections[i]);
						close( all_connections[i] );
						all_connections[i] = -1; /* Connection is now closed */
					}
					if (ret_val > 0)
					{
						printf("Received data (len %d bytes, fd: %d): %s\n", ret_val, all_connections[i], buffer);
					}
					if (ret_val == -1)
					{
						printf("recv() failed for fd: %d [%s]\n", all_connections[i], strerror(errno));
						break;
					}
				}
				ret_val--;
				if ( !ret_val ) continue;
			} /* for-loop */
		} /* (ret_val >= 0) */
	} /* while(1) */

	/* Last step: Close all the sockets */

}

// Server::ServerException::ServerException( const char *error_msg ) : _error_msg(error_msg) {

// 	return;
// };

std::string Server::getProtocolFamilyName(int family)
{
    switch(family)
	{
        case AF_INET:
            return "IPv4";
        case AF_INET6:
            return "IPv6";
        default:
            return "Unknown";
    }
}