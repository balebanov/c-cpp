#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

/*
	Программа составляет рандомное расписание для 10 класса на одну неделю,
	исходя из количества предметов и часов
*/

class Subjects{
public:
	string Name;
	int HoursPerWeek;

	void SetName(string SubjName) {
		Name = SubjName;
	}

	void SetHours(const int SubjHours){
		HoursPerWeek = SubjHours;
	}

	void DisplayInfo() {
		cout << Name << " - " << HoursPerWeek << endl;
	}
};

void DisplayHello() {
	//приветствие
	cout << "Программа составления расписания для 10 класса" << endl;
	cout << "Denis Balebanov (c)2015" << endl << endl;
}

int main() {
	DisplayHello();

	//объявляем переменные-предметы
	Subjects Bio, Algebra, Rus, Social, Pe, German, English, Geometry, Liter, History, Geography, Chemestry, Physics, Technology;

	Bio.SetName("Биология");
	Bio.SetHours(2);

	Algebra.SetName("Алгебра");
	Algebra.SetHours(4);

	Rus.SetName("Русский");
	Rus.SetHours(2);

	Social.SetName("Обществознание");
	Social.SetHours(1);

	Pe.SetName("Физ-ра");
	Pe.SetHours(3);

	German.SetName("Немецкий");
	German.SetHours(2);

	English.SetName("Английский");
	English.SetHours(3);

	Geometry.SetName("Геометрия");
	Geometry.SetHours(2);

	Liter.SetName("Литература");
	Liter.SetHours(3);

	History.SetName("История");
	History.SetHours(2);

	Geography.SetName("География");
	Geography.SetHours(2);

	Chemestry.SetName("Химия");
	Chemestry.SetHours(2);

	Physics.SetName("Физика");
	Physics.SetHours(2);

	Technology.SetName("ИКТ");
	Technology.SetHours(2);

	//создаем массив предметов
	Subjects subj_array[] = {
		Bio, 
		Algebra, 
		Rus, 
		Social, 
		Pe, 
		German, 
		English, 
		Geometry, 
		Liter, 
		History, 
		Geography, 
		Chemestry, 
		Physics, 
		Technology //14 предметов
	};
	int sizeof_subj = sizeof(subj_array) / sizeof(subj_array[0]); //количество предметов (в случае добавления/удаления)

	Subjects *Monday = new Subjects[8]; //понедельник
	Subjects *Tuesday = new Subjects[8]; //вторник
	Subjects *Wednesday = new Subjects[8]; //среда
	Subjects *Thursday = new Subjects[8]; //четверг
	Subjects *Friday = new Subjects[8]; //пятница

	Subjects *arr_days[] = { //неделя
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday
	};

	for (int DAY = 0; DAY < 5; DAY++){
		for (int SUBJ = 0; SUBJ < 8; SUBJ++){
			//if (subj_array[rand() % 13 + 0].HoursPerWeek)
			arr_days[DAY][SUBJ] = subj_array[rand() % 13 + 0];
		}
	}

	cout << "***Понедельник***" << endl;
	for (int i = 0; i < 8; i++){
		cout << Monday[i].Name << endl;
	}
	cout << endl;

	cout << "***Вторник***" << endl;
	for (int i = 0; i < 8; i++){
		cout << Tuesday[i].Name << endl;
	}
	cout << endl;

	cout << "***Среда***" << endl;
	for (int i = 0; i < 8; i++){
		cout << Wednesday[i].Name << endl;
	}
	cout << endl;

	cout << "***Четверг***" << endl;
	for (int i = 0; i < 8; i++){
		cout << Thursday[i].Name << endl;
	}
	cout << endl;

	cout << "***Пятница***" << endl;
	for (int i = 0; i < 8; i++){
		cout << Friday[i].Name << endl;
	}

	cout << endl << "Конец программы. Спасибо за использование!" << endl;
	return 0;
}