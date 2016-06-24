.PHONY: all hello clean

PTHREAD_DIR = ./prebuilt-dll-2-9-1-release
#PTHREAD_DIR = ./prebuilt-dll-2-8-0-release
PTHREAD_INC_DIR = $(PTHREAD_DIR)/include
PTHREAD_LIB_DIR = $(PTHREAD_DIR)/lib/x86
#PTHREAD_LIB_DIR = $(PTHREAD_DIR)/lib

CFLAGS  += -I$(PTHREAD_INC_DIR) 
LDFLAGS += -L$(PTHREAD_LIB_DIR)
#LDLIBS  += $(PTHREAD_LIB_DIR)/pthreadVCE2.lib
LDLIBS  += -static
LDLIBS  += -lpthread

EXE = .exe

TARGETS = join stack_mgr mutex condition

all: $(TARGETS)

join: join.c
	i586-mingw32msvc-gcc  $^ -o $@$(EXE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) 
	gcc  $^ -o $@ $(LDLIBS) 

stack_mgr: stack_mgr.c
	i586-mingw32msvc-gcc  $^ -o $@$(EXE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) 
	gcc  $^ -o $@ $(LDLIBS) 

mutex: mutex.c
	i586-mingw32msvc-gcc  $^ -o $@$(EXE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) 
	gcc  $^ -o $@ $(LDLIBS) 

condition: condition.c
	i586-mingw32msvc-gcc  $^ -o $@$(EXE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) 
	gcc  $^ -o $@ $(LDLIBS) 


clean:
	-rm -rf $(TARGETS) *.exe *.o
