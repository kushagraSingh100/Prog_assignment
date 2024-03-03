#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <mysql.h>
#include <mysql_error.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <windows.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

const string server = "localhost";
const string username = "root";
const string password = " ";


char encryptCh(char ch, int shift) {
	if (isalpha(ch)) {
		char base = isupper(ch) ? 'A' : 'a';
		char encrypted_ch = (ch - base + shift + 26) % 26 + base;
		return encrypted_ch;
	}
	if (isdigit(ch)) {
		char encrypted_ch = (ch - '0' + shift + 10) % 10 + '0';
		return encrypted_ch;
	}
	return ch;
}

string encrypt(const string& password, int shift) {
	string encrypted = "";
	for (int i = 0; i < password.length(); i++) {
		char ch = password[i];
		char encryptedChar = encryptCh(ch, shift);
		encrypted += encryptedChar;
	}
	return encrypted;
}

char decryptCh(char ch, int shift) {
	if (isalpha(ch)) {
		char base = isupper(ch) ? 'A' : 'a';
		char decrypted_ch = (ch - base - shift + 26) % 26 + base;
		return decrypted_ch;
	}
	else if (isdigit(ch)) {
		char decrypted_ch = (ch - '0' - shift + 10) % 10 + '0';
		return decrypted_ch;
	}
	else {
		return ch;
	}
}

string decrypt(const string& encrypted, int shift) {
	string decrypted = "";
	for (size_t i = 0; i < encrypted.length(); i++) {
		char ch = encrypted[i];
		char decryptedChar = decryptCh(ch, shift);
		decrypted += decryptedChar;
	}
	return decrypted;
}
string DBpw(sql::Connection* con, int id, const string& role) {
	string encryptedPW;

	string get = "SELECT PW FROM " + role + "_password WHERE ID = ?";
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement(get);
		pstmt->setInt(1, id);

		sql::ResultSet* res = pstmt->executeQuery();

		if (res) {
			if (res->next()) {
				encryptedPW = res->getString("PW");
			}
			delete res;  // Clean up result set
		}

		delete pstmt;  // Clean up prepared statement
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
	}

	return encryptedPW;
}

int shift = 3;
class User {
public:
	string Name;
	int ID;
	string Password;
};

class Customer : public User {
private:
	int Customer_ID,Fine_Due,Customer_Record;
	string userpw;
	int N_of_days;
	string Start_date, Return_date;
	bool Active,Damage_caused;
	int dailyRateFromDB;

public:
	Customer(int a, string pw, int cr, string b, string c, bool f, int h, bool i, sql::Connection* con1, sql::Connection* con2);
	int calculateDateDifference(const std::string& startDate, const std::string& endDate) {
		// Convert string dates to time_point
		std::tm tmStart = {};
		std::tm tmEnd = {};
		std::istringstream ssStart(startDate);
		std::istringstream ssEnd(endDate);
		ssStart >> std::get_time(&tmStart, "%Y-%m-%d");
		ssEnd >> std::get_time(&tmEnd, "%Y-%m-%d");

		auto tpStart = std::chrono::system_clock::from_time_t(std::mktime(&tmStart));
		auto tpEnd = std::chrono::system_clock::from_time_t(std::mktime(&tmEnd));

		// Calculate the difference in days
		auto duration = tpEnd - tpStart;
		return std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24;
	}
	void setdc(bool x) {
		Damage_caused = x;
	}
	void setfd(int s) {
		Fine_Due = s;
	}
	bool getdc() {
		return Damage_caused;
	}
	int getfd() {
		return Fine_Due;
	}
	void setid(int a) {
		Customer_ID = a;
	}
	void set_sd(string b) {
		Start_date = b;
	}
	void set_ad(string c) {
		Return_date = c;
	}
	void seta(bool f) {
		Active = f;
	}
	
	int getid() {
		return Customer_ID ;
	}
	string get_sd() {
		return Start_date;
	}
	string get_ad() {
		return Return_date;
	}
	int get_n() {
		return N_of_days;
	}
	bool geta() {
		return Active;
	}
	void setrec(int rec) {
		Customer_Record = rec;
	}
	int getrec() {
		return Customer_Record;
	}
	void setpw(string pw) {
		userpw = pw;
	}
	string getpw() const {
		return userpw;
	}
	string getenc() {
		string encryptedPW = encrypt(userpw, shift);
		return encryptedPW;
	}

};
Customer::Customer(int a,string pw, int cr,string b, string c, bool f,int h, bool i, sql::Connection* con1, sql::Connection* con2) {
	Customer_ID = a;
	Start_date = b;
	userpw = pw;
	Return_date = c;
	Customer_Record = cr;
	N_of_days = calculateDateDifference(b,c);

	Active = f;	Fine_Due = h; Damage_caused = i;
}

class Employee : public User {
private:
	int ID,Fine_Due,Employee_Record;
	string userpw;
	int N_of_days;
	string Start_date, Return_date;
	bool Active, Damage_caused;

public:
	Employee(int a, string pw, int rec, string b, string c, bool f, int h, bool i, sql::Connection* con);
	void setpw(string pw) {
		userpw = pw;
	}
	string getpw() const {
		return userpw;
	}
	string getenc() {
		string encryptedPW = encrypt(userpw, shift);
		return encryptedPW;
	}

	void setrec(int rec) {
		Employee_Record = rec;
	}
	int getrec() {
		return Employee_Record;
	}
	void setdc(bool x) {
		Damage_caused = x;
	}
	void setfd(int s) {
		Fine_Due = s;
	}
	bool getdc() {
		return Damage_caused;
	}
	int getfd() {
		return Fine_Due;
	}
	int calculateDateDifference(const std::string& startDate, const std::string& endDate) {
		// Convert string dates to time_point
		std::tm tmStart = {};
		std::tm tmEnd = {};
		std::istringstream ssStart(startDate);
		std::istringstream ssEnd(endDate);
		ssStart >> std::get_time(&tmStart, "%Y-%m-%d");
		ssEnd >> std::get_time(&tmEnd, "%Y-%m-%d");

		auto tpStart = std::chrono::system_clock::from_time_t(std::mktime(&tmStart));
		auto tpEnd = std::chrono::system_clock::from_time_t(std::mktime(&tmEnd));

		// Calculate the difference in days
		auto duration = tpEnd - tpStart;
		return std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24;
	}
	void setid(int a) {
		ID = a;
	}
	void set_sd(string b) {
		Start_date = b;
	}
	void set_ad(string c) {
		Return_date = c;
	}
	void seta(bool f) {
		Active = f;
	}
	int getid() {
		return ID;
	}
	string get_sd() {
		return Start_date;
	}
	string get_ad() {
		return Return_date;
	}
	int get_n() {
		return N_of_days;
	}

	
	bool geta() {
		return Active;
	}

