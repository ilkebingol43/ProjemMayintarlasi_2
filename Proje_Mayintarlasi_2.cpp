// Proje_Mayintarlasi_2.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//
#include <SFML/Graphics.hpp> // Proje için gerekli olan kütüphane 
#include <iostream>   // c++ main kütüphanesi
#include <optional>   //
#include <SFML/Window.hpp> // 
using namespace std;
using namespace sf;  // sf::Angle angle1 = sf::degrees(180); gibi kod karmaşalırnı engeller
#include <chrono>  // c++ kendi zaman kütüpnahanesi
#include <SFML/Window/Event.hpp> // olay tiplerini (KeyPressed, MouseMoved, vs.) kullanabilmek için.



int main()
{
    RenderWindow window;
    window.create(VideoMode({ 800,640 }), "Mayin Tarlasi");


    Texture texture;
    if (!texture.loadFromFile("pngegg.png")) {
        cerr << "Hata: pngegg.png yuklenemedi! Dosya adini ve konumunu kontrol edin." << endl;
    }

    Sprite sprite(texture);
    

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is < Event::Closed>()) {
                window.close();
            }
            window.clear(Color::White);

            window.draw(sprite);

            window.display();

        }
    }



    return 0 ; 
}


