#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

const char *serv[3];
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	if (argv[1] == NULL){
		cout << "ERROR: No input file.\n";
		cout << "Usage: " << argv[0] << " <command_file>\n";
		return 1;
	}
	
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
	int total = 0, fail = 0;
	
	serv[0] = "master";
	serv[1] = "slave1";
	serv[2] = "slave2";
	
	fstream commandfile;
	string line;
	
	// port number: 51717
    portno = 51717;
	
	commandfile.open(argv[1]);
	if( !commandfile.is_open()){
		cout << "ERROR: cannot open the file.\n";
		return 1;
	}
	
	while(getline(commandfile, line)) {
		
		string cmd;
		istringstream iss(line);
		
		iss >> cmd;
		total++;
		bzero(buffer,256);
		// create a socket
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
			error("ERROR opening socket");
		// server name: master, slave1
		server = gethostbyname(serv[total % 2]);
		
		if (server == NULL) {
			fprintf(stderr,"ERROR, no such host\n");
			exit(1);
		}
		// set to 0
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, 
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
		serv_addr.sin_port = htons(portno);
		// connect to the server
		if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0 ) 
			error("ERROR connecting");
		
		if ( (cmd.compare("load") == 0) || 
				(cmd.compare("save") == 0) ||
				(cmd.compare("init") == 0) ||
				(cmd.compare("remit") == 0) ){
			
			// copy to buffer
			strcpy(buffer, line.c_str());
			// cout << buffer << endl;
			// sending commands
			n = write(sockfd, buffer, strlen(buffer));
			if (n < 0)
				error("ERROR writing to socket");
			// cout << "write completed\n";
			bzero(buffer, 256);
			// block until receiving server's responses
			n = read(sockfd, buffer, 255);
			if (n < 0)
				error("ERROR reading from socket");
			// printf("Response: %s\n", buffer);
			if ( strcmp(buffer, "command fail.") == 0 )
				fail++;
			
		}else if( cmd.compare("end") == 0 )
		{
			if ( write(sockfd, cmd.c_str(), 3) < 0 )
				error("ERROR writing to socket");
			bzero(buffer, 256);
			cout << "end here.\n";
			cout << "\n======================================\n\n";
			while ( read(sockfd, buffer, 255) > 0) {
				
				if( strcmp(buffer, "over") == 0)
					break;
				cout << buffer << endl;
				if ( write(sockfd, "get", 3) < 0)
					error("ERROR writing to socket.");
				bzero(buffer, 256);
			}
			break;
		}else {
			cout << "not a command.\n";
			fail++;
		}
		close(sockfd);
	}
	
	cout << "success rate: " << (total-fail) << "/" << total << endl ;
	
	commandfile.close();
    
	
    return 0;
}