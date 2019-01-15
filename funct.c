#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int start(){
  
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
    printf("Enter your password: \n");
    strtok(password,"\n");
    fgets(password, sizeof(password), stdin);

  
    char *user = calloc(1, 200);
    int found = 0;
    read(users, user, 200);
    printf("%s\n", user);
    printf("%ld\n", strlen(user));
    /* while(read(users, user, 200) && !found){
      printf("%s\n", user);
    } */
  } else if(answer[0] == 'n'){
     printf("Enter new username: ");
     fgets(username, sizeof(username), stdin);
     strtok(username,"\n");
     char * use = calloc(100,sizeof(char));
     strcpy(use,username);
     int a = write(users, use, sizeof(username));
      write(users, '\n',1);
    free(use); 
     if(a < 0) {
        printf("%s\n", strerror(errno));
     }
     printf("Enter new password: ");
     fgets(password, sizeof(password), stdin);
     strtok(password,"\n");
     char * pass = calloc(100,sizeof(char));
     strcpy(pass,password);
     int b = write(users, pass, sizeof(password));
      write(users, '\n',1);
     if(b < 0) {
        printf("%s\n", strerror(errno));
     }
     free(pass);
  }
  
  close(users);
  /*
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
  */
}

int readfile(){
  int users  = open("users.txt", O_RDWR | O_APPEND);
  char * line_buffer = (char *) malloc(sizeof(char) * 400);
  int f = read(users,line_buffer,400);
  printf("%s\n",line_buffer);
}

int main(){
  start();
  readfile();
  return 0;
}
