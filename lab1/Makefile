CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic
TARGET  = myshell
SRCS    = myshell.c utility.c
HEADERS = myshell.h

# Цель по умолчанию
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Удалить скомпилированный бинарник
clean:
	rm -f $(TARGET)

.PHONY: clean
