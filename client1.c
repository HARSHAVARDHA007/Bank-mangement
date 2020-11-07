#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
void singleAccountMenu(int);
void jointAccountMenu(int);
void adminMenu(int);
void Menu(int);
void viewTransactions(int socket_desc)
{
	char uname[20],time[20],action[20];
	int balance,amount;
	int flag,flag1;
	while(1)
	{
		read(socket_desc,&flag1,sizeof(flag1));
		if(!flag1)
		break;
	while(flag1)
	{
		read(socket_desc,&flag,sizeof(flag));
		if(flag)
		{
		read(socket_desc,uname,sizeof(uname));
		read(socket_desc,&amount,sizeof(amount));
		read(socket_desc,&balance,sizeof(balance));
		read(socket_desc,action, sizeof(action));
		read(socket_desc,time, sizeof(time));
		printf(" Date and time: %s, action: %s, uname: %s, amount: %d, balance: %d \n",time,action,uname,amount,balance);
	  }
		flag1=0;
		flag=0;
	}
}
	singleAccountMenu(socket_desc);

}
void viewJointTransactions(int socket_desc)
{
	char uname[20],time[20],action[20];
	int balance,amount,acc_id;
	int flag,flag1;
	while(1)
	{
		read(socket_desc,&flag1,sizeof(flag1));
		if(!flag1)
		break;
	while(flag1)
	{
		read(socket_desc,&flag,sizeof(flag));
		if(flag)
		{
		read(socket_desc,uname,sizeof(uname));
		read(socket_desc,&acc_id,sizeof(acc_id));
		read(socket_desc,&amount,sizeof(amount));
		read(socket_desc,&balance,sizeof(balance));
		read(socket_desc,action, sizeof(action));
		read(socket_desc,time, sizeof(time));
		printf("Account_no: %d ,Date and time: %s, action: %s, uname: %s, amount: %d, balance: %d \n",acc_id,time,action,uname,amount,balance);
	  }
		flag1=0;
		flag=0;
	}
}
	jointAccountMenu(socket_desc);

}
void deleteAccount(int socket_desc)
{
	char uname[20],buffer[35];
	printf("\nEnter the username to be deleted: ");
	scanf(" %s", uname);
	write(socket_desc, uname, sizeof(uname));
	read(socket_desc, buffer, sizeof(buffer));
	printf("%s\n",buffer);
	printf("------------------------------------------------\n");
	adminMenu(socket_desc);
}
void deleteJointAccount(int socket_desc)
{
	char uname1[20], uname2[20], buffer[35];
	printf("Enter Username 1 to delete: ");
	scanf(" %s",uname1);
	printf("Enter Username 2 to delete: ");
	scanf(" %s",uname2);
	write(socket_desc, uname1, sizeof(uname1));
	write(socket_desc, uname2, sizeof(uname2));
	read(socket_desc, buffer, sizeof(buffer));
	printf("%s\n",buffer);
	printf("------------------------------------------------\n");
	adminMenu(socket_desc);
}
void newJointAccount(int socket_desc, int flag)
{
	char uname1[20], pass1[20], uname2[20], pass2[20], buffer[30];
	printf("Enter first username: ");
	scanf(" %s", uname1);
	printf("Enter password: ");
	scanf(" %s", pass1);
	printf("Enter second username: ");
	scanf(" %s", uname2);
	printf("Enter password: ");
	scanf(" %s", pass2);
	write(socket_desc, uname1, sizeof(uname1));
	write(socket_desc, pass1, sizeof(pass1));
	write(socket_desc, uname2, sizeof(uname2));
	write(socket_desc, pass2, sizeof(pass2));
	read(socket_desc, buffer, sizeof(buffer));
	printf("%s\n", buffer);
	printf("------------------------------------------------\n");
	if(flag==1)
		Menu(socket_desc);
	else
		adminMenu(socket_desc);
}
void signUp(int socket_desc, int flag)
{
	int account_id;
	char uname[20],pass[20], buffer[30];
	printf("Enter username: ");
	scanf(" %s",uname);
	printf("Enter password: ");
	scanf(" %s",pass);
	write(socket_desc, uname,sizeof(uname));
	write(socket_desc, pass, sizeof(pass));
	read(socket_desc, buffer, sizeof(buffer));
	printf("%s\n", buffer);
	printf("------------------------------------------------\n");
	if(flag==1)
		Menu(socket_desc);
	else
		adminMenu(socket_desc);
}
void login(int socket_desc)
{
	char buffer[30];
	char uname[20],pass[20];
	int flag;
	printf("Enter username: ");
	scanf(" %s",uname);
	printf("Enter password: ");
	scanf(" %s",pass);
	write(socket_desc, uname,sizeof(uname));
	write(socket_desc, pass, sizeof(pass));
	read(socket_desc, &flag, sizeof(flag));
	read(socket_desc, buffer, sizeof(buffer));
	printf("%s\n",buffer);
	printf("------------------------------------------------\n");
	if(flag==1)
		singleAccountMenu(socket_desc);
	else
		Menu(socket_desc);
	}
