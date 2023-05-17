#include "IMT2020129_database.h"
#include "IMT2020129_constants.h"
#include <sys/types.h>          /* See NOTES */
#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    mkdir(BASE_PATH, 0744);
    int fd1 = open(NORMALUSER_DB_PATH, O_RDWR | O_CREAT, 0744); 
    normalUser u1 = {1,"Karamjit","1234",1000,1000.0,"ACTIVE"};
    write(fd1, &u1, sizeof(u1));
    printf("Normal user seeds:\n");
    printf("id=%d\tname=%s\tpassword=%s\n",u1.userID, u1.name, u1.password);
    close(fd1);

    int fd2 = open(JOINTUSER_DB_PATH, O_RDWR | O_CREAT, 0744);
    jointUser u2 = {1,"New","Orleans","2345",2000,2000.0,"ACTIVE"}; 
    write(fd2, &u2, sizeof(u2));
    printf("Joint user seeds:\n");
    printf("id=%d\tname1=%s\tname2=%s\tpassword=%s\n",u2.userID, u2.name1, u2.name2, u2.password);
    close(fd2);

    int fd3 = open(ADMIN_DB_PATH, O_RDWR | O_CREAT, 0744);
    admin u3 = {1,"admin","12345678"}; 
    write(fd3, &u3, sizeof(u3));
    printf("Admin seeds:\n");
    printf("id=%d\tusername=%s\tpassword=%s\n",u3.userID, u3.username, u3.password);
    close(fd3);
    return 0;
}