	int getDailyRateFromDB(int ID, sql::Connection* con) {
		return 0;
	}

};
Employee::Employee(int a,string pw,int rec, string b, string c, bool f,int h,bool i, sql::Connection* con) {
	ID = a;
	Employee_Record = rec;
	userpw = pw;
	Fine_Due = h; Damage_caused = i;
	Start_date = b;
	Return_date = c; N_of_days = calculateDateDifference(b,c);

	Active = f;	
}

class Manager : public User {
public:
	Manager(int id,string pw);
	void addCustomer(sql::Connection* con);
	void updateCustomer(sql::Connection* con);
	void deleteCustomer(sql::Connection* con);
	void addEmployee(sql::Connection* con);
	void updateEmployee(sql::Connection* con);
	void deleteEmployee(sql::Connection* con);
	void addCar(sql::Connection* con);
	void updateCar(sql::Connection* con);
	void deleteCar(sql::Connection* con);
	void show_all_cars(sql::Connection* con);
	void show_all_Customer(sql::Connection* con);
	void show_all_Employee(sql::Connection* con);
	void modify_records(sql::Connection* con1, sql::Connection* con2, sql::Connection* con3) {
		{
			int choice;

			cout << "Manager is modifying records." << std::endl;
			cout << "1. Add Customer\n2. Update Customer\n3. Delete Customer\n";
			cout << "4. Add Employee\n5. Update Employee\n6. Delete Employee\n";
			cout << "7. Add Cars\n8. Update Cars\n9. Delete Cars\n10. Show all Cars\n";
			cout << "11.Show all Customer\n12. Show all Employee\n";
			cout << "Enter your choice: ";
			cin >> choice;
			if (choice == 1) addCustomer(con2);
			if (choice == 2) updateCustomer(con2);
			if (choice == 3) deleteCustomer(con2);
			if (choice == 4) addEmployee(con3);
			if (choice == 5) updateEmployee(con3);
			if (choice == 6) deleteEmployee(con3);
			if (choice == 7) addCar(con1);
			if (choice == 8) updateCar(con1);
			if (choice == 9) deleteCar(con1);
			if (choice == 10) show_all_cars(con1);
			if (choice == 11) show_all_Customer(con2);
			if (choice == 12) show_all_Employee(con3);
			

		}
	}
	void setpw(string pw) {
		Password = pw;
	}
	void setid(int id) {
		ID = id;
	}
	int getid() const {
		return ID;
	}
	string getpw() const {
		return Password;
	}
	string getenc() {
		string encryptedPW = encrypt(Password, shift);
		return encryptedPW;
	}

};
Manager::Manager(int id, string pw) {
	ID = id;
	Password = pw;
}
void Manager::show_all_Employee(sql::Connection* con) {
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* result = stmt->executeQuery("SELECT * FROM RENT2");

		if (!result->next()) {
			cout << "No records found in the RENT2 table." << endl;
		}
		else {
		
			sql::ResultSetMetaData* meta = result->getMetaData();
			int columnCount = meta->getColumnCount();
			cout << "Column Names" << endl;
			for (int i = 1; i <= columnCount; ++i) {
				cout << meta->getColumnName(i) << " ";
			}
			cout << endl;

			do {
				// Print data
				for (int i = 1; i <= columnCount; ++i) {
					cout << result->getString(i) << ", ";
				}
				cout << endl;
			} while (result->next());
		}

		delete result;
		delete stmt;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
	}


}
void Manager::show_all_Customer(sql::Connection* con) {
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* result = stmt->executeQuery("SELECT * FROM RENT");

		if (!result->next()) {
			cout << "No records found in the RENT table." << endl;
		}
		else {
			// Print column names and indices
			sql::ResultSetMetaData* meta = result->getMetaData();
			int columnCount = meta->getColumnCount();
			cout << "Column Names" << endl;
			for (int i = 1; i <= columnCount; ++i) {
				cout << meta->getColumnName(i)<< " ";
			}
			cout << endl;

			do {
				// Print data
				for (int i = 1; i <= columnCount; ++i) {
					cout<< result->getString(i) << ", ";
				}
				cout << endl;
			} while (result->next());
		}

		delete result;
		delete stmt;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
	}

}



