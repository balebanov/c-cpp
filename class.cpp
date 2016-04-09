#include <iostream>
#include <string>
using namespace std;

class Human
{
private:
	string Name;
	int Age;

public:
	void SetName (string HumansName) {
		Name = HumansName;
	}

	void SetAge (int HumansAge) {
		Age = HumansAge;
	}

	void IntroduceSelf() {
		cout << "I am " + Name << " and am ";
		cout << Age << " years old" << endl;
	}

};

int main() {
	Human FirstMan;
	FirstMan.SetName("Den");
	FirstMan.SetAge(16);

	Human FirstWoman;
	FirstWoman.SetName("Dasha");
	FirstWoman.SetAge(16);

	FirstMan.IntroduceSelf();
	FirstWoman.IntroduceSelf();
}
