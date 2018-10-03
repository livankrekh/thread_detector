NAME = detector
CC = g++
SRC = src/Classifier.cpp src/main.cpp
FLAGS = -Wall -Wextra -Werror -std=c++11
OPENCV = `pkg-config --cflags --libs opencv`

all: $(NAME)

$(NAME): $(SRC:.cpp=.o)
	$(CC) -o $(NAME) $(SRC:.cpp=.o) $(FLAGS) $(OPENCV)

%.o: %.cpp
	$(CC) -o $@ -c $< $(FLAGS)

clean:
	rm -f $(SRC:.cpp=.o)

fclean: clean
	rm -f $(NAME)

re: fclean all