#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
using namespace std;

class Game {
public:
    int Height;
    int Energy;

    void InitPl(int newHeight) {
        Height = newHeight;
    }

    int JumpNextPl(int Height1, int Height2, int energy){
        Energy = energy - abs(Height1 - Height2);
        cout << "Перепрыгнули на следующую платформу! " << endl;
        energy = Energy;
        return energy;
    }

    int JumpThroughPl(int Height1, int Height2, int energy){
        Energy = energy - 2*abs(Height1 - Height2);
        cout << "Перепрыгнули через одну платформу! " << endl;
        return Energy;
    }
};

void StartText() {
    cout << "Добро пожаловать в игру!" << endl;
    cout << "Правила следующие: " << endl;
    cout << "1. Сначала вводите начальное кол-во энергии игрока" << endl;
    cout << "2. Введите диапазон высот, т.е максимальную высоту, на которой может находиться платформа" << endl;
    cout << "3. Игра начинается. Вы можете либо перепрыгнуть на следующую платформу, либо через одну" << endl;
    cout << "4. Игра заканчивается, если кол-во энергии стало меньше или равно 0, или если вы нажали 3" << endl;
    cout << "Вот и все! Если вы готовы играть, нажмите Enter. Удачи!" << endl;

    int key = 0;
    key = getchar();
    while(key == EOF); //ожидание ввода Enter
}

int main() {
    int range = 0; //диапазон
    int energyStart = 0; //начальная энергия
    int option = 0; //выбор действия
    int newEnergy1 = 0; //новая энергия (сделал, чтобы не запутаться)

    srand(time(0));

    StartText();

    cout << "Введите начальное кол-во энергии: ";
    cin >> energyStart;
    if(energyStart < 0){ //если введено неправильное кол-во энергии
        while(energyStart <= 0){
            cout << "Ошибка! Энергия должна быть положительной! Введите энергию: ";
            cin >> energyStart;
        }
    }

    cout << "Введите диапазон высот: ";
    cin >> range;
    if(range < 0){ //если введен неправильный диапазон
        while(range <= 0){
            cout << "Ошибка! Диапазон должен быть положительным! Введите диапазон: ";
            cin >> range;
        }
    }
    
    cout << "1 - перепрыгнуть на следующую\n2 - перепрыгнуть через одну\n3 - выход из программы\n" << endl;

    newEnergy1 = energyStart;
    while (option != 3) { //пока не нажат выход
        Game *Platform1 = new Game;
        Platform1->InitPl(rand() % range + 1);
        Game *Platform2 = new Game;
        Platform2->InitPl(rand() % range + 1);

        cin >> option; //выбор действия
        if (option == 1){
            cout << "Кол-во энергии: " << Platform1->JumpNextPl(Platform1->Height, Platform2->Height, newEnergy1) << endl;
            newEnergy1 = Platform1->Energy;
        }
        else if (option == 2){
            cout << "Кол-во энергии: " << Platform1->JumpThroughPl(Platform1->Height, Platform2->Height, newEnergy1) << endl;
            newEnergy1 = Platform1->Energy; 
        }
        else if (option == 3){
            cout << "Спасибо за игру!" << endl;
            delete Platform1;
            delete Platform2;
            exit(0);
        }
        else{
            cout << "Ошибка!!" << endl;
            cout << "Введите число еще раз!" << endl;
        }

        if(newEnergy1 <= 0){ //если заканчивается энергия
            cout << "Энергия закончилась! Спасибо за игру!" << endl;
            delete Platform1;
            delete Platform2;
            exit(0);
        }

        //удаляем платформы, дабы избежать проблем с памятью
        delete Platform1;
        delete Platform2; 
    }

  return 0;
}