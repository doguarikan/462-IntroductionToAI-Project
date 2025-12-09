NAME = boardGame
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I/opt/homebrew/include
SOURCES =  main.c minmax.c game.c
LDFLAGS = -L/opt/homebrew/lib -lraylib \
		-framework Cocoa -framework IOKit -framework OpenGL

OBJS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)
	

$(OBJS): $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:fclean all