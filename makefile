C_PROG= task_server.c
PROG=$(C_PROC:.c=)

C_SRC= queue.c
C_OBJ=$(C_SRC:.c=.o)  

C_SRC_1 = workers.c
C_OBJ_1 = $(C_SRC_1:.c=.o)

C_SOURCES= $(C_PROG) $(C_SRC) $(C_SRC_1)
C_OBJECTS=$(C_SOURCES:.c=.o)


all: task_server 

task_server: task_server.o $(C_OBJ) $(C_OBJ_1)
	$(CC) -g -pthread -o $@ $^

clean:
	-rm task_server $(C_OBJECTS)
