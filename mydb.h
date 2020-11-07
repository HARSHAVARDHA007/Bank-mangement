#define MAX 20
#define ADMIN "harsha"
#define PASSWORD "nopassword"
struct database
{
	char uname[MAX];
	char pass[MAX];
	int acc_id;
	int balance;
};

struct jointdatabase
{
	char uname1[MAX];
	char pass1[MAX];
	char uname2[MAX];
	char pass2[MAX];
	int acc_id;
	int balance;
};
struct Transactions
{
	char uname[MAX];
  char action[MAX];
  char date[MAX];
  int amount;
  int balance;
};
struct jointTrans
{
	char uname[MAX];
	char action[MAX];
	char date[MAX];
	int acc_id;
	int amount;
	int balance;
};
