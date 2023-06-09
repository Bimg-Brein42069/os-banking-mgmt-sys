#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include "IMT2020129_database.h"
#include "IMT2020129_constants.h"

bool single_usr_auth(normalUser currUser){
	int i=currUser.userID-1;
	int fd=open(NORMALUSER_DB_PATH, O_RDONLY, 0744);
	bool result;
	normalUser temp;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    	     //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(normalUser));
	if(!strcmp(temp.password,currUser.password) && !strcmp(temp.status,"ACTIVE"))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool joint_usr_auth(jointUser currUser){
	int i=currUser.userID-1;
	int fd=open(JOINTUSER_DB_PATH, O_RDONLY, 0744);
	bool result;
	jointUser temp;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	     //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(jointUser));
	if(!strcmp(temp.password,currUser.password) && !strcmp(temp.status,"ACTIVE"))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool admin_auth(admin currUser){
	int i=currUser.userID-1;
	int fd=open(ADMIN_DB_PATH, O_RDONLY, 0744);
	bool result;
	admin temp;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     //nth record
	lock.l_len=sizeof(admin);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(admin),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(admin));
	if(!strcmp(temp.password,currUser.password))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

void server_task(int nsd){
	int msgLength,select,type,option,accType,currUserID;
	bool result;
	while(1){
		read(nsd,&option,sizeof(option));
		printf("Option : %d\n",option);
		switch(option){
			case 1:
				normalUser currUser1;
				accType=1;
				msgLength=read(nsd,&currUser1,sizeof(normalUser));
				printf("Username : %d\n",currUser1.userID);
				printf("Password : %s\n",currUser1.password);
				currUserID=currUser1.userID;
				result=single_usr_auth(currUser1);
				write(nsd,&result,sizeof(result));
				break;
			case 2:
				jointUser currUser2;
				accType=2;
				msgLength=read(nsd,&currUser2,sizeof(jointUser));
				currUserID=currUser2.userID;
				printf("Username : %d\n",currUser2.userID);
				printf("Password : %s\n",currUser2.password);
				result=joint_usr_auth(currUser2);
				write(nsd,&result,sizeof(result));
				break;
			case 3:
				admin currUser3;
				accType=3;
				msgLength=read(nsd,&currUser3,sizeof(admin));
				currUserID=currUser3.userID;
				printf("Username : %d\n",currUser3.userID);
				printf("Password : %s\n",currUser3.password);
				result=admin_auth(currUser3);
				write(nsd,&result,sizeof(result));
				break;

			default:
				result=false;
				write(nsd,&result,sizeof(result));
				break;
		}
		if(result)	
			break;		
	}

	int memflag=1;
	float amt;
	while(memflag){
		read(nsd,&select,sizeof(int));
		switch(option){
			case 1:
			case 2:
				switch(select){
					case 1:
						read(nsd,&amt,sizeof(float));
						result=depositMoney(accType,currUserID,amt);
						write(nsd,&result,sizeof(result));
						break;
					case 2:
						read(nsd,&amt,sizeof(float));
						result=withdrawMoney(accType,currUserID,amt);
						write(nsd,&result,sizeof(result));
						break;
					case 3:
						amt=getBalance(accType,currUserID);
						write(nsd,&amt,sizeof(float));
						break;
					case 4:
						char pwd[10];
						read(nsd,pwd,sizeof(pwd));
						result=alterPassword(accType,currUserID,pwd);
						write(nsd,&result,sizeof(result));
						break;
					case 5:
						if(option==1){
							normalUser user1=getNormalUser(currUserID);
							write(nsd,&user1,sizeof(normalUser));
						}
						else if(option==2){
							jointUser user2=getJointUser(currUserID);
							write(nsd,&user2,sizeof(jointUser));
						}
						break;
					case 6:
						memflag=0;
						break;
				}
				break;
			case 3:
				read(nsd,&type,sizeof(int));
				switch(select){
					case 1:
						if(type==1){
							normalUser newUser1;
							read(nsd,&newUser1,sizeof(normalUser));
							result=addNormalUser(newUser1);
							write(nsd,&result,sizeof(result));
						}
						else if(type==2){
							jointUser newUser2;
							read(nsd,&newUser2,sizeof(jointUser));
							result=addJointUser(newUser2);
							write(nsd,&result,sizeof(result));
						}
						break;
					case 2:
						if(type==1){
							int delUserID1;
							read(nsd,&delUserID1,sizeof(int));
							result=deleteNormalUser(delUserID1);
							write(nsd,&result,sizeof(result));
						}
						else if(type==2){
							int delUserID2;
							read(nsd,&delUserID2,sizeof(int));
							result=deleteJointUser(delUserID2);
							write(nsd,&result,sizeof(result));
						}
						break;
					case 3:
						if(type==1){
							normalUser modUser1;
							read(nsd,&modUser1,sizeof(normalUser));
							result=modifyNormalUser(modUser1);
							write(nsd,&result,sizeof(result));
						}
						else if(type==2){
							jointUser modUser2;
							read(nsd,&modUser2,sizeof(jointUser));
							result=modifyJointUser(modUser2);
							write(nsd,&result,sizeof(result));
						}
						break;
					case 4:
						if(type==1){
							normalUser searchUser1;
							int userID1;
							read(nsd,&userID1,sizeof(int));
							searchUser1=getNormalUser(userID1);
							write(nsd,&searchUser1,sizeof(normalUser));
						}
						else if(type==2){
							jointUser searchUser2;
							int userID2;
							read(nsd,&userID2,sizeof(int));
							searchUser2=getJointUser(userID2);
						write(nsd,&searchUser2,sizeof(jointUser));
						}
						break;
					case 5:
						memflag=0;	
						break;

				}
				break;
			default:
				break;
		}
	}
	close(nsd);
	write(1,"Ended client session.....\n",sizeof("Ended client session.....\n"));
	return;
}

void *connection_handler(void *nsd) {
	int nsfd = *(int*)nsd;
	server_task(nsfd);
}

int main(){
	struct sockaddr_in server,client;
	int sd,nsd,clientLen;
	bool result;
	pthread_t threads;
	sd=socket(AF_INET,SOCK_STREAM,0);

	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(PORTNO);

	bind(sd,(struct sockaddr *)&server,sizeof(server));
	listen(sd,5);	//queue length :: 5 clients per connection
	
	write(1,"Waiting for the client.....\n",sizeof("Waiting for the client.....\n"));
	while(1){
		clientLen=sizeof(client);
		nsd=accept(sd,(struct sockaddr *)&client,&clientLen);

		write(1,"Connected to the client.....\n",sizeof("Connected to the client.....\n"));
		if(pthread_create(&threads,NULL,connection_handler,(void*) &nsd)<0){
			perror("could not create thread");
			return 1;
		}
	}
	pthread_exit(NULL);
	close(sd);
	return 0;
}
