/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
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
#include <vector>
#include <algorithm>
#include <hiredis/hiredis.h>

using namespace std;

const char* hostname = "master";
const int port = 6379;
redisContext *c;
redisReply *reply;

int main(int argc, char *argv[])
{
	c = redisConnect(hostname, port);
	if( c==NULL || c->err){
		if(c){
			cout << "Connection error: " << c->err << endl;
		}else
			cout << "Connection error: can't allocate redis context\n";
		redisFree(c);
		exit(1);
	}
	reply = (redisReply *) redisCommand(c, "setnx account2 1000");
	cout << reply->integer << endl;
	freeReplyObject(reply);
	
	reply = (redisReply *) redisCommand(c, "setnx account54 1000");
	cout << reply->integer << endl;
	freeReplyObject(reply);
	
	reply = (redisReply *) redisCommand(c, "setnx account2457 1000");
	cout << reply->integer << endl;
	freeReplyObject(reply);
	
	reply = (redisReply *) redisCommand(c, "setnx account58 2000");
	cout << reply->integer << endl;
	freeReplyObject(reply);
	
	reply = (redisReply *) redisCommand(c, "setnx account458 2000");
	cout << reply->integer << endl;
	freeReplyObject(reply);
	
	cout << "\n================================\n\n";
	
	reply = (redisReply *) redisCommand(c, "keys *");
	cout << "total " << reply->elements << " keys\n";
	
	int key_num = reply->elements;
	vector<string> acc;
	cout << "original:\n";
	for (int i=0;i < key_num;i++) {
		cout << reply->element[i]->str << endl;
		acc.push_back(reply->element[i]->str);
	}
	freeReplyObject(reply);
	std::sort(acc.begin(), acc.end());
	cout << "sorted:\n";
	for (vector<string>::size_type i=0; i < acc.size(); i++) {
		// get key's value
		redisReply *r_tmp;
		char s[100];
		// cout << "get " << reply->element[i] << " info\n";
		r_tmp = (redisReply *) redisCommand(c, "get %s", acc[i].c_str());
		bzero(s, 100);
		strcpy(s, acc[i].c_str());
		strcat(s, ":");
		strcat(s, r_tmp->str);
		cout << s << endl;
		bzero(s, 100);
		freeReplyObject(r_tmp);
	}
	
	
	reply = (redisReply *) redisCommand(c, "flushdb");
	cout << reply->str << endl;
	freeReplyObject(reply);
	
	cout << "get info command: finished\n" ;
    return 0;
}
