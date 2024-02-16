NAME = ircserv
SRCS = main.cpp Server.cpp

CPPF = g++ -Wall -Werror -Wextra -std=c++98 -pthread -g

RM = rm -f

BLUE = \033[1;34m
END = \033[0m

all: ${NAME}

${NAME}:
	${CPPF} ${SRCS} -o ${NAME}
	@echo "${BLUE}To start this program, type ./${NAME} <port> <password>${END}"

fclean:
	${RM} ${NAME}

re: fclean all

.PHONY: re fclean all
