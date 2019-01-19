#include "networking.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];
  int red;
  fd_set read_fds;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  int users = open("users.txt", O_RDWR | O_APPEND);
  if(users < 0){
    printf("%s\n", strerror(errno));
  }

  char* username = calloc(1,50);
  char* password = calloc(1,50);
  char* answer = calloc(1,50);
  char* other_person = calloc(1,50);
  int acc = 0;
  printf("Do you have an account? [y/n] ");
  fflush(stdout);
  fgets(answer, 50, stdin);
  write(server_socket, answer, 1000);
  if(answer[0] == 'y'){
    printf("Enter your username: ");
    fgets(username, 100, stdin);
    // strtok(username,"\n");
    write(server_socket, username, strlen(username));
    printf("Enter your password: ");
    fgets(password, 100, stdin);
    //strtok(password,"\n");
    write(server_socket, password, strlen(password));
  }
  else{
    printf("Enter new username: ");
    fgets(username, 100, stdin);
    strtok(username,"\n");
    write(server_socket, username, strlen(username));
    printf("Enter new password: ");
    fgets(password, 100, stdin);
    strtok(password,"\n");
    write(server_socket, password, strlen(password));
  }

  close(users);
  printf("Who do you want to chat with? ");
  fgets(other_person, 100, stdin);
  printf("Other person: %s\n", other_person);
  strtok(other_person,"\n");
  write(server_socket,other_person,300);
  char* result = malloc(50);
  read(server_socket, result, 50);
  printf("%s\n", result);
  fflush(stdout);


  while (1) {
    char* buffer = calloc(1,BUFFER_SIZE);
    char* text = calloc(1,BUFFER_SIZE+100);
    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(server_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(server_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, BUFFER_SIZE, stdin);
      // *strchr(buffer, '\n') = 0;
      sprintf(text,"[%s] %s", username, buffer);
      write(server_socket, text, strlen(text));
    }//end stdin select

    //currently the server is not set up to
    //send messages to all the clients, but
    //this would allow for broadcast messages
    if (FD_ISSET(server_socket, &read_fds)) {
      read(server_socket, buffer, BUFFER_SIZE);
      printf("%s\n", buffer);
      //the above printf does not have \n
      //flush the buffer to immediately print
      fflush(stdout);
    }//end socket select
    free(buffer);
}//end loop
}
