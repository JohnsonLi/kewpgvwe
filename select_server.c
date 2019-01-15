#include "networking.h"
#include <errno.h>

void process(char *s);
void subserver(int from_client);

int main() {

  int listen_socket;
  int client_socket;
  int f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  //set of file descriptors to read from
  fd_set read_fds;

  listen_socket = server_setup();

  char username[100];
  char password[100];
  char answer[100];
  char other_person[100];
  char login_message[100];
  char register_message[100];
  client_socket = server_connect(listen_socket);
  printf("%s\n", strerror(errno));
  
  read(client_socket, answer, 1000);
  printf("answer: %s\n", answer);
  read(client_socket, username, 100);
  printf("username: %s\n", username);
  read(client_socket,password,100);
  printf("password: %s\n", password);
  if(answer[0] == 'y'){
    // if(authenticate(username,password)){
    //   login_message = "Logged in successfully";
    // }
    // else{
    //   login_message = "Username/password incorrect";
    // }
    // write(client_socket, login_message, strlen(login_message));
  }
  else{
  //   if(register_user(username,password)){
  //     register_message = "Logged in successfully";
  //   }
  //   else{
  //     register_message = "Username/password incorrect";
  //   }
  //   write(client_socket, register_message, strlen(register_message));
  }

  

  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
     client_socket = server_connect(listen_socket);

     f = fork();
     if (f == 0)
       subserver(client_socket);
     else {
       subserver_count++;
       close(client_socket);
     }
    }//end listen_socket select

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, sizeof(buffer), stdin);
      printf("[server] subserver count: %d\n", subserver_count);
    }//end stdin select
  }
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  //for testing client select statement
  strncpy(buffer, "hello client", sizeof(buffer));
  write(client_socket, buffer, sizeof(buffer));

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
}

void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
}