void Manager::show_all_cars(sql::Connection* con) {
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* result = stmt->executeQuery("SELECT * FROM car_table");

		if (!result->next()) {
			cout << "No records found in the car table." << endl;
		}
		else {
			sql::ResultSetMetaData* meta = result->getMetaData();
			int columnCount = meta->getColumnCount();
			cout << "Column Names" << endl;
			for (int i = 1; i <= columnCount; ++i) {
				cout << meta->getColumnName(i) << " ";
			}
			cout << endl;

			do {
				// Print data
				for (int i = 1; i <= columnCount; ++i) {
					cout << result->getString(i) << ", ";
				}
				cout << endl;
			} while (result->next());
		}

		delete result;
		delete stmt;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
	}

}
void Manager::addCar(sql::Connection* con) {
		try {
			int id,dr;
			bool a;
			string md,cd;
			cout << "Enter Vehicle_Id: " << endl;
			cin >> id;
			cout << "Enter Model: " << endl;
			cin >> md;
			cout << "Enter Condition(good/bad): " << endl;
			cin >> cd;
			cout << "Enter daily_rate: " << endl;
			cin >> dr;
			cout << "Enter Availability: " << endl;
			cin >> a;
		sql::Statement* stmt = con->createStatement();
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO Car_table (Vehicle_ID,Model,Condition_Car,Daily_Rate,Available) VALUES (?,?,?,?,?)");
		pstmt->setInt(1, id);
		pstmt->setString(2,md);
		pstmt->setString(3, cd);
		pstmt->setInt(4, dr);
		pstmt->setBoolean(5,a);
		pstmt->execute();

        cout << "Car added successfully!" << std::endl;
        delete stmt;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
	}
}
void Manager::updateCar(sql::Connection* con) {
	try {
		int id, n;
		cout << "Enter Vehicle_ID: " << endl;
		cin >> id;
		cout << "Enter (1) to Update Model" << endl;
		cout << "Enter (2) to Update Condition" << endl;
		cout << "Enter (3) to Update daily_rate" << endl;
		cout << "Enter (4) to Update Availability" << endl;
		cin >> n;
		sql::PreparedStatement* pstmt;
		if (n == 1) {
			string m;
			cout << "Enter new Model: ";
			cin >> m;
			pstmt = con->prepareStatement("UPDATE Car_table SET Model = ? WHERE ID = ?");
			pstmt->setString(1, m);
			pstmt->setInt(2, id);
			pstmt->execute();

		}
		else if (n == 2) {
			string cd;
			cout << "Enter new Condition(good/bad): ";
			cin >> cd;
			pstmt = con->prepareStatement("UPDATE Car_table SET Condition = ? WHERE ID = ?");
			
			pstmt->setString(1, cd);
			pstmt->setInt(2, id);
			pstmt->execute();
		}
		else if (n == 3) {
			int dr;
			cout << "Enter new daily_rate" << endl;
			cin >> dr;
			pstmt = con->prepareStatement("UPDATE Car_table SET Daily_Rate = ? WHERE ID = ?");
			pstmt->setInt(1, dr);
			pstmt->setInt(2, id);
			pstmt->execute();

		}
		else if (n == 4) {
			bool a;
			cout << "Enter new Availability(true/false)" << endl;
			cin >> a;
			pstmt = con->prepareStatement("UPDATE Car_table SET Available = ? WHERE ID = ?");
			pstmt->setBoolean(1, a);
			pstmt->setInt(2, id);
			pstmt->execute();
		}

	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
	}

}
void Manager::deleteCar(sql::Connection* con) {
	try {
		int Id;
		cout << "Enter Vehicle_ID: " << endl;
		cin >> Id;

		sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM Car_table WHERE ID = ?");
		pstmt->setInt(1,Id);
		pstmt->execute();
		delete pstmt;

		cout << "Car entries deleted successfully!" << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "SQL Exception: " << e.what() << endl;
	}
}
void Manager::updateEmployee(sql::Connection* con) {
	try {
		int id, n;
		cout << "Enter Employee_ID: " << endl;
		cin >> id;
		cout << "Enter (0) to update PassWord" << endl;
		cout << "Enter (1) to Update Employee_Record" << endl;
		cout << "Enter (2) to Update Vehicle_ID" << endl;
		cout << "Enter (3) to Update Start_date" << endl;
		cout << "Enter (4) to Update Return_date" << endl;
		cout << "Enter (5) to Update N_of_days" << endl;
		cout << "Enter (6) to Update Active status" << endl;
		cout << "Enter (7) to Update Fine_due" << endl;
		cout << "Enter (8) to Update Damage_caused" << endl;
		cout << "Enter (9) to clear_dues" << endl;


		cin >> n;
		sql::PreparedStatement* pstmt;
		if (n == 9) {
			int y = 0;
			pstmt = con->prepareStatement("UPDATE RENT SET Fine_Due = ? WHERE ID = ?");
	
			pstmt->setInt(1, y);
			pstmt->setInt(2, id);
			pstmt->execute();
			cout << "Dues Cleared";
		}
		else if (n == 0) {
			string pw;
			cout << "Enter new Password: ";
			cin >> pw;
			string encryptedPW = encrypt(pw, shift);
			pstmt = con->prepareStatement("UPDATE Employee_password SET PW = ? WHERE ID = ?");
			pstmt->setString(1,encryptedPW);
			pstmt->setInt(2, id);

			pstmt->execute();
		}
		else if (n == 1) {
			int Record;
			cout << "Enter new Employee_Record: ";
			cin >> Record;
			pstmt = con->prepareStatement("UPDATE Employee_table SET Employee_Record = ? WHERE ID = ?");
			pstmt->setInt(1, Record);
			pstmt->setInt(2, id);
			pstmt->execute();
		}
		else if (n == 2) {
			int vd;
			cout << "Enter new vehicle_id: ";
			cin >> vd;
			pstmt = con->prepareStatement("UPDATE RENT2 SET Vehicle_ID = ? WHERE ID = ?");
			pstmt->setInt(1, vd);
			pstmt->setInt(2, id);
			pstmt->execute();
		}

		else if (n == 3) {
			string startDate;
			cout << "Enter new Start_date (YYYY-MM-DD): ";
			cin >> startDate;
			pstmt = con->prepareStatement("UPDATE RENT2 SET Return_date = ? WHERE ID = ?)");
			pstmt->setString(1, startDate);
			pstmt->setInt(2, id);
		}
		else if (n == 4) {
			string returnDate;
			cout << "Enter new return_date (YYYY-MM-DD): ";
			cin >> returnDate;
			pstmt = con->prepareStatement("UPDATE RENT2 SET N_of_days = ? WHERE ID = ?)");
			pstmt->setString(1, returnDate);
			pstmt->setInt(2, id);
		}
		else if (n == 5) {
			int nod;
			cout << "Enter new N_of_days ";
			cin >> nod;
			pstmt = con->prepareStatement("UPDATE RENT2 SET Start_date = ? WHERE ID = ?)");
			pstmt->setInt(1, nod);
			pstmt->setInt(2, id);
		}
		else if (n == 6) {
			bool a;
			cout << "Enter new Active status: ";
			cin >> a;
			pstmt = con->prepareStatement("UPDATE RENT2 SET Active = ? WHERE ID = ?)");
			pstmt->setBoolean(1, a);
			pstmt->setInt(2, id);
		}
		else if (n == 7) {
			int y;
			cout << "Enter new Fine_due: ";
			cin >> y;
			pstmt = con->prepareStatement("UPDATE RENT2 SET Fine_Due = ? WHERE ID = ?)");
			pstmt->setInt(1, y);
			pstmt->setInt(2, id);
		}
		else if (n == 8) {
			bool x;
			cout << "Enter Damage_caused(true/false): ";
			cin >> x;
			pstmt = con->prepareStatement("UPDATE RENT2 SET Damage_caused = ? WHERE ID = ?)");
			pstmt->setBoolean(1, x);
			pstmt->setInt(2, id);
		}
	}
	catch (sql::SQLException& e) {
		cerr << "SQL Exception: " << e.what() << endl;
	}

}

