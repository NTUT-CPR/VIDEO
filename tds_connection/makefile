CC = g++
CFLAG = -std=c++14 -g3 -Wall
CGTESTLIB = -lgtest -lpthread -lcurl
all: tds_server tds_client utTDSConnection

tds_server: mainTDSServer.o tds_connection.o tds_server.o tds_client.o
	${CC} ${CFLAG} -o tds_server mainTDSServer.o tds_connection.o tds_server.o tds_client.o ${CGTESTLIB}
tds_client: mainTDSClient.o tds_connection.o tds_client.o
	${CC} ${CFLAG} -o tds_client mainTDSClient.o tds_connection.o tds_client.o ${CGTESTLIB}
utTDSConnection: mainTDSConnection.o tds_connection.o
	${CC} ${CFLAG} -o utTDSConnection mainTDSConnection.o tds_connection.o tds_server.o tds_client.o  ${CGTESTLIB}
mainTDSServer.o: mainTDSServer.cpp
	${CC} ${CFLAG} -c mainTDSServer.cpp
mainTDSClient.o: mainTDSClient.cpp
	${CC} ${CFLAG} -c mainTDSClient.cpp
mainTDSConnection.o: utTDSConnection.h mainTDSConnection.cpp
	${CC} ${CFLAG} -c mainTDSConnection.cpp
tds_server.o: tds_server.h tds_server.cpp
	${CC} ${CFLAG} -c tds_server.cpp
tds_client.o: tds_client.h tds_client.cpp
	${CC} ${CFLAG} -c tds_client.cpp
tds_connection.o: tds_connection.h tds_connection.cpp
	${CC} ${CFLAG} -c tds_connection.cpp
clean:
	rm tds_server tds_client utTDSConnection *.o
