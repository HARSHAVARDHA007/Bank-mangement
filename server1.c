#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"mydb.h"
void Menu(int);
void adminMenu(int);
void singleAccountMenu(int,struct database,struct flock,int);
void jointAccountMenu(int,struct jointdatabase,struct flock,int);
char cur_user[20];
void viewTransactions(int socket_desc,struct database db,struct flock lock,int fd)
{
	struct Transactions trac;
	int flag=0;
	int i=0;
	int flag1=1;
	int fd1 = open("Single_trans.dat",O_RDWR);
	while(read(fd1,&trac, sizeof(struct Transactions)))
	{
		write(socket_desc,&flag1,sizeof(flag1));
		if(!strcmp(trac.uname, db.uname))
		{
			flag=1;
			write(socket_desc,&flag,sizeof(flag));
			write(socket_desc,trac.uname, sizeof(trac.uname));
			write(socket_desc,&trac.amount, sizeof(trac.amount));
			write(socket_desc,&trac.balance, sizeof(trac.balance));
			write(socket_desc,trac.action, sizeof(trac.action));
			write(socket_desc,trac.date, sizeof(trac.date));
		}
		else
		{	flag=0;
			write(socket_desc,&flag,sizeof(flag));
		}
	}
	flag1=0;
	write(socket_desc,&flag1,sizeof(flag1));
	close(fd1);
	singleAccountMenu(socket_desc,db,lock,fd);
}
void viewJointTransactions(int socket_desc,struct jointdatabase db,struct flock lock,int fd)
{
	struct jointTrans trac;
	int flag=0;
	int i=0;
	int flag1=1;
	int fd1 = open("Joint_trans.dat",O_RDWR);
	printf("%d\n",db.acc_id);
	while(read(fd1,&trac,sizeof(struct jointTrans)))
	{
		//printf("%s\n",trac.uname);
		//printf("%s\n",trac.action);
		//printf("%s\n",trac.date);
		write(socket_desc,&flag1,sizeof(flag1));
		if(trac.acc_id == db.acc_id)
		{
			flag=1;
			printf("inner entered\n");
			write(socket_desc,&flag,sizeof(flag));
			write(socket_desc,trac.uname, sizeof(trac.uname));
			write(socket_desc,&trac.acc_id, sizeof(trac.acc_id));
			write(socket_desc,&trac.amount, sizeof(trac.amount));
			write(socket_desc,&trac.balance, sizeof(trac.balance));
			write(socket_desc,trac.action, sizeof(trac.action));
			write(socket_desc,trac.date, sizeof(trac.date));
		}
		else
		{	flag=0;
			write(socket_desc,&flag,sizeof(flag));
		}
	}
	flag1=0;
	write(socket_desc,&flag1,sizeof(flag1));
	close(fd1);
	jointAccountMenu(socket_desc,db,lock,fd);
}
int checkAccountExists(char uname[])
{
	struct database db;
	int fd1 = open("Accounts.dat",O_CREAT | O_RDWR,0666);
	while(read(fd1,&db, sizeof(struct database)))
	{
		if(!strcmp(db.uname, uname)){
				close(fd1);
				return 1;
			}
	}
	close(fd1);
	return 0;
}
void findDetails(struct database db_user)
{
	struct database db;
	int fd1 = open("Accounts.dat", O_CREAT | O_RDWR , 0666);
	while(read(fd1,&db, sizeof (struct database)))
	{
		if(!strcmp(db_user.uname, db.uname))
		{
			db.balance = db_user.balance;
			strcpy(db.pass,db_user.pass);
			lseek(fd1,-sizeof(struct database),SEEK_CUR);
			write(fd1,&db,sizeof(struct database));
			break;
		}
	}
}

