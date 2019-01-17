#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int check(char * u,char * p){
  char str[100];
  FILE * file;
  file = fopen("users.txt", "r");
  int i = 0;
  strtok(p,"\n");
  if (file) {
    while (fscanf(file, "%s", str)!=EOF){
      //printf("line %d %s\n",i,str);
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

int start(){
  
  int file = open("users.txt", O_RDWR | O_APPEND);
  printf("%d\n", file);
  if(file < 0){
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
    //read(file, user, 200);
    //printf("%s\n", user);
    //printf("%ld\n", strlen(user));
    if(check(username,password) == 0){
      printf("Logging in\n");
    }
    else{
      printf("Incorrect Username/Password\n");
	}
    
  } else if(answer[0] == 'n'){
     printf("Enter new username: ");
     fgets(username, sizeof(username), stdin);
     //strtok(username,"\n");
     char use[100];
     memset(use,'\0',100);
     strcpy(use,username);
     printf("%s \n",use);
     int a = write(file, use, strlen(use));
    //  write(users, '\n',1);
     if(a < 0) {
        printf("%s\n", strerror(errno));
     }
     printf("Enter new password: ");
     fgets(password, sizeof(password), stdin);
     //strtok(password,"\n");
     char pass[100];
     memset(pass,'\0',100);
     strcpy(pass,password);
     int b = write(file, pass, strlen(pass));
      //write(users, '\n',1);
     if(b < 0) {
        printf("%s\n", strerror(errno));
     }
  }
  
  close(file);
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


int main(){
  start();
  return 0;
}
