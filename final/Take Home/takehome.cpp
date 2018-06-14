#include <iostream>
#include <limits>
#include <map>

//Bank API
//1.a.

//Forward declaration
class User;
class Banker;
class SavingsAccount;
class CheckingAccount;

class BankAccount{
	//Allows User to access BankAccounts private members
	friend class User;
	public:
		//Create account with a pin and number
		BankAccount(int pin, int number){

		}

		//Receive a history of your banking purchases/withdrawels/deposits
		std::string Statement(int pin){
		
		}

		//Put money in account
		virtual void Deposit(int money_in) = 0;

		//Take money from account
		virtual int Withdrawel(int money_out, int pin) = 0;

		//Checks for sufficient funds
		virtual bool SufficientFunds(int money_out) = 0;

		//Transfers money from one account to a savings account
		virtual void TransferMoneySavings(SavingsAccount &s, int money_out) = 0;

		//Transfers money from one account to a checking account
		virtual void TransferMoneyChecking(CheckingAccount &c, int money_out) = 0;
	
		private:
			int pin;
			int bank_number;
};

class CheckingAccount : public BankAccount{
	//Deposit money in checking account
	virtual void Deposit(int money_in){

	}

	//Withdraw money from checking, pin required for security
	//Will call SufficientFunds to check if the request is possible
	virtual int Withdrawel(int money_out, int pin){
		
	}

	//Checks if the money being withdrawn can be withdrawn
	virtual bool SufficientFunds(int money_out){

	}

	//Get checking account for changes to be made by banker or user
	//pin required for authorization purposes
	CheckingAccount AccessChecking(int pin){

	}

	//Transfers money from checkings to savings
	void TransferMoneySavings(SavingsAccount &s, int money_out){

	}

	//Transfer money from one checking to another checking
	void TransferMoneyChecking(CheckingAccount &c, int money_out){

	}

	private:
		int money;
		int checking_number;
};

class SavingsAccount : public BankAccount{
	//Deposit money in savings account
	virtual void Deposit(int money_in){

	}

	//Withdraw money from checking, pin required for security
	//Will call SufficientFunds to check if the request is possible
	virtual int Withdrawel(int money_out, int pin){
		
	}

	//Checks if the money being withdrawn can be withdrawn
	virtual bool SufficientFunds(int money_out){

	}

	//Get savings account for changes to be made by banker or user
	//pin required for authorization purposes
	SavingsAccount AccessSavings(int pin){

	}

	//Transfers money from savings to another savings
	void TransferMoneySavings(SavingsAccount &s, int money_out){

	}

	//Transfer money from one savings to checking
	void TransferMoneyChecking(CheckingAccount &c, int money_out){

	}



	private:
		int money;
		int savings_number;
};

class DebitCard{
	//Allows User to access DebitCards private members
	friend class User;
	//Creates debit card without pin, user has to set it
	DebitCard(std::string name, BankAccount *b, int cvc, long digits, int expiration) : name(name), bank_account(b), cvc(cvc), digits(digits), expiration(expiration) {}

	//Can't use debit card until it has a pin
	void ActivateDebitCard(int pin){
		pin = pin;
	}
	
	//Pop up window like on an atm that shows Checking balance to screen
	void ViewChecking(int pin){

	}

	//Pop up window like on an atm that shows Savings balance to screen
	void ViewSavings(int pin){

	}

	//If pin needs to be updated, verify with the old pin
	//and update to the new pin
	void UpdatePin(int old_pin, int new_pin){
		
	}

	private:
		std::string name;
		BankAccount *bank_account;
		int pin;
		int cvc;
		long digits;
		int expiration;
};

class User{
	//To allow banker to access Users fields for processing payments
	friend class Banker;
	public:
	User(std::string name, int social_security, BankAccount *b) : name(name), social_security(social_security), bank_account(b) {}

	//Calls Statement on the BankAccount member to receive a statement
	std::string getStatement(int pin){

	}

	//Pop up window like on an atm that shows Checking balance to screen
	void ViewChecking(int pin){

	}

	//Pop up window like on an atm that shows Savings balance to screen
	void ViewSavings(int pin){

	}

	//If pin needs to be updated, verify with the old pin
	//and update to the new pin
	void UpdatePin(int old_pin, int new_pin){
		
	}

	//Apply for a specified loan amount and return bool
	//if the request was granted or not granted
	bool ApplyForLoan(Banker &banker, int loan_amount){

	}
	
	//Passes itself into DeleteUserAccount for the bankers to take them out of their 
	//system then Calls Destructor on User class
	void CloseBankAccount(){

	}

	//Calls bankers CreateDebitCard and set debit_card's value to its returned value
	//Deletes old debit_card if one exists
	void RequestDebitCard(){

	}

	//Calls Debit cards activate debit card function to give it the specified pin
	void ActivateDebitCard(int pin){

	}

	private:
		std::string name;
		int social_security;
		DebitCard *debit_card;
		BankAccount *bank_account;
};


class Banker{
	public:
	//Runs analytics on users accounts and decides if they can afford
	//To give out the request loan
	bool ProcessLoanRequest(int loan_amount){
		
	}

