#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
using namespace std;

int main()
{
    //Открытие окна
    sf::RenderWindow window(sf::VideoMode(760, 475), "SFML window");
    sf::Image image;
    image.loadFromFile("texture.png");

    //Создание текстуры
    sf::Texture texture;
    if(!texture.loadFromImage(image)){
        return EXIT_FAILURE;
    }

    //Создание спрайта
    sf::Sprite sprite;
    sprite.setTexture(texture);
    while (window.isOpen())
    {
        //Обработчик событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            //Закрытие окна
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //Очистка экрана
        window.clear();
        //Отображение спрайта
        window.draw(sprite);
        //Обновление окна
        window.display();
    }
    return EXIT_SUCCESS;
}