#include <unistd.h>
#include <fcntl.h>

#include "networking.h"

int main(int argc, char **argv) {

  /* int server_socket;
  char buffer[BUFFER_SIZE];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP ); */

  int users = open("users.txt", O_RDONLY | O_APPEND | 0644);

  char username[100];
  char password[100];
  char answer[2];

  printf("Do you have an account? [y/n]\n");
  fgets(answer, sizeof(answer), stdin);
  if(answer[0] == 'y'){
    printf("Enter your username: \n");
    fgets(username, sizeof(username), stdin);
    printf("Enter your password: \n");
    fgets(password, sizeof(password), stdin);

  
    char user[200];
    int found = 0;
    while(read(users, user, 200) && !found){
      printf("%s\n", user);
    }
  } else if(answer[0] == 'n'){
     printf("Enter new username: \n");
     fgets(username, sizeof(username), stdin);  
     write(users, username, sizeof(username));
     printf("Enter new password: \n");
     fgets(password, sizeof(password), stdin);
     write(users, password, sizeof(password));
  }
  
  close(users);

  /* while (1) {
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
  } */
}
