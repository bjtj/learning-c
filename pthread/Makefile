all: basic consumer_producer


basic:
	gcc -o $@ basic.c -pthread


consumer_producer:
	gcc -o $@ consumer_producer.c queue.c -pthread

clean:
	rm -rf basic consumer_producer
