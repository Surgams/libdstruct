CC =cc
CFLAGS=--std=c11 -O3 -Wall -Werror -Wextra 
CFLAGS_DEBUG=--std=c11 -g3 -Wall -Werror -Wextra 

RM = rm -f

PROJECT_O=libdstruct.o
PROJECT_D=libdstruct.so
PROJECT_S=libdstruct.a
OBJECTS=ds_hash.o ds_list.o	ds_queue.o ds_stack.o ds_set.o ds_array_stack.o ds_array_queue.o
HEADERS=ds_*.h  
SOURCES=ds_hash.c ds_list.c	ds_queue.c ds_stack.c ds_set.c ds_array_stack.c ds_array_queue.c

VERSION=1.1.0
OUTPUT=$(PROJECT_D)
OUTPUT_STATIC=$(PROJECT_S)
LIB_DIR=/usr/local/lib
INCLUDE_DIR=/usr/local/include

-include Makefile.depend

all: build 

build: depend $(PROJECT_O) $(PROJECT_D) $(PROJECT_S)

debug:
	$(CC) -fPIC -c $(SOURCES) $(CFLAGS_DEBUG)

$(PROJECT_O):
	$(CC) -fPIC -c $(SOURCES) $(CFLAGS)

$(PROJECT_D):
	$(CC) -shared -o $(PROJECT_D).$(VERSION) $(OBJECTS) 

$(PROJECT_S):
	ar rcs $(PROJECT_S) $(OBJECTS) 
	ranlib $(PROJECT_S)


clean:
	$(RM) *.so *.so.* *.o *.a Makefile.depend 

depend:
	cc -E -MM $(SOURCES) > Makefile.depend

install: all
	install $(OUTPUT).$(VERSION) $(LIB_DIR)
	ln -sf $(LIB_DIR)/$(OUTPUT).$(VERSION) $(LIB_DIR)/$(OUTPUT)
	install $(OUTPUT_STATIC) $(LIB_DIR)
	install -m644 $(HEADERS) $(INCLUDE_DIR)


deinstall:
	$(RM) $(LIB_DIR)$(OUTPUT).$(VERSION)
	$(RM) $(LIB_DIR)$(OUTPUT)
	$(RM) $(LIB_DIR)/$(OUTPUT_STATIC) 
	$(RM) $(INCLUDE_DIR)/$(HEADERS)
