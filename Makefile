CC = gcc

CFLAGS = -Wall -g

SRC1 = dllsniffer.c
SRC2 = dllsniffer2.c
SRC3 = dllsniffer_pro.c

EXEC1 = dllsniffer
EXEC2 = dllsniffer2
EXEC3 = dllsniffer_pro

all: $(EXEC1) $(EXEC2) $(EXEC3)

$(EXEC1): $(SRC1)
	$(CC) $(CFLAGS) -o $@ $^

$(EXEC2): $(SRC2)
	$(CC) $(CFLAGS) -o $@ $^

$(EXEC3): $(SRC3)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXEC1) $(EXEC2) $(EXEC3)

.PHONY: all clean
