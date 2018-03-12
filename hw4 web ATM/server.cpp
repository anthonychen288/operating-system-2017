/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <string>
#include <locale>
#include <vector>
#include <algorithm>

#include <hiredis/hiredis.h>

using namespace std;

string line;
redisContext *c;
redisReply *reply;
const char *hostname = "master";
const int port = 6379;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
string stolower(string s){
	locale loc;
	string slow;
	
	for(std::string::size_type i=0;i<s.length();i++)
	{
		slow = slow + std::tolower(s[i], loc);
	}
	return slow;
}
bool verifyAccount(string acc, int mula) {
	// check account name EXIST acc1
	reply = (redisReply *)redisCommand(c, "EXISTS %s", acc.c_str());
	if( reply->integer == 0) {
		cout << "account name: "<< acc << " dose not exits.\n";
		freeReplyObject(reply);
		return false;
	}
	// check mula
	if ( mula < 0 ) {
		cout << "not enough money:" << acc << endl;
		freeReplyObject(reply);
		return false;
	}
	freeReplyObject(reply);
	return true;
}
bool do_command(string line)
{
	string cmd, acc1, acc2;
	int mula;
	istringstream iss(line);
	
	iss >> cmd;
	
	if ( cmd.compare("load") == 0){
		iss >> acc1 >> mula;
		acc1 = stolower(acc1);
		
		if (verifyAccount(acc1, mula)) {
			// check deposit EXIST acc1
			reply = (redisReply *)redisCommand(c, "GET %s", acc1.c_str());
			if( atoi(reply->str) < mula ) {
				cout << "not enough money: " << acc1 << endl;
				freeReplyObject(reply);
				return false;
			}
			freeReplyObject(reply);
			// DECRBY acc1 "mula"
			reply = (redisReply *)redisCommand(c, "DECRBY %s %d", acc1.c_str(), mula);
			cout << "deposit: " << reply->integer << endl;
			freeReplyObject(reply);
		}else
			return false;
	}else if( cmd.compare("save") == 0){
		iss >> acc1 >> mula;
		acc1 = stolower(acc1);
		if ( verifyAccount(acc1, mula) ) {
			// INCRBY acc1 "mula"
			reply = (redisReply *)redisCommand(c, "INCRBY %s %d", acc1.c_str(), mula);
			cout << "deposit: " << reply->integer << endl;
			freeReplyObject(reply);
		}else
			return false;
	}else if( cmd.compare("init") == 0){
		iss >> acc1 >> mula;
		acc1 = stolower(acc1);
		// check deposit
		if ( mula < 0) {
			cout << "not enough money: " << acc1 << endl;
			return false;
		}
		// SETNX acc1 "mula"
		reply = (redisReply *)redisCommand(c, "SETNX %s %d", acc1.c_str(), mula);
		if ( reply->integer == 0) {
			cout << "account name:" << acc1 << " already exits.\n";
			freeReplyObject(reply);
			return false;
		}
		freeReplyObject(reply);
	}else if( cmd.compare("remit") == 0 ) {
		iss >> acc1 >> acc2 >> mula;
		acc1 = stolower(acc1);
		acc2 = stolower(acc2);
		if ( verifyAccount(acc1, mula) && verifyAccount(acc2, mula) ) {
			// check acc1 & 2
			if (acc1.compare(acc2.c_str()) == 0)
				return false;
			// check acc1's deposit
			reply = (redisReply *)redisCommand(c, "GET %s", acc1.c_str());
			if( atoi(reply->str) < mula ) {
				cout << "not enough money: " << acc1 << endl;
				freeReplyObject(reply);
				return false;
			}
			freeReplyObject(reply);
			// DECRBY acc1 "mula"
			reply = (redisReply *)redisCommand(c, "decrby %s %d", acc1.c_str(), mula);
			cout << acc1 << ": " << reply->integer << endl;
			freeReplyObject(reply);
			// INCRBY acc2 "mula"
			reply = (redisReply *)redisCommand(c, "incrby %s %d", acc2.c_str(), mula);
			cout << acc2 << ": " << reply->integer << endl;
			freeReplyObject(reply);
		}else
			return false;
	}
	return true;
}
void showAccount(int sock) {
	// get all keys
	reply = (redisReply *) redisCommand(c, "keys *");
	int key_num = reply->elements;
	char s[100];
	
	vector<string> acc;
	for (int i=0;i < key_num;i++) {
		// cout << reply->element[i]->str << endl;
		acc.push_back(reply->element[i]->str);
	}
	freeReplyObject(reply);
	sort(acc.begin(), acc.end());
	redisReply *r_tmp;
	for (vector<string>::size_type i=0; i < acc.size(); i++) {
		// get key's value
		r_tmp = (redisReply *) redisCommand(c, "get %s", acc[i].c_str());
		bzero(s, 100);
		strcpy(s, acc[i].c_str());
		strcat(s, ":");
		strcat(s, r_tmp->str);
		// send key:value
		if ( write(sock, s, strlen(s)) < 0)
			error("ERROR writing to sockets."); 
		bzero(s, 100);
		if ( read(sock, s, 100) < 0)
			error("ERROR reading socket.");
		if ( strcmp(s, "get") != 0) {
			cout << "ERROR: send outputs failure.\n";
			break;
		}
		freeReplyObject(r_tmp);
	}
	if ( write(sock, "over", 4) < 0 )
		error("ERROR writing to socket");
	reply = (redisReply *) redisCommand(c, "flushdb");
	cout << "flush current DB: " << reply->str << endl;
	freeReplyObject(reply);
}

/************* DOSTUFF() *****************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
	int n;
    char buffer[256];
    
    bzero(buffer,256);
	c = redisConnect(hostname, port);
	
	if( c==NULL || c->err){
		if(c){
			cout << "Connection error: " << c->err << endl;
		}else
			cout << "Connection error: can't allocate redis context\n";
		redisFree(c);
		exit(1);
	}
	if (read(sock,buffer,255) < 0 )
		error("ERROR reading socket");
	
	if( strcmp(buffer, "end") == 0 ) {
		showAccount(sock); // GET info & FLUSHDB
		// break;
	}else {
		
		line.assign(buffer);
		cout << line << endl;
		if(do_command(line))
		{
			if ( write(sock,"command completed.",18) < 0)
				error("ERROR writing to socket");
		}else{
			if ( write(sock, "command fail.", 13) < 0 )
				error("ERROR writing to socket");
		}
		bzero(buffer, 256);
	}
    redisFree(c);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
	// create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
		error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
	// port number: 51717
    portno = 51717;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
	// bind the socket to the host server
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
	// listen for connections
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
	
    while (1) {
		// block until a client connects to the server
        newsockfd = accept(sockfd, 
				(struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
		
		dostuff(newsockfd);
		close(newsockfd);
    } /* end of while */
    
	close(sockfd);
    return 0; /* we never get here */
}
