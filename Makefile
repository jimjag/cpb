TARGET = src/libcpb.a

SOURCES = \
src/cpb/misc.c \
src/cpb/decoder.c \
src/cpb/encoder.c \
src/cpb/encoder2.c 

OBJECTS = $(SOURCES:%.c=%.o)

CFLAGS = -std=c89 -fPIC -Wall -I./src/include -I./src/cpb

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(AR) -cr $@ $^

check :
	$(MAKE) -C ./test check

clean :
	rm -f $(TARGET) $(OBJECTS)

