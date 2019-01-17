#include "networking.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int check(char * u,char * p){
  char str[100];
  FILE * file;
  file = fopen("users.txt", "r");
  int i = 0;
  strtok(p,"\n");
  if (file) {
      while (fscanf(file, "%s", str)!=EOF){
	printf("line %d %s\n",i,str);
        if(!strcmp(str,u) && i % 2 == 0){
            fscanf(file,"%s",str);
            i++;
	    if(!strcmp(str,p)){	
	      return 0;
              } 
	} 
	i++;            
      }		     
      fclose(file);			      
      return 1;					      
  }
					     
}
int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  fd_set read_fds;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );


  int users = open("users.txt", O_RDWR | O_APPEND);
  if(users < 0){
    printf("%s\n", strerror(errno));
  }

  char username[100];
  char password[100];
  char answer[100];
  char other_person[100];
  int acc = 0;
  printf("Do you have an account? [y/n] ");
  fflush(stdout);
  fgets(answer, sizeof(answer), stdin);
  write(server_socket, answer, 1000);
  if(answer[0] == 'y'){

    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    strtok(username,"\n");
    write(server_socket, username, sizeof(username));
    printf("Enter your password: ");
    strtok(password,"\n");
    fgets(password, sizeof(password), stdin);
    write(server_socket, password, sizeof(password));

    char *user = calloc(1, 200);
    if(check(username,password) == 0){
        printf("Logging in\n");
	acc = 1;
       }
    else{
	printf("Incorrect Username/Password\n");
    } 
  } else if(answer[0] == 'n'){
    printf("Enter new username: ");
    fgets(username, sizeof(username), stdin);
     strtok(username,"\n");
    write(server_socket, username, strlen(username));
    printf("Enter new password: ");
    fgets(password, sizeof(password), stdin);
     strtok(password,"\n");
    write(server_socket, password, strlen(password));
  }

  close(users);
  if(acc){
  printf("Who do you want to chat with? ");
  fgets(other_person, sizeof(other_person), stdin);
  printf("Other person: %s\n", other_person);
  strtok(other_person,"\n");
  write(server_socket,other_person,300);
  char* result = malloc(50);
  read(server_socket, result, 50);
  printf("%s\n", result);
  fflush(stdout);
}








  while (1) {

    printf("enter data: ");
    //the above printf does not have \n
    //flush the buffer to immediately print
    fflush(stdout);

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(server_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(server_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, sizeof(buffer), stdin);
      *strchr(buffer, '\n') = 0;
      write(server_socket, buffer, sizeof(buffer));
      read(server_socket, buffer, sizeof(buffer));
      printf("received: [%s]\n", buffer);
    }//end stdin select

    //currently the server is not set up to
    //send messages to all the clients, but
    //this would allow for broadcast messages
    if (FD_ISSET(server_socket, &read_fds)) {
      read(server_socket, buffer, sizeof(buffer));
      printf("[SERVER BROADCAST] [%s]\n", buffer);
      printf("enter data: ");
      //the above printf does not have \n
      //flush the buffer to immediately print
      fflush(stdout);
    }//end socket select

  }//end loop
}
