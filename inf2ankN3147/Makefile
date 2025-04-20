.PHONY: all clean debug

APP=inf2ankN3147
CFLAGS=-Wall -Wextra -Werror

all: $(APP)

$(APP): $(APP).c $(APP).S
	gcc -o $(APP)_c $(CFLAGS) $(APP).c
	nasm -felf64 $(APP).S
	ld -o $(APP)_n $(APP).o

clean:
	rm -rf $(APP)_c $(APP)_n $(APP).o

