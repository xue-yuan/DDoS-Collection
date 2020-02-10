edit: main.o cldap.o http.o tcp.o
	gcc -o main main.o cldap_amplification.o http_flood.o tcp_conn_flood.o

main.o: main.c main.h
	gcc -c main.c

cldap.o: cldap_amplification.c cldap_amplification.h
	gcc -c cldap_amplification.c

http.o: http_flood.c http_flood.h
	gcc -c http_flood.c

tcp.o: tcp_conn_flood.c tcp_conn_flood.o
	gcc -c tcp_conn_flood.c

clean:
	rm -rf *.o main