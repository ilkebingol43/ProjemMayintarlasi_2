#include <SFML/Graphics.hpp>    
#include <iostream>   // c++ main kütüphanesi
#include <optional>   //
#include <SFML/Window.hpp>;
using namespace std;
using namespace sf;  // sf::Angle angle1 = sf::degrees(180); gibi kod karmaşalırnı engeller
#include <chrono>  // c++ kendi zaman kütüpnahanesi
#include <SFML/Window/Event.hpp> // olay tiplerini (KeyPressed, MouseMoved, vs.) kullanabilmek için.
#include <SFML/Audio.hpp>
#include <filesystem>
#include <algorithm> // min ve max fonksiyonları için


enum class gameState {  // kontrolcü çakışmasını engellemek için ve hangi ekrandayken hangi tuş ne işe yarayacak sorunun çözmek için enum class
    girisEkrani,         
    oyunEkrani,
    cıkısEkrani,
};


void volumeChange(Keyboard::Key basilanTus, SoundSource& sesKaynagi);
void ekranCiz(RenderWindow& window, Sprite jpg); //1. Prototipi referans olacak şekilde ayarlıyoruz
int main()
{
    RenderWindow window; // ekran nesnesi yaratıldı
    window.create(VideoMode({ 800,640 }), "Mayin Tarlasi"); // 800 e 640lık mayın Tarlasi isminde pencere 

    // giriş ekranı
    Texture firstScreen("Jpg/jgirisEkrani.jpg"); // Oyun için giriş ekranı
    if (!firstScreen.loadFromFile("Jpg/jgirisEkrani.jpg")) { // png dosyası yüklendi  //GirisEkrani olmadı
        cout << "GirisEkrani Yüklenemedi";  // hata                                  // c++ ta /G escape sequencetir
        return -1;
    }
    Sprite girisEkrani(firstScreen);
    girisEkrani.setScale(Vector2f(800.0f / 2816.0f, 640.0f / 1536.0f)); // 2820*1532 olarak aldığımız dosyayı // parametre olarak oran alır 800.f / 2816.f anlamı bu fotorafı yüzde 28 küçült
    

    Texture exit;
    if (!exit.loadFromFile("Jpg/jCikisEkrani.jpg")) {
        cout << "CıkısEkrani yüklenemedi" << endl ;
        return - 1 ; // pencere açılmayacak
    }
    Sprite cikisEkrani(exit);
    cikisEkrani.setScale(Vector2f(800.0f / 1600.0f, 640.0f / 872.0f));
    
    

    //Level Ekranı
    Texture levelScreen("Jpg/jSeviye.jpg");
    if (!levelScreen.loadFromFile("Jpg/jSeviye.jpg")) {
        cout << "SeviyeEkrani Yüklenemedi" << endl;;
        return -1;
    }
    Sprite seviyeEkrani(levelScreen);
    seviyeEkrani.setScale(Vector2f(800.0f / 1375.0f, 640.0f / 768.0f)); // seviye Ekranı  pencereye göre ayarlandı // eğer f koyulmazsa 640 /768 yaklaşık 0.20 dir burdada 0 olarak kabul edilir ve siyah olur 

    //--------------------------------------------------
    //Çalacak Müzüik
    Music girisMuzigi; // oyun açıldığında çalacak olam müzik
    if (!girisMuzigi.openFromFile("Ses/sGirisMuzigi.mp3")) { // eğer dosyaya ulaşamaz ise false döndürüp programı kapatır
        cout << "Error" << endl;  // hata 
        return -1;
    }


    // bomba sesi
    SoundBuffer tiklmamaMuzigi("Ses/stiklamaSesi.wav"); // bombaSesi.wav buffere 
    if (!tiklmamaMuzigi.loadFromFile("Ses/stiklamaSesi.wav")) {  // eğer dosyaya ulaşamaz ise false döndürüp programı kapatır 
        cout << "Error" << endl;
    }
    Sound tiklamaSesi(tiklmamaMuzigi);

    tiklamaSesi.setVolume(50.f); // tıklama sesi azaltma 




    girisMuzigi.setLooping(true);  // girisEkranını Müzik ayarları
    girisMuzigi.setVolume(50);     // girisEkranın SesSeviyesi
    girisMuzigi.setPitch(0.75f);   // girisEkranın sesFrekansı
    girisMuzigi.play(); // müzik eklendi  // 


    ekranCiz(window, girisEkrani); // eğer windowun yanına & koyarsak window nesnenin bellek adresi göndermiş oluruz

    //window.clear(); 
    //window.draw(girisEkrani);                       // program çalışmaya başladı an çıkıcak olan ekran
    //window.display();

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is <Event::Closed>()) {
                window.close(); // pencere kapandı ;

            }
            //-----------------------------------
            if (const auto* basilanTus = event->getIf <Event::KeyPressed>()) {
                volumeChange(basilanTus->code, girisMuzigi); // basılacak tuşun kendisi gönderiyoruz
                volumeChange(basilanTus->code, tiklamaSesi); //
            }
            // --------------------------------------------
            if (const auto* mouseClick = event->getIf < Event::MouseButtonPressed>()) {  // mouse sol tıkına basıldığında bomba sesi çıkar
                if (mouseClick->button == Mouse::Button::Left) {
                    tiklamaSesi.play();

                }
            }
            //if (Keyboard::isKeyPressed(Keyboard::Key::Enter)) {
            //    ekranCiz(window, seviyeEkrani);
            //}
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
                ekranCiz(window,cikisEkrani);
      
            }

        }// Sart kontrol



    }// ekranı sürekli açık tutan döngü



    return 0;
}// end of main


void ekranCiz(RenderWindow& window, Sprite jpg) { // nesneler referanslar ile fonksiyonlara bildirilir 
    // & operatörü bize direk nesnenin kendisi ile uğraşma imkanı sunar

    window.clear();
    window.draw(jpg);
    window.display();
}// end of function

void volumeChange(Keyboard::Key basilanTus, SoundSource& sesKaynagi) {
    
    if (basilanTus == Keyboard::Key::F1) {
        sesKaynagi.pause(); // şarkıyı o andan itibaren dondurur play komutu geldiğinde o andan itbaren başlaıtr 

    }// ses kapatma şartı

    if (basilanTus == Keyboard::Key::F3) {
        if (sesKaynagi.getStatus() != SoundSource::Status::Playing) {
            sesKaynagi.play();

        }
        float mevcutSes = sesKaynagi.getVolume();

        if (mevcutSes < 100.0f) {
            sesKaynagi.setVolume(min(mevcutSes + 5.0f, 100.0f));
            cout << "Ses artirildi" << endl;
        }
        else {
            cout << "Ses 100 den buyuk olamaz" << endl; // maximum 100 olabilir
        }

    } // ses artırma şartı  son

    if (basilanTus == Keyboard::Key::F2)
    {
        if (sesKaynagi.getStatus() != SoundSource::Status::Playing) { // pause yada stopta ise 0 döndürecek 
            sesKaynagi.play(); //

        }
        float mevcutSes = sesKaynagi.getVolume();                 

        if (mevcutSes > 0.0f) {
            sesKaynagi.setVolume(max(mevcutSes - 5.0f,0.0f));
            cout << "Ses azaltildi" << endl;
        }
        else {
            cout << "Ses 0 dan kucuk olamaz" << endl; // minumum 100 olabilir
        } // ses azaltma son 


    }
}// end of function 
