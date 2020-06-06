CC =cc
CFLAGS=-g3 -Wall -Werror -Wextra

RM = rm -f

PROJECT_O=libdstruct.o
OBJECTS=../src/ds_hash.o ../src/ds_list.o ../src/ds_queue.o ../src/ds_stack.o ../src/ds_set.o ../src/ds_array_stack.o ../src/ds_array_queue.o
SOURCE_LOCATION=../src
MAKE=make
BINARY=libdstruct_sample

LIB_DIR=/usr/local/lib
INCLUDE_DIR=/usr/local/include

all: build list_sample queue_sample stack_sample hash_sample set_sample array_stack_sample array_queue_sample

build: $(OBJECTS) $(PROJECT_O) 

$(PROJECT_O):
	cd $(SOURCE_LOCATION) && $(MAKE) debug


list_sample:
	$(CC)  list_sample.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) 
	$(info )
	./$(BINARY)
	@echo ""; 

queue_sample:
	$(CC)  queue_sample.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) 
	./$(BINARY)
	@echo ""; 

stack_sample:
	$(CC)  stack_sample.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) 
	./$(BINARY)
	@echo ""; 

hash_sample:
	$(CC)  hash_sample.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) 
	./$(BINARY)
	@echo ""; 

set_sample:
	$(CC)  set_sample.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) 
	./$(BINARY)
	@echo ""; 

array_stack_sample:
	$(CC)  array_stack_sample.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) 
	./$(BINARY)
	@echo ""; 

array_queue_sample:
	$(CC)  array_queue_sample.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) 
	./$(BINARY)
	@echo ""; 

clean:
	$(RM) $(BINARY) && cd $(SOURCE_LOCATION) && $(MAKE) clean

run:
	./$(BINARY)

mem_check:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./$(BINARY)
