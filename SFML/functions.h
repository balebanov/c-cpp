void check_resized(sf::Event event){
	if(event.type == sf::Event::Resized){
		cout << "new width: " << event.size.width << endl;
		cout << "new height: " << event.size.height << endl;
	}
}

void if_text_entered(sf::Event event){
	if(event.type == sf::Event::TextEntered){
		if(event.text.unicode < 128) cout << "Typed: " << static_cast<char>(event.text.unicode) << endl;
	}
}

void if_window_closed(sf::Event event){
	if (event.type == sf::Event::Closed) window.close();
}

void if_key_pressed(sf::Event event){
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		cout << "Нажата левая стрелка!" << endl;
	}
}