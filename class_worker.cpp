#include <iostream>
#include <string>
using namespace std;

class Worker{
public:
	string name;
	unsigned int experience;
	unsigned int hour_salary;
	unsigned int hours;

	void setWorker(string str, int exp, int sal, int hrs){
		this->name = str;
		this->experience = exp;
		this->hour_salary = sal;
		this->hours = hrs;
		setSalary();
		setPremy();
	}

	void showInfo(){
		cout << "Имя: " << this->name << endl;
		cout << "Стаж (лет): " << this->experience << endl;
		cout << "Часовая зарплата: " << this->hour_salary << endl;
		cout << "Отработал часов: " << this->hours << endl;
		cout << "Зарплата: " << this->salary << endl;
		cout << "Премия: " << this->premy << endl << endl;
	}

private:
	unsigned int salary;
	double premy;

	void setSalary(){
		this->salary = this->hour_salary * this->hours;
	}

	void setPremy(){
		if (this->experience <= 1) premy = 0.0;
		else if(this->experience > 1 && this->experience <= 3) premy = (double)salary * 1.05;
		else if(this->experience > 3 && this->experience <= 5) premy = (double)salary * 1.08;
		else if(this->experience > 5) premy = (double)salary * 1.15;
	}
};

int main(int argc, char const *argv[])
{
	//Worker worker;
	Worker *worker = new Worker;
	string str;
	unsigned int exp, sal, hrs;

	cout << "Введите имя: ";
	cin >> str;
	cout << "Введите стаж: ";
	cin >> exp;
	cout << "Введите часовую зарплату: ";
	cin >> sal;
	cout << "Введите число отработанных часов: ";
	cin >> hrs;

	//worker.setWorker(str, exp, sal, hrs);
	worker->setWorker(str, exp, sal, hrs);
	cout << endl;

	//worker.showInfo();
	worker->showInfo();
	delete worker;

	return 0;
}