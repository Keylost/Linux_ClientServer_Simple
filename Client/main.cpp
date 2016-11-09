#include <stdio.h>

#include <stdlib.h>
#include <stdint.h>

#include <memory.h>
//#include <sys/types.h> 
//#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> //gethostbyname()

/*
 * изначально сервер принимает только два числа и умеет их складывать.
 * задачи:
 * 1. сделать так, чтобы сервер мог принять и обработать выражения из двух операндов и одного оператора (+, - , *, /)
 * 2. произвольное число операндов и операторов и обработка приоритетов опреций
 * 3. Добавить таймауты на подключение к серверу
 */


using namespace std;

int connect(const char* host, int port);
bool get_data(void *dst, int socket, size_t size);
bool send_data(void *src,int socket,size_t size);

int main(int argc, char **argv)
{
	int serverSockfd = -1;
	serverSockfd = connect("127.0.0.1",1212);
	
	/*
	int operand1 = 0, operand2 = 0;
	char _operator = '0';
	if(get_data(&operand1, clientSockfd, sizeof(int) &&
				get_data(&_operator, clientSockfd, sizeof(char) &&
				get_data(&operand2, clientSockfd, sizeof(int) );
	*/
	
	while(true)
	{
		int32_t operands[2];
		int32_t answer = 0;
		printf("Enter A: "); scanf("%d",operands);
		printf("Enter B: "); scanf("%d",operands+1);
		
		if(send_data(&operands, serverSockfd, sizeof(int32_t)*2))
		{
			if(get_data(&answer, serverSockfd, sizeof(int32_t)))
			{
				printf("A+B=%d\n",answer);				
			}
			else
			{
				perror("ERROR reading data");
				exit(EXIT_FAILURE);					
			}
		}
		else
		{
			perror("ERROR writing data");
			exit(EXIT_FAILURE);	
		}
		
		printf("One more time? (Y/n) >> ");
		char ans = 'y';
		scanf("%s",&ans);
		if(ans == 'N' || ans == 'n')
		{
			exit(EXIT_SUCCESS);
		}
	}
	return 0;
}  


/*
 * подключение к серверу
 */
int connect(const char* host, int port)
{
	struct sockaddr_in serv_addr;
	struct hostent *server;		
	int sockfd = -1;
	
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("ERROR opening socket");
		exit(EXIT_FAILURE);
	}
	
	if ( (server = gethostbyname(host)) == NULL)
	{
		perror("ERROR, no such host");
		exit(EXIT_FAILURE);
	}

	memset((char *) &serv_addr,0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *)&serv_addr.sin_addr.s_addr,
			(char *)server->h_addr,
			server->h_length);
	serv_addr.sin_port = htons(port);
	
	if (::connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR connecting to server");
		exit(EXIT_FAILURE);
	}
	
	return sockfd;
}

/*
 * Функция get_data() отвечает за прием данных из @dst размера @size байт с использованием сокета @socket
 */
bool get_data(void *dst, int socket, size_t size)
{
	size_t i=0;
	int bytes =0;
	for (i = 0; i < size; i += bytes)
	{
		if ((bytes = recv(socket, (char *)dst+i, size-i, 0)) == -1)
		{
			return false;
		}
	}
	return true;
}

/*
 * Функция send_data() отвечает за отправку данных из @src размера @size байт с использованием сокета @socket
 */
bool send_data(void *src,int socket,size_t size)
{
	int bytes = 0;
	size_t i = 0;
	for (i = 0; i < size; i += bytes) 
	{
		if ((bytes = send(socket, ((uint8_t *)src)+i, size-i, MSG_NOSIGNAL)) <= 0) 
		{
			return false;
		}
	}
	return true;
}
