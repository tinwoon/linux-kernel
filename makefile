OBJECTS = led.o
SRCS = led.c
CCFLAGS = -g
TARGET = led

all : $(TARGET)

clean :
	rm -f $(TARGET) $(OBJECTS) core

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

$(OBJECTS) : $(SRCS)

