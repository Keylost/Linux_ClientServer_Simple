#include <stdio.h>
/*
 * изначально сервер принимает только два числа и умеет их складывать.
 * задачи:
 * 1. сделать так, чтобы сервер мог принять и обработать выражения из двух операндов и одного оператора (+, - , *, /)
 * 2. произвольное число операндов и операторов и обработка приоритетов опреций
 */


using namespace std;



int main(int argc, char **argv)
{
	int clientSockfd = -1;
	clientSockfd = waitForClient();
	
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
		if(get_data(&operands, clientSockfd, sizeof(int32_t)*2))
		{
			int32_t answer = operands[0] + operands[1];
			if(send_data(&answer, clientSockfd, sizeof(int32_t)))
			{
				perror("ERROR writing data");
				exit(EXIT_FAILURE);					
			}
		}
		else
		{
			perror("ERROR reading data");
			exit(EXIT_FAILURE);		
		}
	}
	return 0;
}  


/*
 * открыть сокет и ждать соединения
 */
int waitForClient()
{
	socklen_t clilen; //размер структуры адреса клиента
	int sockfd = -1; //дескриптор сокета
	int clientSockfd = -1; //дескриптор сокета
	struct sockaddr_in serv_addr, cli_addr; //структуры адресов
	
	int portno = 1212; //номер порта для прослушивания
	
	/*открытие сокета*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(EXIT_FAILURE);
	}
	
	
	//sockOptEnable(sockfd,SO_KEEPALIVE);
	//sockOptEnable(sockfd,SO_REUSEADDR);
	
	/* Заполнить память по адресу &serv_addr нулями */
    bzero((char *) &serv_addr, sizeof(serv_addr));
	
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; //слушать все доступные адреса
	serv_addr.sin_port = htons(portno); //слушать порт portno
	
	/* назначить соответствие адреса и сокета */
	while(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
    {
		sleep(1); 
	}

	/* ждать клиента */
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	clientSockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	
	return clientSockfd;
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
