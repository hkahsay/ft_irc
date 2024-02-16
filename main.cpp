#include"Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr<<"Usage: "<<argv[0]<< " <port> <password>" <<std::endl;
		return 1;
	}
	try
	{
		Server	server(atoi(argv[1]), argv[2]);
		server.launch();
	}
	catch(Server::ServerException& e)
	{
		std::cerr << e.what() << '\n';
	}
}