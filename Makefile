# Source files
SRCS = main.cpp Server.cpp

# Compiler flags
CPPF = g++ -Wall -Werror -Wextra -std=c++98 -pthread -g

# Object files 
OBJS = ${SRCS:.cpp=.o}

# Executable name OR target
NAME = ircserv
RM = rm -f

BLUE = \033[1;34m
END = \033[0m

all: ${NAME}

# Rule to compile .cpp files into object files
%.o: %.cpp
	${CPPF} -c $< -o $@

${NAME}: ${OBJS}
	${CPPF} $^ -o $@
	@echo "${BLUE}To start this program, type ./${NAME} <port> <password>${END}"

# Rule to clean the compiled files
clean:
	${RM} ${OBJS} ${NAME}

fclean: clean
	${RM} ${NAME}

re: fclean ${NAME}

.PHONY: all clean fclean re

