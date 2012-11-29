
# Change cpp and linker flags here
INCLUDES=-I../src -I../aloe -I../src -I../interfaces 
CPPFLAGS=-D_GNU_SOURCE -D__USE_GNU -O0 -g3 -Wall -Wno-format-extra-args $(INCLUDES) -std=c99 -Winline -fPIC

LDFLAGS=-lrt -lm
LDFLAGS_A=-L../aloe/ $(LDFLAGS) -lswapi


#Add here more .c files required by the module DSP function
MODSRC=../src/modulename.c 

#Matlab root path
MATLAB_ROOT=/path/to/matlab
MATLAB_INC=-D_COMPILE_MEX_INCLUDE

#Comment the next line if you have matlab to avoid mexPrintf warnings
MATLAB_INC=



# DO NOT NEED TO MODIFY BELOW HERE

# Name of the module 
NAME=modulename

MATLAB_INCLUDE=$(MATLAB_ROOT)/extern/include
CC=gcc
LIBNAME=lib$(NAME).a
LIBNAME_A=lib$(NAME).so

SASRC=../standalone/test_main.c ../src/test_generate.c ../standalone/gnuplot_i.c 
SAOBJ=$(SASRC:.c=.o)
ALOESRC=../aloe/aloe.c

MODOBJ_M=$(MODSRC:.c=.om)
MODOBJ_A=$(MODSRC:.c=.oa)
MODOBJ=$(MODSRC:.c=.o)
ALOEOBJ=$(ALOESRC:.c=.o)

.PHONY: all clean distclean

all: $(SOURCES) $(NAME) matlab aloe

matlab: $(MODOBJ_M) $(COMOBJ)
	ar rcs $(LIBNAME) $(MODOBJ_M)

aloe: $(MODOBJ_A)  $(ALOEOBJ)
	$(CC) $(CPPFLAGS) -u _run_cycle -shared -o $(LIBNAME_A) $(ALOEOBJ) $(MODOBJ_A) $(LDFLAGS_A)

$(ALOEOBJ):
	$(CC) $(CPPFLAGS) -D_COMPILE_ALOE -c $(ALOESRC) -o $(ALOEOBJ)

$(MODOBJ_A):
	$(CC) $(CPPFLAGS) -D_COMPILE_ALOE -c $(MODSRC) -o $(MODOBJ_A)

$(MODOBJ_M):
	$(CC) $(CPPFLAGS) -I$(MATLAB_INCLUDE) $(MATLAB_INC) -D_COMPILE_MEX -c $(MODSRC) -o $(MODOBJ_M)

$(NAME): $(MODOBJ) $(SAOBJ)
	$(LINK.cc) $(MODOBJ) $(SAOBJ) $(LDFLAGS) -o $@

clean:
	@- $(RM) $(LIBNAME) $(NAME)
	@- $(RM) $(MODOBJ) $(MODOBJ_M) $(MODOBJ_A) $(SAOBJ) $(ALOEOBJ)
	
	
	
