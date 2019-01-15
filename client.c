#include <unistd.h>
#include <fcntl.h>


#include "networking.h"

int main(int argc, char **argv) {

	int server_socket;
	char buffer[BUFFER_SIZE];

	if (argc == 2)
		server_socket = client_setup( argv[1]);
	else
		server_socket = client_setup( TEST_IP );

	int users = open("users.txt", O_RDWR | O_APPEND);
	printf("%d\n", users);
	if(users < 0){
		printf("%s\n", strerror(errno));
	}

	char username[100];
	char password[100];
	char answer[100];
	char other_person[100];

	printf("Do you have an account? [y/n]\n");
	fgets(answer, sizeof(answer), stdin);
	if(answer[0] == 'y'){

		printf("Enter your username: \n");
		fgets(username, sizeof(username), stdin);
		strtok(username,"\n");
		write(server_socket, username, sizeof(username);
			printf("Enter your password: \n");
			strtok(password,"\n");
			fgets(password, sizeof(password), stdin);
			write(server_socket, password, sizeof(password));


			char *user = calloc(1, 200);
			int found = 0;
			read(users, user, 200);
			printf("%s\n", user);
			printf("%ld\n", strlen(user));
	} else if(answer[0] == 'n'){
			printf("Enter new username: ");
			fgets(username, sizeof(username), stdin);
			strtok(username,"\n");
			int a = write(users, username, sizeof(username));
			if(a < 0) {
				printf("%s\n", strerror(errno));
			}
			printf("Enter new password: ");
			fgets(password, sizeof(password), stdin);
			strtok(password,"\n");
			int b = write(users, password, sizeof(password));
			if(b < 0) {
				printf("%s\n", strerror(errno));
			}
		}

		close(users);

		printf("Who do you want to chat with? ");
		fgets(other_person, sizeof(other_person), stdin);
		printf("Other person: %s\n", other_person);
		strtok(other_person,"\n");
		char *chatroom = malloc(300);
		int chat_file;
		sprintf(chatroom, "%s_%s.txt", username, other_person);
		chat_file = open(chatroom, O_RDWR | O_APPEND);
		if(chat_file < 0){
			sprintf(chatroom, "%s_%s.txt", other_person, username);
			chat_file = open(chatroom, O_RDWR | O_APPEND);
			if(chat_file < 0){
				chat_file = open(chatroom, O_CREAT | O_RDWR | O_APPEND, 0644);
				printf("Created new chat: %s\n", chatroom);
			}
			else{
				printf("Found file\n");
			}
		}
		else{
			printf("Found file\n");
		}
  /* while (1) {
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
  } */
	}