void jointAccountLogin(int socket_desc)
{
	char buffer[30];
	char uname1[20],pass[20],uname2[20];
	int flag;
	printf("Enter first username: ");
	scanf(" %s",uname1);
	printf("Enter second username: ");
	scanf(" %s",uname2);
	printf("Enter password for first user: ");
	scanf(" %s",pass);
	write(socket_desc, uname1,sizeof(uname1));
	write(socket_desc, uname2,sizeof(uname2));
	write(socket_desc, pass, sizeof(pass));
	read(socket_desc, &flag, sizeof(flag));
	read(socket_desc, buffer, sizeof(buffer));
	printf("%s\n",buffer);
	printf("------------------------------------------------\n");
	if(flag==1)
	{
		jointAccountMenu(socket_desc);
	}
	else
		Menu(socket_desc);}

void viewDetails(int socket_desc)
{
	char uname[20];
	int acc_id, balance;

	read(socket_desc,uname, sizeof(uname));
	read(socket_desc,&acc_id, sizeof(acc_id));
	read(socket_desc,&balance, sizeof(balance));
	printf("Username: %s\n",uname);
	printf("Account ID: %d\n",acc_id);
	printf("Balance: %d\n",balance);
	printf("------------------------------------------------\n");
	singleAccountMenu(socket_desc);
}
void viewJointDetails(int socket_desc)
{
	char uname1[20], uname2[20];
	int acc_id, balance;

	read(socket_desc,uname1, sizeof(uname1));
	read(socket_desc,uname2, sizeof(uname2));
	read(socket_desc,&acc_id, sizeof(acc_id));
	read(socket_desc,&balance, sizeof(balance));
	printf("Username1: %s,Username2: %s\n",uname1,uname2);
	printf("Account ID: %d\n",acc_id);
	printf("Balance: %d\n",balance);
	printf("------------------------------------------------\n");
	jointAccountMenu(socket_desc);
}
void deposit(int socket_desc)
{
	int amount;
	printf("\nEnter amount to deposit: ");
	scanf("%d",&amount);

	write(socket_desc, &amount, sizeof(amount));
	read(socket_desc, &amount, sizeof(amount));
	printf("\n");
	printf("New Balance: %d",amount);
	printf("------------------------------------------------\n");
	singleAccountMenu(socket_desc);
}
void jointDeposit(int socket_desc)
{
	int amount;
	printf("\nEnter amount to deposit: ");
	scanf("%d",&amount);
	write(socket_desc, &amount, sizeof(amount));
	read(socket_desc, &amount, sizeof(amount));
	printf("New Balance: %d",amount);
	printf("\n");
	printf("------------------------------------------------\n");
	jointAccountMenu(socket_desc);
}
void jointWithdraw(int socket_desc)
{
	int amount,flag;
	printf("\nEnter amount to withdraw: ");
	scanf("%d",&amount);
	write(socket_desc, &amount, sizeof(amount));
	read(socket_desc, &flag, sizeof(flag));
	if(flag==0)
	{
		printf("Insufficient Balance\n");
		read(socket_desc, &amount, sizeof(amount));
		printf("Balance: %d",amount);
		printf("\n");
		printf("------------------------------------------------\n");
	}
	else
	{
		read(socket_desc, &amount, sizeof(amount));
		printf(" NewBalance: %d",amount);
		printf("\n");
		printf("------------------------------------------------\n");
	}
jointAccountMenu(socket_desc);
}
void withdraw(int socket_desc)
{
	int amount,flag;
	printf("\nEnter amount to withdraw: ");
	scanf("%d",&amount);
	write(socket_desc, &amount, sizeof(amount));
	read(socket_desc, &flag, sizeof(flag));
	if(flag==0)
	{
		printf("Insufficient Balance\n");
		read(socket_desc, &amount, sizeof(amount));
		printf("Balance: %d",amount);
		printf("\n");
		printf("------------------------------------------------\n");
	}
	else
	{
		read(socket_desc, &amount, sizeof(amount));
		printf(" NewBalance: %d",amount);
		printf("\n");
		printf("------------------------------------------------\n");
	}
	singleAccountMenu(socket_desc);
}
void jointPassChange(int socket_desc)
{
	char pass[20];
	printf("\nEnter the new Password: ");
	scanf(" %s",pass);
	write(socket_desc,pass,sizeof(pass));
	Menu(socket_desc);
}
void passChange(int socket_desc)
{
	char pass[20];
	printf("\nEnter the new Password: ");
	scanf(" %s",pass);
	write(socket_desc,pass,sizeof(pass));
	Menu(socket_desc);

}
void adminLogin(int socket_desc)
{
	char uname[20], pass[20], buffer[20];
	int flag;
	printf("\nEnter the admin username: ");
	scanf(" %s",uname);
	printf("\nEnter admin password: ");
	scanf(" %s",pass);
	write(socket_desc, uname, sizeof(uname));
	write(socket_desc, pass, sizeof(pass));
	read(socket_desc, &flag, sizeof(flag));
	read(socket_desc, buffer, sizeof(buffer));
	printf("------------------------------------------------\n");
	printf("%s\n",buffer);
	printf("------------------------------------------------\n");
	if(flag == 1)
		adminMenu(socket_desc);
	else
		Menu(socket_desc);
}
void Menu(int socket_desc)
{
	int choice;
	printf("\n1.SIGNUP\n2.LOGIN\n3.ADMIN LOGIN\n4.NEW JOINT ACCOUNT\n5.JOINT ACCOUNT LOGIN\n6.EXIT\n");
	printf("Enter your choice: ");
	scanf("%d",&choice);
	write(socket_desc, &choice, sizeof(choice));
	switch(choice)
	{
		case 1: signUp(socket_desc,1);
				break;
		case 2: login(socket_desc);
				break;
		case 3: adminLogin(socket_desc);
				break;
		case 4: newJointAccount(socket_desc,1);
				break;
		case 5: jointAccountLogin(socket_desc);
				break;
		case 6: exit(0);
				break;
		default:printf("wrong choice again enter\n");
		        Menu(socket_desc);
						break;
	}
}
void singleAccountMenu(int socket_desc)
{
	int choice;
	printf("\n1.View Details\n2.Deposit\n3.Withdraw\n4.Password Change\n5.View transactions\n6.Exit\n");
	printf("Enter your choice: ");
	scanf("%d",&choice);
	write(socket_desc, &choice, sizeof(choice));
	switch(choice)
	{
		case 1: viewDetails(socket_desc);
		break;
		case 2: deposit(socket_desc);
		break;
		case 3: withdraw(socket_desc);
		break;
		case 4: passChange(socket_desc);
		break;
		case 5: viewTransactions(socket_desc);
		break;
		case 6: Menu(socket_desc);
		break;
		default: printf("Enter proper choice");
		break;
	}
}
void jointAccountMenu(int socket_desc)
{
	int choice;
	printf("\n1.View Details\n2.Deposit\n3.Withdraw\n4.Password Change\n5.Exit");
	printf("Enter your choice: ");
	scanf("%d",&choice);
	write(socket_desc, &choice, sizeof(choice));
	switch(choice)
	{
		case 1: viewJointDetails(socket_desc);
				break;
		case 2: jointDeposit(socket_desc);
				break;
		case 3: jointWithdraw(socket_desc);
				break;
		case 4: jointPassChange(socket_desc);
				break;
		case 5: Menu(socket_desc);
				break;
	}

}
void adminMenu(int socket_desc)
{
	int choice;

	printf("\n1.Add Account\n2.Delete Account\n3.Search Account\n4.Add Joint Account\n5.Delete Joint Account\n6.Search Joint Account\n7.Exit\nEnter your choice: ");
	scanf("%d", &choice);

	char buffer[46], uname[20], pass[20], uname1[20], uname2[20];
	write(socket_desc, &choice, sizeof(choice));

	switch(choice)
	{
		case 1:signUp(socket_desc,0);
			break;
		case 2:deleteAccount(socket_desc);
			break;
		case 3:printf("\nEnter username to search: ");
			scanf(" %s", uname);
			write(socket_desc, uname, sizeof(uname));
			read(socket_desc, &buffer, sizeof(buffer));
			printf(" %s\n", buffer);
			printf("------------------------------------------------\n");
			adminMenu(socket_desc);
			break;
		case 4:newJointAccount(socket_desc,0);
			break;
		case 5:deleteJointAccount(socket_desc);
			break;
		case 6:printf("\nEnter username 1 to search: ");
			scanf(" %s", uname1);
			printf("\nEnter username 2 to search: ");
			scanf(" %s", uname2);
			write(socket_desc, uname1, sizeof(uname1));
			write(socket_desc, uname2, sizeof(uname2));
			read(socket_desc, &buffer, sizeof(buffer));
			printf("%s\n", buffer);
			printf("------------------------------------------------\n");
			adminMenu(socket_desc);
			break;
		case 7:Menu(socket_desc);
		break;
	}
}
int main(int argc,char *argv[])
{
	struct sockaddr_in server;
	int socket_desc;
	char buffer[80];
	socket_desc = socket(AF_INET, SOCK_STREAM,0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(argv[1]));
	connect(socket_desc, (struct sockaddr *)(&server), sizeof(server));
	Menu(socket_desc);
	close(socket_desc);
	return 0;
}