void jointFindDetails(struct jointdatabase db_user)
{
	struct jointdatabase db;
	int fd1 = open("Joint_Account.dat", O_CREAT | O_RDWR , 0666);
	while(read(fd1,&db, sizeof (struct jointdatabase)))
	{
		if(!strcmp(db_user.uname1, db.uname1) && !strcmp(db_user.uname2, db.uname2)){
			db.balance = db_user.balance;
			strcpy(db.pass1,db_user.pass1);
			strcpy(db.pass2,db_user.pass2);
			lseek(fd1,-sizeof(struct jointdatabase),SEEK_CUR);
			write(fd1,&db,sizeof(struct jointdatabase));

			break;
		}
	}
}

int jointAccountExists(char uname1[],char uname2[])
{
	struct jointdatabase db;
	int fd1 = open("Joint_Account.dat",O_CREAT | O_RDWR, 0666);
	while(read(fd1,&db, sizeof(struct jointdatabase)))
	{
		if((!strcmp(db.uname1, uname1)) && (!strcmp(db.uname2,uname2)) || (!strcmp(db.uname1, uname2)) && (!strcmp(db.uname2,uname1)))
			{
				close(fd1);
				return 1;
			}
	}
	close(fd1);
	return 0;
}
void addjointAccount(char uname1[], char pass1[], char uname2[], char pass2[], int socket_desc)
{
	int fd1 = open("Joint_Account.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	struct jointdatabase db;
	strcpy(db.uname1, uname1);
	strcpy(db.pass1, pass1);
	strcpy(db.uname2, uname2);
	strcpy(db.pass2, pass2);
	db.balance=0;
	srand(time(0));
	db.acc_id=rand();
	write(fd1,&db, sizeof(struct jointdatabase));
	write(socket_desc, "Account Created", 16);
	close(fd1);
}

void addAccount(char uname[], char pass[],int socket_desc)
{
	int fd1 = open("Accounts.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	struct database db;
	strcpy(db.uname, uname);
	strcpy(db.pass, pass);
	db.balance=0;
	srand(time(0));
	db.acc_id=rand();
	write(fd1,&db, sizeof(struct database));
	write(socket_desc, "Account Created", 16);
	close(fd1);
}

void newJointAccount(int socket_desc, int flag)
{
	char uname1[20], pass1[20], uname2[20], pass2[20];
	read(socket_desc, uname1, sizeof(uname1));
	read(socket_desc, pass1, sizeof(pass1));
	read(socket_desc, uname2, sizeof(uname2));
	read(socket_desc, pass2, sizeof(pass2));
	if(!jointAccountExists(uname1,uname2))
	{
		addjointAccount(uname1, pass1, uname2, pass2, socket_desc);
		if(flag==1)
			Menu(socket_desc);
		else
			adminMenu(socket_desc);
	}
	else
	{
		write(socket_desc, "Username already exist\n", 24);
		if(flag ==1)
			Menu(socket_desc);
		else
			adminMenu(socket_desc);
	}
}
void signUp(int socket_desc,int flag)
{
	char uname[20], pass[20];
	read(socket_desc, uname, sizeof(uname));
	read(socket_desc, pass, sizeof(pass));
	if(!checkAccountExists(uname)){
		addAccount(uname, pass, socket_desc);
		if(flag ==1)
			Menu(socket_desc);
		else
			adminMenu(socket_desc);
}
	else{
		write(socket_desc, "Username already exist\n", 24);
		if(flag ==1)
		Menu(socket_desc);
		else
		adminMenu(socket_desc);
	}
}
void login(int socket_desc,int fd)
{
	char uname[20], pass[20];
	struct flock lock;
	read(socket_desc, uname, sizeof(uname));
	read(socket_desc, pass, sizeof(pass));
	struct database db;
	int flag=1;
	int fd1 = open("Accounts.dat", O_RDWR);
	while(read(fd1,&db, sizeof(struct database)))
	{
		if(!strcmp(db.uname, uname) && !strcmp(db.pass, pass))
			{
				lseek(fd1,-sizeof(struct database),SEEK_CUR);
				lock.l_type=F_WRLCK;
				lock.l_whence=SEEK_CUR;
				lock.l_start=0;
				lock.l_len=sizeof(struct database);
				fcntl(fd1,F_SETLKW,&lock);
				//critical section
				write(socket_desc, &flag, sizeof(flag));
				write(socket_desc, "Login Successful", 17);
				strcpy(cur_user,db.uname);
				printf("db uname: %s\n",db.uname);
				printf("cur_user: %s\n",cur_user);
				singleAccountMenu(socket_desc,db,lock,fd1);
				close(fd1);
				return;
			}
	}
	close(fd1);
	flag=0;
	write(socket_desc, &flag, sizeof(flag));
	write(socket_desc, "Invalid Credentials", 20);
	Menu(socket_desc);
}

void jointAccountLogin(int socket_desc,int fd)
{
	char uname1[20], uname2[20], pass[20];
	struct flock lock;
	read(socket_desc, uname1, sizeof(uname1));
	read(socket_desc, uname2, sizeof(uname2));
	read(socket_desc, pass, sizeof(pass));
	strcpy(cur_user,uname1);
	struct jointdatabase db;
	int flag=1;
	int fd1 = open("Joint_Account.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	while(read(fd1, &db, sizeof(struct jointdatabase)))
	{
		if((!strcmp(db.uname1, uname1) && !strcmp(db.pass1,pass) && !strcmp(db.uname2, uname2)) || (!strcmp(db.uname1, uname2) && !strcmp(db.uname2, uname1) && !strcmp(db.pass2,pass)))
			{
				lseek(fd1,-sizeof(struct jointdatabase),SEEK_CUR);
				lock.l_type=F_WRLCK;
				lock.l_whence=SEEK_CUR;
				lock.l_start=0;
				lock.l_len=sizeof(struct jointdatabase);
				fcntl(fd1,F_SETLKW,&lock);
				//critical section
				write(socket_desc, &flag, sizeof(flag));
				write(socket_desc, "Login Successful", 17);
				jointAccountMenu(socket_desc,db,lock,fd1);
				close(fd1);
				return;
			}
	}

	close(fd1);
	flag=0;
	write(socket_desc, &flag, sizeof(flag));
	write(socket_desc, "Invalid Credentials", 20);
	Menu(socket_desc);
}
void viewDetails(int socket_desc, struct database db,struct flock lock,int fd)
{
	write(socket_desc,db.uname, sizeof(db.uname));
	write(socket_desc,&db.acc_id, sizeof(db.acc_id));
	write(socket_desc,&db.balance, sizeof(db.balance));
	singleAccountMenu(socket_desc,db,lock,fd);
}
void viewJointDetails(int socket_desc, struct jointdatabase db,struct flock lock,int fd)
{
	write(socket_desc,db.uname1, sizeof(db.uname1));
	write(socket_desc,db.uname2, sizeof(db.uname2));
	write(socket_desc,&db.acc_id, sizeof(db.acc_id));
	write(socket_desc,&db.balance, sizeof(db.balance));
	jointAccountMenu(socket_desc,db,lock,fd);
}
void deposit(int socket_desc, struct database db,struct flock lock,int fd)
{
	int amount;
	time_t t;
	time(&t);
	struct Transactions tran;
	strcpy(tran.uname,cur_user);
	printf("In deposit: %s\n",tran.uname);
	strcpy(tran.action,"DEPOSIT");
	strcpy(tran.date,ctime(&t));
	read(socket_desc, &amount, sizeof(amount));
	db.balance = db.balance + amount;
	tran.amount=amount;
	tran.balance=db.balance;
	int fd1 = open("Single_trans.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	write(fd1,&tran,sizeof(struct Transactions));
	close(fd1);
	findDetails(db);
	write(socket_desc, &db.balance, sizeof(db.balance));
	singleAccountMenu(socket_desc,db,lock,fd);
}
void jointDeposit(int socket_desc, struct jointdatabase db,struct flock lock,int fd)
{
	int amount;
	time_t t;
	time(&t);
	struct jointTrans tran;
	strcpy(tran.uname,cur_user);
	printf("In deposit: %s\n",tran.uname);
	strcpy(tran.action,"DEPOSIT");
	strcpy(tran.date,ctime(&t));
	read(socket_desc, &amount, sizeof(amount));
	db.balance = db.balance + amount;
	tran.amount=amount;
	tran.balance=db.balance;
	tran.acc_id=db.acc_id;
	int fd1 = open("joint_trans.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
	write(fd1,&tran,sizeof(struct jointTrans));
	close(fd1);
	jointFindDetails(db);
	write(socket_desc, &db.balance, sizeof(db.balance));
	jointAccountMenu(socket_desc,db,lock,fd);
}
void jointWithdraw(int socket_desc, struct jointdatabase db,struct flock lock, int fd)
{
	int amount, flag=0;
	time_t t;
	read(socket_desc, &amount, sizeof(amount));
	if(amount>=db.balance){
		write(socket_desc,&flag,sizeof(flag));
		write(socket_desc, &db.balance, sizeof(db.balance));
	}
	else{
		flag=1;
		struct jointTrans tran;
		strcpy(tran.uname,cur_user);
		strcpy(tran.action,"WITHDRAWAL");
		strcpy(tran.date,ctime(&t));
		db.balance = db.balance - amount;
		tran.amount=amount;
		tran.balance=db.balance;
		tran.acc_id=db.acc_id;
		int fd1 = open("joint_trans.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
		write(fd1,&tran, sizeof(struct jointTrans));
		close(fd1);
		jointFindDetails(db);
		write(socket_desc,&flag,sizeof(flag));
		write(socket_desc, &db.balance, sizeof(db.balance));
	}
	jointAccountMenu(socket_desc,db,lock,fd);
}
void withdraw(int socket_desc, struct database db,struct flock lock,int fd)
{
	int amount, flag=0;
	time_t t;
	time(&t);
	read(socket_desc, &amount, sizeof(amount));
	if(amount>=db.balance){
		write(socket_desc,&flag,sizeof(flag));
		write(socket_desc, &db.balance, sizeof(db.balance));
	}
	else{
		flag=1;
		struct Transactions tran;
		strcpy(tran.uname,cur_user);
		strcpy(tran.action,"WITHDRAWAL");
		strcpy(tran.date,ctime(&t));
		db.balance = db.balance - amount;
		tran.amount=amount;
		tran.balance=db.balance;
		int fd1 = open("Single_trans.dat", O_CREAT | O_APPEND | O_RDWR , 0666);
		write(fd1,&tran, sizeof(struct Transactions));
		close(fd1);
		findDetails(db);
		write(socket_desc,&flag,sizeof(flag));
		write(socket_desc, &db.balance, sizeof(db.balance));
	}
	singleAccountMenu(socket_desc,db,lock,fd);
}
void jointPassChange(int socket_desc, struct jointdatabase db,struct flock lock)
{
	char pass[MAX];
	read(socket_desc,pass,sizeof(pass));
    if(!strcmp(cur_user,db.uname1))
	   strcpy(db.pass1,pass);
	else
		strcpy(db.pass2,pass);

	jointFindDetails(db);
	Menu(socket_desc);
}
void passChange(int socket_desc, struct database db,struct flock lock)
{
	char pass[MAX];
	read(socket_desc,pass,sizeof(pass));
	strcpy(db.pass,pass);
	findDetails(db);
	Menu(socket_desc);
}
void deleteAccount(int socket_desc)
{
	char uname[20];
	int fd1, fd2;
	fd1 = open("Accounts.dat", O_CREAT | O_RDWR , 0666);
	fd2 = open("Accounts2.dat", O_CREAT | O_RDWR, 0666);
	read(socket_desc, uname, sizeof(uname));
	if(!checkAccountExists(uname)){
		write(socket_desc, "Account does not exist", 23);
	}
	else{
		struct database db[1000], db1;
		int i=0;
		lseek(fd1, 0, SEEK_SET);
		while(read(fd1, (char *)&db[i++], sizeof(struct database)));//getting each user details into one struct
		for(int j=0;j<i-1;j++){
			if(!strcmp(db[j].uname, uname)){
				continue;
			}
			else{
				write(fd2,(char *)&db[j],sizeof(struct database));//writing all users except the user whose details has to be deleted
			}
		}
		lseek(fd2, 0, SEEK_SET);
		fd1=open("Accounts.dat",O_TRUNC|O_RDWR,0666);
		lseek(fd1, 0, SEEK_SET);
		while(read(fd2, (char *)&db1, sizeof(struct database)))
		{
			write(fd1,(char *)&db1,sizeof(struct database));//copying into old file
		}
		lseek(fd1,0, SEEK_SET);
		write(socket_desc,"Account deleted Successfully", 29);
		open("Accounts2.dat",O_TRUNC|O_RDWR,0666);//clearing data in the temporary file for next time use.
	}
	close(fd1);
	close(fd2);
	adminMenu(socket_desc);
}
void deleteJointAccount(int socket_desc)
{
	char uname1[20], uname2[20];
	int fd1, fd2;
	fd1 = open("Joint_Account.dat", O_CREAT | O_RDWR , 0666);
	fd2 = open("Joint_Account2.dat", O_CREAT | O_RDWR, 0666);
	read(socket_desc, uname1, sizeof(uname1));
	read(socket_desc, uname2, sizeof(uname2));
	if(!jointAccountExists(uname1,uname2))
	{
		write(socket_desc, "Account does not exist", 22);
	}
	else{
		struct jointdatabase db[1000], db1;
		int i=0;
		lseek(fd1, 0, SEEK_SET);
		while(read(fd1, (char *)&db[i++], sizeof(struct jointdatabase)));
		for(int j=0;j<i-1;j++)
		{
			if((!strcmp(db[j].uname1, uname1)) && (!strcmp(db[j].uname2,uname2)) || (!strcmp(db[j].uname1, uname2)) && (!strcmp(db[j].uname2,uname1)))
			{
				continue;
			}else{
				write(fd2,(char *)&db[j],sizeof(struct jointdatabase));
			}
		}
		lseek(fd2, 0, SEEK_SET);
		fd1=open("Joint_Account.dat",O_TRUNC|O_RDWR,0666);
		lseek(fd1, 0, SEEK_SET);
		while(read(fd2, (char *)&db1, sizeof(struct jointdatabase)))
		{
			write(fd1,(char *)&db1,sizeof(struct jointdatabase));
		}lseek(fd1,0, SEEK_SET);
		write(socket_desc,"Account deleted Successfully", 29);
		open("Joint_Account2.dat",O_TRUNC|O_RDWR,0666);
	}close(fd1);
	close(fd2);
	adminMenu(socket_desc);
}
void adminLogin(int socket_desc)
{
	char uname[20], pass[20];
	int flag=1;
	read(socket_desc, uname, sizeof(uname));
	read(socket_desc, pass, sizeof(pass));
	if(!strcmp(uname,ADMIN) && !strcmp(pass,PASSWORD)){
		write(socket_desc, &flag, sizeof(flag));
		write(socket_desc,"Welcome Admin", 14);
		adminMenu(socket_desc);
	}
	else{
		flag=0;
		write(socket_desc, &flag, sizeof(flag));
		write(socket_desc,"Invalid Credentials", 20);
		Menu(socket_desc);
	}
}
void jointAccountMenu(int socket_desc,struct jointdatabase db,struct flock lock,int fd)
{
	int choice;
	read(socket_desc, &choice, sizeof(choice));
	switch(choice)
	{
		case 1: viewJointDetails(socket_desc,db,lock,fd);
				break;
		case 2: jointDeposit(socket_desc,db,lock,fd);
				break;
		case 3: jointWithdraw(socket_desc,db,lock,fd);
				break;
		case 4: jointPassChange(socket_desc,db,lock);
				break;
		case 5: lock.l_type=F_UNLCK;
				fcntl(fd,F_SETLK,&lock);
				Menu(socket_desc);
				break;
		}
}
void Menu(int socket_desc)
{
	int choice,fd;
	read(socket_desc, &choice, sizeof(choice));
	switch(choice)
	{
		case 1: signUp(socket_desc,1);
		break;
		case 2: login(socket_desc,fd);
		break;
		case 3: adminLogin(socket_desc);
		break;
		case 4: newJointAccount(socket_desc,1);
		break;
		case 5: jointAccountLogin(socket_desc,fd);
		break;
		case 6: exit(0);
		break;
		default:printf("wrong choice again enter\n");
		        Menu(socket_desc);
		break;

	}
}
void adminMenu(int socket_desc)
{
	int choice;
	char uname[20], pass[20], uname1[20], uname2[20];
	read(socket_desc, &choice, sizeof(choice));
	switch(choice)
	{
		case 1:signUp(socket_desc,0);
			break;
		case 2:deleteAccount(socket_desc);
			break;
		case 3:read(socket_desc, uname, sizeof(uname));
				if(checkAccountExists(uname))
			{
				write(socket_desc, "Account Found", 14);
			}
			else
			{
				write(socket_desc, "Account does not exist with the given Username", 46);
			}
			adminMenu(socket_desc);
			break;
		case 4:newJointAccount(socket_desc,0);
			break;
		case 5:deleteJointAccount(socket_desc);
		     	break;
		case 6:read(socket_desc, uname1, sizeof(uname1));
				read(socket_desc, uname2, sizeof(uname2));
				if(jointAccountExists(uname1,uname2))
			{
				write(socket_desc, "Account Found\0", 14);
			}
			else{
				write(socket_desc, "Account does not exist with the given Username", 46);
			}
			adminMenu(socket_desc);
			break;
		default:Menu(socket_desc);
		break;
	}
}

void singleAccountMenu(int socket_desc, struct database db,struct flock lock,int fd)
{
	int choice;
	read(socket_desc, &choice, sizeof(choice));
	printf("%d\n",choice);
	switch(choice)
	{

		case 1: viewDetails(socket_desc,db,lock,fd);
		break;
		case 2: deposit(socket_desc,db,lock,fd);
		break;
		case 3: withdraw(socket_desc,db,lock,fd);
		break;
		case 4: passChange(socket_desc,db,lock);
		break;
		case 5: viewTransactions(socket_desc,db,lock,fd);
		break;
		case 6:lock.l_type=F_UNLCK;
					fcntl(fd,F_SETLK,&lock);
					Menu(socket_desc);
					break;
	}
}
int main(int argc,char *argv[])
{
	struct sockaddr_in server, client;
	int socket_desc, client_addr_len, nsd;
	socket_desc = socket(AF_INET, SOCK_STREAM,0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(argv[1]));
	if(bind(socket_desc,(struct sockaddr *)(&server), sizeof(server))<0){
		printf("Error at bind\n");
	}listen(socket_desc,5);//maximum 5 clients at a time
	client_addr_len = sizeof(client);
	while(1){
		nsd = accept(socket_desc, (struct sockaddr *)&client, &client_addr_len);
		if(!fork()){

			close(socket_desc);
			Menu(nsd);
			exit(0);
		}else
		{
			close(nsd);
		}

	}close(nsd);
	close(socket_desc);
	return 0;
}
