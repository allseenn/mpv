TARGET=mpvg
DIR=/usr/local/sbin

ARGS=`pkg-config --cflags --libs gtk+-3.0`

all: $(TARGET)

$(TARGET): $(TARGET).o 
	gcc -o $(TARGET) $(TARGET).o $(ARGS)

$(TARGET).o: $(TARGET).c  
	gcc -c $(TARGET).c -o $(TARGET).o $(ARGS)

install:
	cp $(TARGET) $(DIR)
	cp $(TARGET).css $(DIR)

clean:
	rm -f $(TARGET).o $(TARGET)

run:
	./$(TARGET)

remove:
	rm -f $(DIR)/$(TARGET)
	rm -f $(DIR)/$(TARGET).css