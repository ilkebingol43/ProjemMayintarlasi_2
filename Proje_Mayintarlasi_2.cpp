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



void ekranCiz(RenderWindow &window, Sprite jpg); //1. Prototipi referans olacak şekilde ayarlıyoruz
int main()
{
    RenderWindow window; // ekran nesnesi yaratıldı
    window.create(VideoMode({ 800,640 }), "Mayin Tarlasi"); // 800 e 640lık mayın Tarlasi isminde pencere 

    // giriş ekranı
    Texture firstScreen("Jpg/jgirisEkrani.jpg"); // Oyun için giriş ekranı
    if (!firstScreen.loadFromFile("Jpg/jgirisEkrani.jpg")) { // png dosyası yüklendi  //GirisEkrani olmadı
        cout << "GirisEkrani Yüklenemedi";  // hata                                  // c++ ta /G escape sequencetir
     }
    Sprite girisEkrani(firstScreen);
    

    //Level Ekranı
    Texture levelScreen("Jpg/jSeviye.jpg");
    if (!levelScreen.loadFromFile("Jpg/jSeviye.jpg")) {
        cout << "SeviyeEkrani Yüklenemedi" << endl;;
        return -1;
    }
    Sprite seviyeEkrani(levelScreen);
    
     //Çalacak Müzüik
    Music girisMuzigi ; // oyun açıldığında çalacak olam müzik
    if (!girisMuzigi.openFromFile("Ses/sGirisMuzigi.mp3")) { // eğer dosyaya ulaşamaz ise false döndürüp programı kapatır
        cout << "Error" << endl;  // hata 
        return -1;
    }

    // bomba sesi
    SoundBuffer bombaMuzigi("Ses/sbombaSesi.wav"); // bombaSesi.wav buffere 
    if (!bombaMuzigi.loadFromFile("Ses/sbombaSesi.wav")) {  // eğer dosyaya ulaşamaz ise false döndürüp programı kapatır 
        cout << "Error" << endl;
    }
    Sound bombaPatlama(bombaMuzigi);




    girisMuzigi.setLooping(true);  // girisEkranını Müzik ayarları
    girisMuzigi.setVolume(50);     // girisEkranın SesSeviyesi
    girisMuzigi.setPitch(0.75f);   // girisEkranın sesFrekansı
    girisMuzigi.play(); // müzik eklendi 
    
    
    ekranCiz(window,girisEkrani); // eğer windowun yanına & koyarsak window nesnenin bellek adresi göndermiş oluruz

    //window.clear(); 
    //window.draw(girisEkrani);                       // program çalışmaya başladı an çıkıcak olan ekran
    //window.display();

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
                ekranCiz(window, seviyeEkrani);
            }
        }
        
          
        
    }
    


   
    



    return 0 ; 
}

void ekranCiz(RenderWindow &window ,Sprite jpg) { // nesneler referanslar ile fonksiyonlara bildirilir 
                                                  // & operatörü bize direk nesnenin kendisi ile uğraşma imkanı sunar
    
    window.clear();
    window.draw(jpg);
    window.display();
}