	//Create a new User at the banking company
	//Pin requested by user used to call GenerateNewBankAccount
	User *OpenBankAccount(int social_security, int pin){
		
	}

	/*
	 * Banker uses User requested pin
	 * Search's through map until it finds a hole in the sequential ordering of
	 * Bank numbers and uses that number for the new bank account
	 */
	BankAccount *GenerateNewBankAccount(int pin){
		
	}
	
	//removes user from the map using the user's bank account info
	//provided from the CloseBankAccount function
	void DeleteUserAccount(){

	}

	//Creates a new debit card using an open debit card number and users provided information
	DebitCard *CreateDebitCard(User &user){
		
	}
	
	private:
		//Map of all used bank account #'s, when a banker generates a new one it checks for an unused spot
		std::map<int, BankAccount> bank_account_numbers;
		//Map of all used debit card #'s, when a banker generates a new one it checks for an unused spot
		std::map<int, DebitCard> debit_card_numbers;
};

/*
1. b. tests for validation would be depositing multiple amounts of money and viewing each transaction
   Checking bank statement after every transaction
   trying to gain access to account with user information
   Attempting to apply for a loan from multiple bankers using same statistical analysis on users
   Testing the debit cards functionality, including being able to use only when correct info is used
   Transfer money from savings to savings, savings to checkings, checkings to savings, and checkings to checkings

1. c. 

map<int, int> checkingAccountNumbers; //global socialSecurity in, Checking account # out
map<int, int> savingAccountNumbers; //global socialSecurity in, Savings account # out
map<int, int> accounts;       //global bank account # in, money amount out
map<int, std::string> statements //global bank account # in, string of transactions out

//Accesses accounts returns money
int viewChecking(int accountNumber, int pin){

}

//Uses social security to update checkingAccountNumbers
void makeChecking(int social_security, int pin){

}

//Transfers money from checking to savings
void transferCheckingToSavings(int bank_account_number, int pin, int bank_account_number2){

}

//Transfers money from checking to Checking
void transferCheckingToChecking(int bank_account_number, int pin, int bank_account_number2){

}


//Removes checking
void removeChecking(int social_security, int bank_account_number, int pin){

}

//Accesses accounts returns money
int viewSavings(int account_number, int pin){

}

//Uses social security to update savingAccountNumbers
void makeSavings(int social_security, int pin){

}

//Transfers money from savings to checking
void transferSavingsToChecking(int bank_account_number, int pin, int bank_account_number2){

}

//Transfers money from savings to savings
void transferSavingsToSavings(int bank_account_number, int pin, int bank_account_number2){

}


//Removes savings
void removeSavings(int social_security, int bank_account_number, int pin){

}

//Statement updates in here as well as money coming out
int withdrawChecking(int account_number, int withdraw_amount){

}

//Statement updates as well as money going in
void depositChecking(int account_number, int deposit_amount){

}

//Statement updates as well as money coming out
int withdrawSavings(int account_number, int withdraw_amount){

}

//Statement updates as well as money going in
void depositSavings(int account_number, int deposit_amount){

}

//Gets string out of statements with a concatenated string of history
std::string statement(int account_number, int pin){

}

//Checks to make sure user provided correct info and deletes account from checking
//account numbers
void removeCheckingAccount(int social_security, int bank_account_number, int pin){

}

//Checks to make sure user provided correct info and deletes account from saving
//account numbers
void removeSavingsAccount(int social_security, int bank_account_number, int pin){

}

//Move money from bank_account1 to bank_account2
//Uses pin1 for access to bank_account1
void transferMoneyAccount(int bank_account1, int bank_account2, pin1){

}

//Apply for loan approved or disapproved based on transaction history (true/false)
bool applyForLoan(int social_security, int bank_account_number, int pin){

}

//Checks if the amount being withdrawn won't overdraft
bool sufficientFunds(int bank_account_number, int pin){

}

1. c. Advantages/Disadvantages
      Advantages:
		It all fits in one area, with no need to worry about inheritance (Easier to create)
		Global variables are easier to use since they can be accessed everywhere, but is also a disadvantage listed below
      Disadvantages:
		Harder to maintain
		Much more stuff being passed into each function call
		Less Secure, no private members and such(all global space)

2. Code can be made more maintainable by error checking a ton and separating each 
   step in processing bank account stuff into their own functions.
   Comments over difficult to understand parts can be helpful as well.

3. Whenever I created a new pointer to an object reference, I incremented the counter.  This 
   increment was wrapped in a mutex to prevent threads from updating at the same time.  Whenever
   I deleted a pointer to an object reference, the count was decremented and the decrement was
   wrapped in a mutex to prevent threads from udpating at the same time.  Every Smart Pointer shared
   a lock if they pointed to the same object in memory.
*/

/*
4
*/
int maxLoop(int *a, int n){
	int max = std::numeric_limits<int>::min();
	for(int i = 0; i < n; i++){
		if(a[i] > max) max = a[i];
	}
	return max;
}

int maxFunc(const int *a, const int n){
	if(n == 1) return a[0];
	return std::max(a[n-1], maxFunc(a, n-1));
}

int main(){

}