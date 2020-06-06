CC =cc
CFLAGS=-g3 -Wall -Werror -Wextra

RM = rm -f

PROJECT_O=libdstruct.o
OBJECTS=../src/ds_hash.o ../src/ds_list.o ../src/ds_queue.o ../src/ds_stack.o ../src/ds_set.o ../src/ds_array_stack.o ../src/ds_array_queue.o
OBJECTS_HASH=../src/ds_list.o ../src/ds_queue.o ../src/ds_stack.o ../src/ds_set.o ../src/ds_array_stack.o ../src/ds_array_queue.o
SOURCE_LOCATION=../src
EXT_LIB= -lcheck
MAKE=make
BINARY=libdstruct_test

LIB_DIR=/usr/local/lib
INCLUDE_DIR=/usr/local/include


all: build test_common test_list test_queue test_stack test_hash test_set test_array_stack test_array_queue

build: $(TEST_SOURCES) $(OBJECTS) $(PROJECT_O) 

$(PROJECT_O):
	cd $(SOURCE_LOCATION) && $(MAKE) debug

test_common:
	@echo ""; echo "************ Common Test Suit **********"
	$(CC)  ds_common_test.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(EXT_LIB)
	./$(BINARY)
	@echo ""; 

test_list:
	@echo ""; echo "************ List Test Suit **********"
	$(CC)  ds_list_test.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(EXT_LIB)
	$(info )
	./$(BINARY)
	@echo ""; 

test_queue:
	@echo ""; echo "************ Queue Test Suit **********"
	$(CC)  ds_queue_test.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(EXT_LIB)
	./$(BINARY)
	@echo ""; 

test_stack:
	@echo ""; echo "************ Stack Test Suit **********"
	$(CC)  ds_stack_test.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(EXT_LIB)
	./$(BINARY)
	@echo ""; 

test_hash:
	@echo ""; echo "************ Hash Test Suit **********"
	$(CC)  -D DEBUG ds_hash_test.c  $(OBJECTS_HASH) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(EXT_LIB)
	./$(BINARY)
	@echo ""; 

test_set:
	@echo ""; echo "************ Set Test Suit **********"
	$(CC)  ds_set_test.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(EXT_LIB)
	./$(BINARY)
	@echo ""; 

test_array_stack:
	@echo ""; echo "************ Array Stack Test Suit **********"
	$(CC)  ds_array_stack_test.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(EXT_LIB)
	./$(BINARY)
	@echo ""; 

test_array_queue:
	@echo ""; echo "************ Array Queue Test Suit **********"
	$(CC)  ds_array_queue_test.c  $(OBJECTS) -o $(BINARY)  $(CFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(EXT_LIB)
	./$(BINARY)
	@echo ""; 

clean:
	$(RM) $(BINARY) && cd $(SOURCE_LOCATION) && $(MAKE) clean

run:
	./$(BINARY)

mem_check:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./$(BINARY)