void Manager::updateCustomer(sql::Connection* con) {
	try {
		int id, n;
		cout << "Enter Customer_ID: " << endl;
		cin >> id;
		cout << "Enter (0) to Update PassWord" << endl;
		cout << "Enter (1) to Update Customer_Record" << endl;
		cout << "Enter (2) to Update Vehicle_ID" << endl;
		cout << "Enter (3) to Update Start_date" << endl;
		cout << "Enter (4) to Update Return_date" << endl;
		cout << "Enter (5) to Update N_of_days" << endl;
		cout << "Enter (6) to Update Active status" << endl;
		cout << "Enter (7) to Update Fine_due" << endl;
		cout << "Enter (8) to Update Damage_caused" << endl;
		cout << "Enter (9) to clear_dues" << endl;
		cin >> n;
		sql::PreparedStatement* pstmt;
		 
		if (n == 9) {
			int y=0;
			pstmt = con->prepareStatement("UPDATE RENT SET Fine_Due = ? WHERE ID = ?");
			pstmt->setInt(1, y);
			pstmt->setInt(2, id);
			pstmt->execute();
			cout << "Dues Cleared";
        }
		else if (n == 0) {
			string pw;
			cout << "Enter new Password: ";
			cin >> pw;
			string encryptedPW = encrypt(pw, shift);
			pstmt = con->prepareStatement("UPDATE Customer_password SET PW = ? WHERE ID = ?");
			pstmt->setString(1, encryptedPW);
			pstmt->setInt(2, id);
			pstmt->execute();

		}
		else if(n==1){
			int customerRecord;
			cout << "Enter new Customer_Record: ";
			cin >> customerRecord;
			pstmt = con->prepareStatement("UPDATE Customer_table SET Customer_Record = ? WHERE ID = ?");
			pstmt->setInt(1, customerRecord);
			pstmt->setInt(2, id);
			pstmt->execute();
		}
		else if(n==2){
			  int vd;
			  cout << "Enter new vehicle_id: ";
			  cin >> vd;
			  pstmt = con->prepareStatement("UPDATE RENT SET Vehicle_ID = ? WHERE ID = ?");
			  pstmt->setInt(1, vd);
			  pstmt->setInt(2, id);
			  pstmt->execute();
		}

		else if (n == 3) {
			string startDate;
			cout << "Enter new Start_date (YYYY-MM-DD): ";
			cin >> startDate;
			pstmt = con->prepareStatement("UPDATE RENT SET Return_date = ? WHERE ID = ?)");
			pstmt->setString(1, startDate);
			pstmt->setInt(2, id);
		}
		else if(n==4){
			string returnDate;
			cout << "Enter new return_date (YYYY-MM-DD): ";
			cin >> returnDate;
			pstmt = con->prepareStatement("UPDATE RENT SET N_of_days = ? WHERE ID = ?)");
			pstmt->setString(1, returnDate);
			pstmt->setInt(2, id);
		}
		else if(n==5){
			int nod;
			cout << "Enter new N_of_days ";
			cin >> nod;
			pstmt = con->prepareStatement("UPDATE RENT SET Start_date = ? WHERE ID = ?)");
			pstmt->setInt(1, nod);
			pstmt->setInt(2, id);
		}
	
		else if (n == 6) {
			bool a;
			cout << "Enter new Active status: ";
			cin >> a;
			pstmt = con->prepareStatement("UPDATE RENT SET Active = ? WHERE ID = ?)");
			pstmt->setBoolean(1, a);
			pstmt->setInt(2, id);
		}
		else if(n==7){
			int y;
			cout << "Enter new Fine_due: ";
			cin >> y;
			pstmt = con->prepareStatement("UPDATE RENT SET Fine_Due = ? WHERE ID = ?)");
			pstmt->setInt(1, y);
			pstmt->setInt(2, id);
		}
		else if (n == 8) {
			bool x;
			cout << "Enter Damage_caused(true/false): ";
			cin >> x;
			pstmt = con->prepareStatement("UPDATE RENT SET Damage_caused = ? WHERE ID = ?)");
			pstmt->setBoolean(1, x);
			pstmt->setInt(2, id);
		}
	}
	catch (sql::SQLException& e) {
		cerr << "SQL Exception: " << e.what() << endl;
	}

}
void Manager::addCustomer(sql::Connection* con) {
	try {
		int customerRecord;
		int id;
		cout << "Enter Customer ID: " << endl;
		cin >> id;

		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* result = stmt->executeQuery("SELECT AVG(Customer_Record) FROM Customer_table");
		result->next(); 
		double average = result->getDouble(1);

		customerRecord = static_cast<int>(average);
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO Customer_table (ID, Customer_Record) VALUES (?, ?)");
		pstmt->setInt(1, id);
		pstmt->setInt(2, customerRecord);
		pstmt->execute();
		string cp;
		cout << "Enter Customer_password" << endl;
		cin >> cp;
		string encryptedPW = encrypt(cp, shift);
		pstmt = con->prepareStatement("INSERT INTO Customer_password(Id,PW) VALUES(?,?)");
		pstmt->setInt(1, id);
		pstmt->setString(2, encryptedPW);
		pstmt->execute();

		pstmt = con->prepareStatement("INSERT INTO RENT (ID, Vehicle_ID, Start_date, Return_date, N_of_days, Amount_due, Active, Fine_Due, Damage_caused) VALUES (?,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)");
		pstmt->setInt(1, id);
		pstmt->execute();

        cout << "Customer added successfully!" << std::endl;
        delete stmt;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
	}
}
void Manager::addEmployee(sql::Connection* con) {
	try {
		int employerRecord;
		int id;
		cout << "Enter Customer ID: " << endl;
		cin >> id;
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* result = stmt->executeQuery("SELECT AVG(Employee_Record) FROM Employee_table");

		result->next();
		double average = result->getDouble(1);

		employerRecord = static_cast<int>(average);
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO Employee_table (ID, Employee_Record) VALUES (?, ?)");
		pstmt->setInt(1, id);
		pstmt->setInt(2, employerRecord);
		pstmt->execute();

		pstmt = con->prepareStatement("INSERT INTO RENT2 (ID, Vehicle_ID, Start_date, Return_date, N_of_days, Amount_due, Active, Fine_Due, Damage_caused) VALUES (?,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)");
		pstmt->setInt(1, id);
		pstmt->execute();
		string cp;
		cout << "Enter Employee_password" << endl;
		cin >> cp;
		string encryptedPW = encrypt(cp, shift);
		pstmt = con->prepareStatement("INSERT INTO Employee_password(Id,PW) VALUES(?,?)");
		pstmt->setInt(1, id);
		pstmt->setString(2, encryptedPW);
		pstmt->execute();

		cout << "Employer added successfully!" << std::endl;
		delete stmt;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
	}
}
void Manager::deleteCustomer(sql::Connection* con) {
    try {
        int customerId;
        cout << "Enter Customer_ID: " << endl;
        cin >> customerId;

  
        sql::PreparedStatement* pstmtRent = con->prepareStatement("DELETE FROM db2.RENT WHERE ID = ?");
        pstmtRent->setInt(1, customerId);
        pstmtRent->execute();
        delete pstmtRent;

        sql::PreparedStatement* pstmtCustomer = con->prepareStatement("DELETE FROM Customer_table WHERE ID = ?");
        pstmtCustomer->setInt(1, customerId);
        pstmtCustomer->execute();
        delete pstmtCustomer;

		sql::PreparedStatement* pstmtPW = con->prepareStatement("DELETE FROM Customer_password WHERE ID = ?");
		pstmtPW->setInt(1, customerId);
		pstmtPW->execute();
		delete pstmtPW;

        cout << "Customer deleted successfully!" << endl;
    } catch (sql::SQLException& e) {
        cerr << "SQL Exception: " << e.what() << endl;
    }
}
void Manager::deleteEmployee(sql::Connection* con) {
	try {
		int Id;
		cout << "Enter Employee_ID: " << endl;
		cin >> Id;
		sql::PreparedStatement* pstmtRent = con->prepareStatement("DELETE FROM db3.RENT2 WHERE ID = ?");
		pstmtRent->setInt(1, Id);
		pstmtRent->execute();
		delete pstmtRent;

		sql::PreparedStatement* pstmtEmployee = con->prepareStatement("DELETE FROM Employee_table WHERE ID = ?");
		pstmtEmployee->setInt(1, Id);
		pstmtEmployee->execute();
		delete pstmtEmployee;

		sql::PreparedStatement* pstmtPW = con->prepareStatement("DELETE FROM Employee_password WHERE ID = ?");
		pstmtPW->setInt(1,Id);
		pstmtPW->execute();
		delete pstmtPW;

		cout << "Employee deleted successfully!" << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "SQL Exception: " << e.what() << endl;
	}
}

