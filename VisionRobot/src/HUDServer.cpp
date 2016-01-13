/*
 * HUDServer.cpp
 *
 *  Created on: Mar 25, 2015
 *      Author: LAHS
 */

#include <HUDServer.h>

HUDServer::HUDServer(int portno, Servo* serv1, Servo* serv2) {
	// TODO Auto-generated constructor stub
	std::cout << "before";
	std::cout << "Test";
	t = std::thread(send, this, portno);

}
void HUDServer::send(void* v, int port) {
	((HUDServer*) v)->callSend(port);

}

void HUDServer::callSend(int portno) {
	while (true) {
		int sockfd, newsockfd, n;
		std::cout << "initialized sockfd, new sockfd, n" << std::endl;
		socklen_t clilen;
		std::cout << "initialized clilen" << std::endl;
		struct sockaddr_in serv_addr, cli_addr;
		std::cout << "sockaddr" << std::endl;
		sockfd = socket(AF_INET, SOCK_STREAM, 0); //create socket
		std::cout << "create socket" << std::endl;
		if (sockfd < 0) //if any errors occur
			std::cout << "ERROR opening socket" << std::endl;
		serv_addr.sin_family = AF_INET; //setting the family of the socket
		std::cout << "setting family" << std::endl;
		serv_addr.sin_port = htons(portno); //setting the port number
		std::cout << "setting port number" << std::endl;
		serv_addr.sin_addr.s_addr = INADDR_ANY; //making it accept connections on any ip that points to it.
		if (!bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
			std::cout << "ERROR on binding" << std::endl;
		listen(sockfd, 5); //actually listening on the socket now

		std::cout << "listening" << std::endl;
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //accept client connection
		std::cout << "listening" << std::endl;

		if (newsockfd < 0) //error checking
			std::cout << "ERROR on accept" << std::endl;
		std::cout << "listening" << std::endl;
		while (true) { //infinite loops :D
//			int err;
//			int sizeerr = sizeof(err);
//			int ret = getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &sizeerr);
//			if (ret) {
//				std::cout << "HUD disconnected\n";
//				break;
//			}
			std::String number;
			newsockfd >> number;

			std::stringstream ss(number);
			std::istream_iterator<std::string> begin(ss);
			std::istream_iterator<std::string> end;
			std::vector<std::string> vstrings(begin, end);
			std::copy(vstrings.begin(), vstrings.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

			//int i = std::stoi(s);

			int coord [2];
			if(n < 0)
				break;

			Wait(1);
		}
		close(newsockfd);
		close(sockfd);
	}
}

HUDServer::~HUDServer() {
	// TODO Auto-generated destructor stub
}