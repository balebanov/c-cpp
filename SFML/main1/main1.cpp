#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
using namespace std;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Resized){
                cout << "new width: " << event.size.width << endl;
                cout << "new height: " << event.size.height << endl;
            }

            if(event.type == sf::Event::TextEntered){
                if(event.text.unicode < 128) cout << "Typed: " << static_cast<char>(event.text.unicode) << endl;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                cout << "Нажата левая стрелка!" << endl;
            }
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}