class Car {
private:
	string Model, Condition_Car;
	bool Available;
	int Daily_Rate;
	int Vehicle_ID;
public:
	Car(int vehicle_id,string model, string condition_car, int daily_rate,bool available);
	void setModel(string a) {
	     Model = a;
	}
	void setCond(string b) {
		Condition_Car = b;
	}
	void setdlr(int c) {
		Daily_Rate = c ;
	}
	void setvi(int d) {
	     Vehicle_ID = d;
	}
	void seta(int e) {
		 Available = e;
	}
	string getModel() {
		return Model;
	}
	string getCond() {
		return Condition_Car;
	}
	float dlr() {
		return Daily_Rate;
	}
	int getvi() {
		return Vehicle_ID;
	}
	bool geta() {
		return Available;
	}
};
Car::Car(int vehicle_id, string model, string condition_car, int daily_rate, bool available) {
	Model = model;
	Vehicle_ID = vehicle_id;
	Condition_Car = condition_car;
	Daily_Rate = daily_rate;
	Available = available;
}



bool doesIdExist(sql::Connection* con, int id, const std::string& role) {
	try {
		sql::Statement* stmt = con->createStatement();
		string query;
		if (role == "Manager") {
			query = "SELECT * FROM " + role + "_password WHERE ID = " + std::to_string(id);
		}

		else  query = "SELECT * FROM " + role + "_table WHERE ID = " + std::to_string(id);
		sql::ResultSet* result = stmt->executeQuery(query);

		// Check if the result set has at least one row (ID exists)
		bool idExists = result->next();

		// Clean up resources
		delete result;
		delete stmt;

		// Return whether the ID exists in the table
		return idExists;
	}
	catch (sql::SQLException& e) {
		// Handle SQL exceptions (print an error message and return false)
		std::cerr << "SQL Exception: " << e.what() << std::endl;
		return false;
	}
}


int calculateAverage(sql::Connection* con, const std::string& feature,const string& role) {
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* result = stmt->executeQuery("SELECT AVG(" + feature + ") " +role +"_table");

		result->next(); // Move to the first row
		double average = result->getDouble(1);

		delete result;
		delete stmt;

		return static_cast<int>(average);
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
		exit(1);
	}
}


void displayAvailableCars(sql::Connection* con) {
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* result = stmt->executeQuery("SELECT * FROM car_table WHERE Available = true");

		int numFields = result->getMetaData()->getColumnCount();

		for (int i = 1; i <= numFields; ++i) {
			std::cout << result->getMetaData()->getColumnLabel(i) << "\t";
		}
		std::cout << std::endl;

		while (result->next()) {
			for (int i = 1; i <= numFields; ++i) {
				std::cout << result->getString(i) << "\t";
			}
			std::cout << std::endl;
		}

		delete result;
		delete stmt;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
		exit(1);
	}
}
bool areAllCarsUnavailable(sql::Connection* con) {
	try {
		sql::Statement* stmt = con->createStatement();

		// Count unavailable cars
		sql::ResultSet* result = stmt->executeQuery("SELECT COUNT(*) FROM car_table WHERE Available = false");
		result->next();
		int unavailableCount = result->getInt(1);
		delete result;

		// Get total number of rows in the table
		result = stmt->executeQuery("SELECT COUNT(*) FROM car_table");
		result->next();
		int totalCount = result->getInt(1);
		delete result;

		delete stmt;

		return (unavailableCount > 0) && (unavailableCount == totalCount);
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
		exit(1);
	}
}

void displayColumnForUser(sql::Connection* con, int userId, const std::string& role) {
	try {
		sql::Statement* stmt = con->createStatement();
		std::string query;

		// Use a prepared statement to handle integer parameters
		sql::PreparedStatement* pstmt = nullptr;

		if (role == "Customer") {
			query = "SELECT RENT.Vehicle_ID, db1.car_table.Model FROM RENT JOIN db1.car_table ON RENT.Vehicle_ID = db1.car_table.Vehicle_ID WHERE RENT.ID = ?";
			pstmt = con->prepareStatement(query);
			pstmt->setInt(1, userId);
		}
		else {
			query = "SELECT RENT2.Vehicle_ID, db1.car_table.Model FROM RENT2 JOIN db1.car_table ON RENT2.Vehicle_ID = db1.car_table.Vehicle_ID WHERE RENT2.ID = ?";
			pstmt = con->prepareStatement(query);
			pstmt->setInt(1, userId);
		}

		sql::ResultSet* result = pstmt->executeQuery();

		// Fetch and display results
		while (result->next()) {
			std::cout << "Vehicle ID: " << result->getInt("Vehicle_ID") << ", Model: " << result->getString("Model") << std::endl;
		}

		delete result;
		delete pstmt;
		delete stmt;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
		exit(1);
	}
}




