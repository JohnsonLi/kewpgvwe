#include "networking.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

void process(char *s);
void subserver(int from_client);

int check_account(char * u,char * p){
  char str[100];
  FILE * file;
  file = fopen("users.txt", "r");
  int i = 0;
  strtok(p,"\n");
  strtok(u,"\n");
  if (file) {
    while (fscanf(file, "%s", str)!=EOF){
      //printf("line %d %s\n",i,str);
      if(!strcmp(str,u) && i % 2 == 0){
        printf("true");
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

int main() {

  int listen_socket;
  int client_socket;
  int f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  //set of file descriptors to read from
  fd_set read_fds;

  listen_socket = server_setup();
  client_socket = server_connect(listen_socket);
  f = fork();
  if (f==0){
    int acc = 0;
    char username[100];
    char password[100];
    char answer[100];
    char other_person[100];
    char login_message[100];
    char register_message[100];
    memset(username, 0, 100);
    memset(password, 0, 100);
    memset(answer, 0, 100);
    memset(other_person, 0, 100);
    memset(login_message, 0, 100);
    memset(register_message, 0, 100);

    printf("%s\n", strerror(errno));

    read(client_socket, answer, 1000);
    printf("[Server] answer: %s\n", answer);
    read(client_socket, username, 100);
    printf("[Server] username: %s\n", username);
    read(client_socket,password,100);
    printf("[Server] password: %s\n", password);
    if(answer[0] == 'y'){
      if(check_account(username,password)){
        printf("Correct/n");
      }
      else{
        printf("Incorrect\n");
      }
    }
    else{
      int file = open("users.txt",O_RDWR | O_APPEND);
      int a = write(file,username,strlen(username));
      if( a <0 ){
        printf("%s \n",strerror(errno));
      }
      int b = write(file,password,strlen(password));
      if(b< 0 ){
        printf("%s\n",strerror(errno));
      }
      read(client_socket, other_person,100);
      int chat_file;
      char* result = malloc(50);
      char *chatroom = malloc(300);
      sprintf(chatroom, "%s_%s.txt", username, other_person);
      chat_file = open(chatroom, O_RDWR | O_APPEND);
      if(chat_file < 0){
        sprintf(chatroom, "%s_%s.txt", other_person, username);
        chat_file = open(chatroom, O_RDWR | O_APPEND);
        if(chat_file < 0){
          chat_file = open(chatroom, O_CREAT | O_RDWR | O_APPEND, 0644);
          if (chat_file>0){
            strcpy(result,"[Server] Created new chat\n");
          }
          else{
            strcpy(result,"[Server] Failed\n");
          }
        }
        else{
          strcpy(result,"[Server] Found chat\n");
        }
      }
      else{
        strcpy(result,"[Server] Found chat\n");
      }
      printf("%s %s\n", result, chatroom);
      write(client_socket, result, 50);

      while (1) {

      //select() modifies read_fds
      //we must reset it at each iteration
      FD_ZERO(&read_fds); //0 out fd set
      FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
      FD_SET(chat_file, &read_fds); //add socket to fd set

      //select will block until either fd is ready
      select(chat_file + 1, &read_fds, NULL, NULL, NULL);
      printf("hello\n");

      //if listen_socket triggered select
      if (FD_ISSET(chat_file, &read_fds)) {
        printf("%ld\n", read(chat_file,buffer,10));
      }//end listen_socket select

      //if stdin triggered select
      if (FD_ISSET(STDIN_FILENO, &read_fds)) {
  //if you don't read from stdin, it will continue to trigger select()
        fgets(buffer, sizeof(buffer), stdin);
        printf("[server] subserver count: %d\n", subserver_count);
      }//end stdin select
    }
  }
}
else{
  close(client_socket);
}
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  //for testing client select statement
  strncpy(buffer, "hello client", sizeof(buffer));
  write(client_socket, buffer, sizeof(buffer));

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    //process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
}