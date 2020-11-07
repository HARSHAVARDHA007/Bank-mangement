# Bank-mangement
This project Simulates the simple operations done in bank management software.
This a client server archeiture using socket programming.
Data is stored in files.
Everything is coded in C.
whenever possible i have used System calls.
Record locking is done whereever applicable.
Types of users:
-> normal user
-> Admin
-> Joint account user
=>Features for normal user.
1.add an account
2.see the details of the user
3.deposit
4.withdraw
5.see transactions.
6.change password
=>Features of Admin.
1.Add normal account
2.search for normal account
3.Delete the account
4.Add joint account
5.delete the joint account
6.Search for the joint account
=>Features of joint account
1.Create a new joint account
2.deposit
3.withdraw
4.change password

# compilation procedure
gcc mydb.h
terminal 1:gcc server.c <port-no>
terminal 2:gcc client.c <port-no>
