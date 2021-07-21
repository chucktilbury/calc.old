# Custom make file.
TARGET	=	calc
SRCS	=	main.c \
			ast.c \
			visit.c \
			error.c \
			memory.c \
			symbols.c
SRCS1	=	parse.c \
			scan.c
OBJS	=	$(SRCS:.c=.o)
OBJS1	=	$(SRCS1:.c=.o)
CARGS	=	-g -O0 -Wall -Wextra
INCDIRS	=	-I.
LIBDIRS	=	-L.
LIBS	=	-lreadline -lm
CC		=	gcc

all: $(TARGET)

.c.o:
	$(CC) $(CARGS) $(INCDIRS) -c $< -o $@

$(TARGET): $(OBJS1) $(OBJS)
	$(CC) $(CARGS) -o $(TARGET) $(OBJS) $(OBJS1) $(LIBS)

parse.c parse.h: parse.y
	bison --report=lookahead -tvdo parse.c parse.y

scan.c: scan.l parse.h scan.h
	flex -o scan.c scan.l

clean:
	-rm -f $(TARGET) $(OBJS) $(OBJS1) $(SRCS1) parse.output

