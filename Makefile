
cc = g++
CFLAGS = -g

all:
	$(CC) $(CFLAGS) src/top.cpp src/subseek_dtw.cpp -I include -o top

.PHONY: clean
clean:
	rm top
