#include <mysql/mysql.h> // Заголовочный файл с описание функций
#include <stdio.h>

int mysql(const char query[])
{
	MYSQL mysql; // Дескриптор соединения
	MYSQL_ROW row; // Массив полей текущей строки
	MYSQL_RES *res; // Дескриптор результирующей таблицы

	char host[] = "127.0.0.1"; // хост
	char user[] = "root"; // пользователь
	char passwd[] = "passwd"; // пароль
	char db[] = "database"; // название базы данных
	int port = 0; //  порт. Если порт у сервера MySQL не по умолчанию (3306), то нужно указывать конкретный номер порта

	mysql_init(&mysql);  // Инициализация
	mysql_real_connect(&mysql, host, user, passwd, db, port, NULL, 0); // соединение

	if (mysql_query(&mysql, query) > 0) // запрос. Если ошибок нет, то продолжаем работу
        {
			
			// Если была ошибка, ...
			printf("%s", mysql_error(&mysql));  // ... вывдем ее
			return 1; // и завершим работу
        } 

 
	mysql_close(&mysql); // Закрываем соединение
	return 0;
}

int main()
{
	int result = mysql("select * from users");
	printf("%d\n", result);
	return 0;
}
