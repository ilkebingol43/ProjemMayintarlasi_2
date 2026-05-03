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
#include <SFML/Audio.hpp>
#include <filesystem>


int main()
{
    RenderWindow window; // ekran nesnesi
    window.create(VideoMode({ 800,640 }), "Mayin Tarlasi"); // 800 e 640lık mayın Tarlasi isminde pencere 

    // giriş ekranı
    Texture firstScreen("Jpg\girisEkrani.jpg"); // Oyun için giriş ekranı
    if (!firstScreen.loadFromFile("Jpg\girisEkrani.jpg")) { // png dosyası yüklendi  //GirisEkrani olmadı
        cout << "GirisEkrani Yüklenemedi";  // hata                                  // c++ ta /G escape sequencetir
     }
    Sprite girisEkrani(firstScreen);
    

    //Level Ekranı
    Texture levelScreen("Jpg\Seviye.jpg");
    if (!levelScreen.loadFromFile("Jpg\Seviye.jpg")) {
        cout << "SeviyeEkrani Yüklenemedi" << endl;;
        return -1;
    }
    Sprite seviyeEkrani(levelScreen);
    
     //Çalacak Müzüik
    Music girisMuzigi("Ses\GirisMuzigi.mp3"); // oyun açıldığında çalacak olam müzik
    if (!girisMuzigi.openFromFile("Jpg\GirisMuzigi.mp3")) { // eğer dosyaya ulaşamaz ise false döndürüp programı kapatır
        cout << "Error" << endl;  // hata 
        return -1;
    }

    // bomba sesi
    SoundBuffer bombaMuzigi("Ses\bombaSesi.wav"); // bombaSesi.wav buffere 
    if (!bombaMuzigi.loadFromFile("Ses\bombaSesi.wav")) {  // eğer dosyaya ulaşamaz ise false döndürüp programı kapatır 
        cout << "Error" << endl;
    }
    Sound bombaPatlama(bombaMuzigi);




    girisMuzigi.setLooping(true);  // girisEkranını Müzik ayarları
    girisMuzigi.setVolume(50);     // girisEkranın SesSeviyesi
    girisMuzigi.setPitch(0.75f);   // girisEkranın sesFrekansı
    girisMuzigi.play(); // müzik eklendi 
    



    window.clear(); 
    window.draw(girisEkrani); // program çalışmaya başladı an çıkıcak olan ekran
    window.display();

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is < Event::Closed>()) {  //
                window.close();   // kapatma tuşuna basılınca pencereyi kapatır 
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Backspace)) { // Muziği durdurmak için backspace basmak

                girisMuzigi.stop(); 
            }
            if (const auto*  mouseClick = event->getIf < Event::MouseButtonReleased>()) {  // mouse sol tıkına basıldığında bomba sesi çıkar
                if (mouseClick->button == Mouse::Button::Left) { 
                    bombaPatlama.play();
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Enter)) {
                window.clear();
                window.draw(seviyeEkrani); //kullanıcı enter tuşuna basar ise level ekranı çıkıcak
                window.display();
            }
        }
        
          
        
    }
    


   
    



    return 0 ; 
}


