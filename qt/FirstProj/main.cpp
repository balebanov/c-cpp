#include <QApplication>
#include <QLabel>
int main ( int lArg, char * lArgv [ ] )
{
//Создаём объект QApplication, который инициализирует и настраивает оконную программу,
//управляет её выполнением с помощью цикла обработки событий
    QApplication lApplication ( lArg, lArgv );
    QLabel lLabel; //Создаём виджет QLabel — метка
    lLabel.setText ( " I am widget! " ); //Задаём текст для метки
    lLabel.setGeometry ( 200, 200, 300, 150 );
//Задаём размеры — позицию (x, y) ширину и высоту. Задаём выравнивание текста
    lLabel.setAlignment (Qt::AlignHCenter | Qt::AlignVCenter );
//Класс QFont используют для настройки параметров шрифта.
//Выбираем семейство шрифтов Arial Black и размер 12.
    QFont lBlackFont ( " Arial Black ", 12 );
    lLabel.setFont ( lBlackFont ); //Задаём шрифт для метки
    lLabel.show ( ); //Вызываем метод show() для показа метки на экране.
    return lApplication.exec ( ); //Запускаем программу на выполнение exec() выполняет
//цикл обработки событий. Программа ожидает действия пользователя и выполняет их обработку.
}

