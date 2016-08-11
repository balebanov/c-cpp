#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main(){
	int listen_socket, result_addr, result_bind;
	struct addrinfo hints;
	struct addrinfo* addr = NULL;
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	result_addr = getaddrinfo("10.0.0.77", "8080", &hints, &addr);
	if(result_addr != 0) {
		cerr << "Ошибка getaddrinfo(): " << result_addr << endl;
		return 1;
	}
	
	listen_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if(listen_socket < 0){
		cerr << "Ошибка сокета" << endl;
		close(listen_socket);
		return 1;
	}
	
	result_bind = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);
	if(result_bind < 0){
		cerr << "Ошибка bind(): " << result_bind << endl;
		return 1;
	}
	
	if(listen(listen_socket, 10) < 0){
		cerr << "Ошибка listen()" << endl;
		close(listen_socket);
		return 1;
	}

	const int max_client_buffer_size = 1024;
	char buf[max_client_buffer_size];
	int client_socket = 0;
		
	for(;;){	
		client_socket = accept(listen_socket, NULL, NULL);
		if(client_socket < 0){
			cerr << "Ошибка клиентского сокета: " << client_socket << endl;
			break;
			return 1;
		}
		
		int recv_result = recv(client_socket, buf, max_client_buffer_size, 0);
		stringstream response;
		stringstream response_body;
		
		if(recv_result < 0) {
			cerr << "Ошибка recv(): " << recv_result << endl;
			break;
			return 1;
		} else if (recv_result == 0){
			cerr << "Данные не получены!" << endl;
			break;
			return 1;
		} else if (recv_result > 0){
			buf[recv_result] = '\0';
			
			responce_body << "<h1>Тест!</h1>";
				
			response << "HTTP/1.1 200 OK\r\n"
				<< "Version: HTTP/1.1\r\n"
				<< "Content-Type: text/html; charset=utf-8\r\n"
				<< "Content-Length: " << response_body.str().length()
				<< "\r\n\r\n"
				<< response_body.str();
				
			int send_result = send(client_socket, response.str().c_str(), response.str().length(), 0);
			if(send_result < 0){
				cerr << "Ошибка send(): " << send_result << endl;
				break;
				close(client_socket);
			}
		}
	}
	close(client_socket);
	close(listen_socket);				
	return 0;
}
