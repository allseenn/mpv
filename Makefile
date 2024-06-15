TARGET=mpvg

ARGS=`pkg-config --cflags --libs gtk+-3.0`

all: $(TARGET).exe
	./$(TARGET).exe

$(TARGET).exe: $(TARGET).o 
	gcc -o $(TARGET).exe $(TARGET).o $(ARGS)

$(TARGET).o: $(TARGET).c  
	gcc -c $(TARGET).c -o $(TARGET).o $(ARGS)

clean:
	rm -f $(TARGET).o $(TARGET).exe 