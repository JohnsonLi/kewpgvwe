#include "networking.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int check_account(char * u,char * p){
 char str[100];
 FILE * file;
 file = fopen("users.txt", "r");
 int i = 0;
 strtok(p,"\n");
 strtok(u,"\n");
 if (file) {
  while (fscanf(file, "%s", str)!=EOF){
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

int print_file(char* filename, int client_socket){
 int fd = open(filename, O_RDONLY);
 char* buffer = malloc(100);
 printf("%ld\n", sizeof(buffer));
 int a;
 char* fence = "==========================================";
 write(client_socket,fence, strlen(fence));
 while(a=read(fd, buffer, 100)){
  printf("%s\n", buffer);
  write(client_socket, buffer, a);
 }
 close(fd);
}

int main() {
 struct stat file_stat;
 int listen_socket;
 int client_socket;
 int f;
 int subserver_count = 0;
 char buffer[BUFFER_SIZE];
 int last_modified = 0;
  //set of file descriptors to read from
 fd_set read_fds;

 listen_socket = server_setup();
 while(1){
  client_socket = server_connect(listen_socket);
  f = fork();
  if (f==0){
   int acc = 0;
   int err;
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
      err = stat(chatroom, &file_stat);
      last_modified = file_stat.st_mtime;
      strcpy(result,"[Server] Found chat\n");
     }
    }
    else{
     err = stat(chatroom, &file_stat);
     last_modified = file_stat.st_mtime;
     strcpy(result,"[Server] Found chat\n");
    }
    printf("%s %s\n", result, chatroom);
    write(client_socket, result, 50);
    int in;
    char* buffer = calloc(1,100);




     
    while (1) {
     read(client_socket, buffer, 100);
     while (strlen(buffer)){
      write(chat_file, buffer, strlen(buffer));
      print_file(chatroom,client_socket);
      read(client_socket, buffer, 100);
     }
    }
   }
  }
  else{
   close(client_socket);
  }
 }
}