int giveColumnForUser(sql::Connection* con, int userId, const string& role) {
	try {
		sql::Statement* stmt = con->createStatement();
		string query;

		
		sql::PreparedStatement* pstmt = nullptr;

		if (role == "Customer") {
			query = "SELECT Customer_Record FROM " + role + "_table WHERE ID = ?";
			pstmt = con->prepareStatement(query);
			pstmt->setInt(1, userId);
		}
		else {
			query = "SELECT Employee_Record FROM " + role + "_table WHERE ID = ?";
			pstmt = con->prepareStatement(query);
			pstmt->setInt(1, userId);
		}

		sql::ResultSet* result = pstmt->executeQuery();

		int columnData = 0;  // Assuming default value is 0

		if (result->next()) {
			columnData = result->getInt("Customer_Record");  // or "Employee_Record" based on the role
		}
		delete result;
		delete pstmt;
		delete stmt;

		return columnData;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL Exception: " << e.what() << std::endl;
		exit(1);
	}
}


int main() {
	try {
		sql::Driver* driver;
		sql::Connection* con1;
		sql::Connection* con2;
		sql::Connection* con3;
		sql::Statement* stmt1;
		sql::Statement* stmt2;
		sql::Statement* stmt3;
		sql::PreparedStatement* pstmt1;
		sql::PreparedStatement* pstmt2;
		sql::PreparedStatement* pstmt3;
		driver = get_driver_instance();
		con1 = driver->connect(server, username, password);
		con2 = driver->connect(server, username, password);
		con3 = driver->connect(server, username, password);
		stmt1 = con1->createStatement();
		//stmt1->execute("CREATE DATABASE IF NOT EXISTS db1");
	
		stmt2 = con2->createStatement();
		//stmt2->execute("CREATE DATABASE IF NOT EXISTS db2");
		
		stmt3 = con3->createStatement();
		//stmt3->execute("CREATE DATABASE IF NOT EXISTS db3");
		stmt3->execute("DROP DATABASE IF EXISTS db3");
		stmt2->execute("DROP DATABASE IF EXISTS db2");
		stmt1->execute("DROP DATABASE IF EXISTS db1");
		stmt1->execute("CREATE DATABASE IF NOT EXISTS db1");
		con1->setSchema("db1");

		stmt1->execute("DROP TABLE IF EXISTS car_table");
		stmt1->execute("CREATE TABLE IF NOT EXISTS car_table (Vehicle_ID Int,Model VARCHAR(255), Condition_Car VARCHAR(255),Daily_Rate INT,Available BOOLEAN)");
		Car m(1,"compass", "good", 5000, true);
		Car n(2,"zl", "good", 2000, true);
		Car b(3,"z5", "bad", 2300, false);
		Car v(4,"x5", "good", 9000, false);
		Car c(5,"i9", "bad", 4000, true);
		pstmt1 = con1->prepareStatement("INSERT INTO car_table (Vehicle_ID, Model, Condition_Car, Daily_Rate, Available) VALUES (?, ?, ?, ?,?)");
		pstmt1->setInt(1, m.getvi());
		pstmt1->setString(2, m.getModel());
		pstmt1->setString(3, m.getCond());
		pstmt1->setInt(4, m.dlr());
		pstmt1->setBoolean(5, m.geta());
		pstmt1->execute();

		pstmt1->setInt(1, n.getvi());
		pstmt1->setString(2, n.getModel());
		pstmt1->setString(3, n.getCond());
		pstmt1->setInt(4, n.dlr());
		pstmt1->setBoolean(5, n.geta());
		pstmt1->execute();

		pstmt1->setInt(1, b.getvi());
		pstmt1->setString(2, b.getModel());
		pstmt1->setString(3, b.getCond());
		pstmt1->setInt(4, b.dlr());
		pstmt1->setBoolean(5, b.geta());
		pstmt1->execute();

		pstmt1->setInt(1, v.getvi());
		pstmt1->setString(2, v.getModel());
		pstmt1->setString(3, v.getCond());
		pstmt1->setInt(4, v.dlr());
		pstmt1->setBoolean(5, v.geta());
		pstmt1->execute();
		
		pstmt1->setInt(1, c.getvi());
		pstmt1->setString(2, c.getModel());
		pstmt1->setString(3, c.getCond());
		pstmt1->setInt(4, c.dlr());
		pstmt1->setBoolean(5, c.geta());
		pstmt1->execute();
		stmt2->execute("CREATE DATABASE IF NOT EXISTS db2");
		con2->setSchema("db2");

		stmt2->execute("DROP TABLE IF EXISTS Customer_table");
		stmt2->execute("CREATE TABLE IF NOT EXISTS Customer_table (ID INT,Customer_Record INT, PRIMARY KEY(ID))");
		stmt2->execute("DROP TABLE IF EXISTS Customer_password");
		stmt2->execute("CREATE TABLE IF NOT EXISTS Customer_password (ID INT, PW VARCHAR(255), PRIMARY KEY(ID))");
        stmt2->execute("DROP TABLE IF EXISTS RENT");
		stmt2->execute("CREATE TABLE IF NOT EXISTS RENT(ID INT, Vehicle_ID Int, Start_date DATE, Return_date DATE, N_of_days INT, Active BOOLEAN,Fine_Due INT,Damage_caused BOOLEAN, FOREIGN KEY(ID) REFERENCES Customer_table(ID))");
		Customer c1(1,"yyfq", 3, "2024-02-01", "2024-02-6", false, 0, false,con1, con2);
		Customer c2(2,"yuyv", 4, "2024-02-03", "2024-02-8", false, 0, true,con1, con2);
		Customer c3(3,"oppw", 2, "2024-03-1", "2024-03-2", false, 0, false,con1, con2);
		Customer c4(4,"jppo", 4, "2024-02-28", "2024-03-3", true, 1000, true,con1, con2);
		Customer c5(5,"pqop", 1, "2024-01-03", "2024-01-18", false, 0, true,con1, con2);
		pstmt2 = con2->prepareStatement("INSERT INTO Customer_password (ID, PW) VALUES (?,?)");
		pstmt2->setInt(1, c1.getid());
		pstmt2->setString(2, c1.getenc());
		pstmt2->execute();
		pstmt2->setInt(1, c2.getid());
		pstmt2->setString(2, c2.getenc());
		pstmt2->execute();
		pstmt2->setInt(1, c3.getid());
		pstmt2->setString(2, c3.getenc());
		pstmt2->execute();
		pstmt2->setInt(1, c4.getid());
		pstmt2->setString(2, c4.getenc());
		pstmt2->execute();
		pstmt2->setInt(1, c5.getid());
		pstmt2->setString(2, c5.getenc());
		pstmt2->execute();
		pstmt2 = con2->prepareStatement("INSERT INTO Customer_table(ID,Customer_Record) VALUES(?,?)");
		pstmt2->setInt(1, c1.getid());
		pstmt2->setInt(2, c1.getrec());
		pstmt2->execute();
		pstmt2->setInt(1, c2.getid());
		pstmt2->setInt(2, c2.getrec());
		pstmt2->execute();
		pstmt2->setInt(1, c3.getid());
		pstmt2->setInt(2, c3.getrec());
		pstmt2->execute();
		pstmt2->setInt(1, c4.getid());
		pstmt2->setInt(2, c4.getrec());
		pstmt2->execute();
		pstmt2->setInt(1, c5.getid());
		pstmt2->setInt(2, c5.getrec());
		pstmt2->execute();
		

		pstmt2 = con2->prepareStatement("INSERT INTO RENT(ID, Vehicle_ID, Start_date, Return_date, N_of_days, Active,Fine_Due,Damage_Caused) VALUES (?, ?, ?, ?, ?,?,?,?)");
		pstmt2->setInt(1, c1.getid());
		pstmt2->setInt(2, m.getvi());
		pstmt2->setString(3, c1.get_sd());
		pstmt2->setString(4, c1.get_ad());
		pstmt2->setInt(5, c1.get_n());
		pstmt2->setBoolean(6, c1.geta());
		pstmt2->setInt(7, c1.getfd());
		pstmt2->setBoolean(8, c1.getdc());

		pstmt2->execute();

		pstmt2->setInt(1, c2.getid());
		pstmt2->setInt(2, n.getvi());
		pstmt2->setString(3, c2.get_sd());
		pstmt2->setString(4, c2.get_ad());
		pstmt2->setInt(5, c2.get_n());
		pstmt2->setBoolean(6, c2.geta());
		pstmt2->setInt(7, c2.getfd());
		pstmt2->setBoolean(8, c2.getdc());
		pstmt2->execute();

		pstmt2->setInt(1, c3.getid());
		pstmt2->setInt(2, b.getvi());
		pstmt2->setString(3, c3.get_sd());
		pstmt2->setString(4, c3.get_ad());
		pstmt2->setInt(5, c3.get_n());
		pstmt2->setBoolean(6, c3.geta());
		pstmt2->setInt(7, c3.getfd());
		pstmt2->setBoolean(8, c3.getdc());
		pstmt2->execute();

		pstmt2->setInt(1, c4.getid());
		pstmt2->setInt(2, v.getvi());
		pstmt2->setString(3, c4.get_sd());
		pstmt2->setString(4, c4.get_ad());
		pstmt2->setInt(5, c4.get_n());
		pstmt2->setBoolean(6, c4.geta());
		pstmt2->setInt(7, c4.getfd());
		pstmt2->setBoolean(8, c4.getdc());
		pstmt2->execute();
		
		pstmt2->setInt(1, c5.getid());
		pstmt2->setInt(2, c.getvi());
		pstmt2->setString(3, c5.get_sd());
		pstmt2->setString(4, c5.get_ad());
		pstmt2->setInt(5, c5.get_n());
		pstmt2->setBoolean(6, c5.geta());
		pstmt2->setInt(7, c5.getfd());
		pstmt2->setBoolean(8, c5.getdc());
		pstmt2->execute();
		
		stmt3->execute("CREATE DATABASE IF NOT EXISTS db3");
		con3->setSchema("db3");
		stmt3->execute("DROP TABLE IF EXISTS Employee_table");
		stmt3->execute("CREATE TABLE IF NOT EXISTS Employee_table (ID INT,Employee_Record INT, PRIMARY KEY(ID))");
	
		stmt3->execute("DROP TABLE IF EXISTS Employee_password");
		stmt3->execute("CREATE TABLE IF NOT EXISTS Employee_password (ID INT, PW VARCHAR(255), PRIMARY KEY(ID))");
		stmt3->execute("DROP TABLE IF EXISTS Manager_password");
		stmt3->execute("CREATE TABLE IF NOT EXISTS Manager_password (ID INT,PW VARCHAR(255))");
		Manager ma(1, "oljio");
		pstmt3 = con3->prepareStatement("INSERT INTO Manager_password (ID, PW) VALUES (?,?)");
		pstmt3->setInt(1, ma.getid());
		pstmt3->setString(2, ma.getenc());
		pstmt3->execute();

		stmt3->execute("DROP TABLE IF EXISTS RENT2");
		stmt3->execute("CREATE TABLE IF NOT EXISTS RENT2(ID INT, Vehicle_ID Int, Start_date DATE, Return_date DATE, N_of_days INT, Active BOOLEAN,Fine_Due INT,Damage_caused BOOLEAN , FOREIGN KEY(ID) REFERENCES Employee_table(ID))");
		Employee e1(1,"hvea", 3, "2024-02-01", "2024-02-6", false, 300, false, con3);
		Employee e2(2,"efde", 1, "2024-02-03", "2024-02-8", false, 0, true, con3);
		Employee e3(3,"esqe", 4, "2024-02-13", "2024-02-28", true, 0, false, con3);
		Employee e4(4,"tyiq", 6, "2024-02-1", "2024-02-3", false, 1000, true, con3);
		Employee e5(5,"tqoi", 3, "2024-01-03", "2024-01-18", false, 700, false, con3);
		pstmt3= con3->prepareStatement("INSERT INTO Employee_password (ID, PW) VALUES (?,?)");
		
	    pstmt3->setInt(1, e1.getid());
		pstmt3->setString(2,e1.getenc());
		pstmt3->execute();
		pstmt3->setInt(1, e2.getid());
		pstmt3->setString(2,e2.getenc());
		pstmt3->execute();
		pstmt3->setInt(1, e3.getid());
		pstmt3->setString(2,e3.getenc());
		pstmt3->execute();
		pstmt3->setInt(1, e4.getid());
		pstmt3->setString(2,e4.getenc());
		pstmt3->execute();
		pstmt3->setInt(1, e5.getid());
		pstmt3->setString(2, e5.getenc());
		pstmt3->execute();
		pstmt3 = con3->prepareStatement("INSERT INTO Employee_table(ID,Employee_Record) VALUES(?,?)");
		pstmt3->setInt(1, e1.getid());
		pstmt3->setInt(2, e1.getrec());
		pstmt3->execute();
		pstmt3->setInt(1, e2.getid());
		pstmt3->setInt(2, e2.getrec());
		pstmt3->execute();
		pstmt3->setInt(1, e3.getid());
		pstmt3->setInt(2, e3.getrec());
		pstmt3->execute();
		pstmt3->setInt(1, e4.getid());
		pstmt3->setInt(2, e4.getrec());
		pstmt3->execute();
		pstmt3->setInt(1, e5.getid());
		pstmt3->setInt(2, e5.getrec());
		pstmt3->execute();
		


		pstmt3 = con3->prepareStatement("INSERT INTO RENT2(ID, Vehicle_ID, Start_date, Return_date, N_of_days,Active,Fine_Due,Damage_caused) VALUES (?, ?, ?,?,  ?, ?,?,?)");
		pstmt3->setInt(1, e1.getid());
		pstmt3->setInt(2, m.getvi());
		pstmt3->setString(3, e1.get_sd());
		pstmt3->setString(4, e1.get_ad());
		pstmt3->setInt(5, e1.get_n());
		
		pstmt3->setBoolean(6, e1.geta());
		pstmt3->setInt(7, e5.getfd());
		pstmt3->setBoolean(8, e5.getdc());
		pstmt3->execute();

		pstmt3->setInt(1, e2.getid());
		pstmt3->setInt(2, n.getvi());
		pstmt3->setString(3, e2.get_sd());
		pstmt3->setString(4, e2.get_ad());
		pstmt3->setInt(5, e2.get_n());
		
		pstmt3->setBoolean(6, e2.geta());
		pstmt3->setInt(7, e5.getfd());
		pstmt3->setBoolean(8, e5.getdc());
		pstmt3->execute();

		pstmt3->setInt(1, e3.getid());
		pstmt3->setInt(2, b.getvi());
		pstmt3->setString(3, e3.get_sd());
		pstmt3->setString(4, e3.get_ad());
		pstmt3->setInt(5, e3.get_n());
	
		pstmt3->setBoolean(6, e3.geta());
		pstmt3->setInt(7, e5.getfd());
		pstmt3->setBoolean(8, e5.getdc());
		pstmt3->execute();

		pstmt3->setInt(1, e4.getid());
		pstmt3->setInt(2, v.getvi());
		pstmt3->setString(3, e4.get_sd());
		pstmt3->setString(4, e4.get_ad());
		pstmt3->setInt(5, e4.get_n());
	
		pstmt3->setBoolean(6, e4.geta());
		pstmt3->setInt(7, e5.getfd());
		pstmt3->setBoolean(8, e5.getdc());
		pstmt3->execute();

		pstmt3->setInt(1, e5.getid());
		pstmt3->setInt(2, c.getvi());
		pstmt3->setString(3, e5.get_sd());
		pstmt3->setString(4, e5.get_ad());
		pstmt3->setInt(5, e5.get_n());
	
		pstmt3->setBoolean(6, e5.geta());
		pstmt3->setInt(7, e5.getfd());
		pstmt3->setBoolean(8, e5.getdc());
		pstmt3->execute();


		bool exit = false;
		while (!exit) {
			cout << "*************************" << endl;
			cout << "****CAR RENTAL SYSTEM****" << endl;
			cout << "*************************" << endl;
			cout << "1. Customer:" << endl;
			cout << "2. Employee:" << endl;
			cout << "3. Manager:" << endl;
			cout << "Enter your role:" << endl;

			int val;
			cin >> val;
			string role;
			if (val == 1 || val ==2) {

				if (val == 1) {
				role = "Customer";
				}
				else role = "Employee";
				string name;
				int id;
				cout << "Enter your name: ";
				cin >> name;
				cout << "Enter your id: ";
				cin >> id;

				// Check if the ID exists in the respective table
				if (role == "Customer") {
					if (doesIdExist(con2, id, role)) {
						string pw;
						cout << "Enter Your password: ";
						cin >> pw;
						string getDB = DBpw(con2, id,role);
						string decryptedPW;
						if (!getDB.empty()) decryptedPW = decrypt(getDB, shift);

							if (decryptedPW == pw) {
								cout << "Welcome" << endl;
							}
							else {
								cout << "Incorrect Password. Try Again!" << endl;
								exit = true;
								break;
								
							}
					}
					else {
						cout << "You are not a registered user , Please ask the manager to add you" << endl;
						exit = true;
						break;
					}
				}
				else {
					if (doesIdExist(con3, id, role)) {
						string pw;
						cout << "Enter Your password: ";
						cin >> pw;
						string getDB = DBpw(con3, id, role);
						string decryptedPW;
						if (!getDB.empty()) decryptedPW = decrypt(getDB, shift);

						if (decryptedPW == pw) {
							cout << "Welcome" << endl;
						}
						else {
							cout << "Incorrect Password. Try Again!" << endl;
							exit = true;
							break;

						}
					}
					else {
						cout << "You are not a registered user , Please ask the manager to add you" << endl;
						exit = true;
						break;

					}
				}




				int ans;
				cout << "Type 1 to see available cars:" << endl;
				cout << "Type 2 to see cars currently rented:" << endl;
				cout << "Type 3 to rent a car from available cars:" << endl;
				cin >> ans;

				if (ans == 1) {
					displayAvailableCars(con1);
				}
				else if (ans == 2) {if ( role == "Customer") displayColumnForUser(con2, id,role);
				else displayColumnForUser(con3, id, role);
				}
				else if (ans == 3) {
					int a_c = 3;
					int y;
					if (role == "Customer") y = giveColumnForUser(con2, id, role);
					else y = giveColumnForUser(con3, id, role);
					if (!areAllCarsUnavailable(con1)) {
						if (y < a_c) {
							cout << "You cannot rent car" << endl;
						}
						else {
							cout << "Can rent a maximum of " << a_c << " cars" << endl;
						}
					}
					else {
						cout << "Cars not available to rent" << endl;
					}
				}
			}
			else if (val == 3) {
				string name;
				cout << "Enter your name: " << endl;
				cin >> name;
				int id;
				cout << "Enter you id: " << endl;
				cin >> id;
			
				string role = "Manager";
				if (doesIdExist(con3, id, role)) {
					string pw;
					cout << "Enter Your password: ";
					cin >> pw;
					string getDB = DBpw(con3, id, role);
					string decryptedPW;
					if (!getDB.empty()) decryptedPW = decrypt(getDB, shift);

					if (decryptedPW == pw) {
						cout << "Welcome" << endl;
						Manager manager(id, pw);
						manager.modify_records(con1, con2, con3);
					}
					else {
						cout << "Incorrect Password. Try Again!" << endl;
						exit = true;
						break;

					}
				}
				else {
					cout << "You are not registered" << endl;
					exit = true;
					break;

				}
			
				}

	
		}

		delete con1;
		delete con2;
		delete con3;
	}
	catch (sql::SQLException& e) {
		cerr << "SQL Exception: " << e.what() << endl;

	}
	return 0;
	}
