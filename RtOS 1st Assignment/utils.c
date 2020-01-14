#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

extern inline char*  friendlyMessage(){
	return "Hello, i am Raspberry Pi W Zero. How are you doing today?";
}

typedef struct message{
	uint32_t sender;
	uint32_t receiver;
	unsigned long timestamp;
	char body[256];
	//maybe insert a parser here
}message;

void parser(char * textToParse,message * msg){
	char symbol [] ="_";
	
	textToParse = strtok(textToParse , symbol);
	msg->sender = atoi(textToParse);

	textToParse = strtok(NULL,symbol);
	msg->receiver = atoi(textToParse);
	
	textToParse = strtok(NULL,symbol);
	msg->timestamp = atol(textToParse);
	
	textToParse = strtok(NULL,symbol);
	strcpy(msg->body,textToParse);
	
}

void printer(message * message){
	printf("The sender is %d\n",message->sender);
	printf("The receiver is %d\n",message->receiver);
	printf("The timestamp is %lu\n",message->timestamp);
	printf("The body is %s\n",message->body);

}

int main(){
	message A;
	A.sender = 8054;
	A.receiver = 7861;
	A.timestamp = 1234567;
	strcpy(A.body, "Hello How are you doing");
	printf("%lu",sizeof(message));
	//printer(&A);
	char test[] = "152_545_1645543_LulxD xD kai allo xD";
	message B;
	parser(test , & B);
	printer(&B);
	printf("%s\n",friendlyMessage());
}

