#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.hpp"

class Server {
	private:
		int		_port;
		const char *			_password;
		int						_servSockfd;				// for socket()
		struct sockaddr_in		_server_address;		// for bind()
		struct addrinfo 		_hints, *_servinfo;	// for getaddrinfo()

		int						all_connections[MAX_CONNECTIONS];
		fd_set					read_fd;
		int						new_fd;
		struct sockaddr_storage	new_addr;
		socklen_t				addrlen;
	public:
		Server(int port, char *password);
		~Server(void);
		void	launch( void );
		void	accept(void);
		std::string		getProtocolFamilyName(int family);
		class ServerException : public std::exception {

		private:
			const char *				_error_msg;

		public:
			ServerException( const char *error_msg ): _error_msg(error_msg) 
			{

			}
			virtual const char *what() const throw()
			{
				return _error_msg;
			}
	};

};

#endif