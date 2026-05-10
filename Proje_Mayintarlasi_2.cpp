
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


//typedef struct {
//    bool bombaVarmi = false;
//    bool bayrakVarmi = false ;
//    bool acıldıMı = false;
//    int komşuMayınSayisi;
//
//
//
//}hucre;

enum class GameState {  // kontrolcü çakışmasını engellemek için ve hangi ekrandayken hangi tuş ne işe yarayacak sorunun çözmek için enum class
    girisEkrani,         
    oyunEkrani,
    levelEkrani,
    cıkısEkrani,
    duraklamaEkrani,
    lvlCıkısEkrani,
};

bool resimYukleVeCiz(Texture& texture, Sprite& sprite, const std::string& dosyaYolu, float Uzunluk, float Genislik);
void volumeChange(Keyboard::Key basilanTus, SoundSource& sesKaynagi);
void ekranCiz(RenderWindow& window, Sprite jpg); //1. Prototipi referans olacak şekilde ayarlıyoruz

int main()
{

    GameState gameState = GameState::girisEkrani; // kod çakışmasını engellemek için  oyun hangi ekranda ise o ekranı kontrol etmeliyiz



    RenderWindow window; // ekran nesnesi yaratıldı
    window.create(VideoMode({ 800,640 }), "Mayin Tarlasi"); // 800 e 640lık mayın Tarlasi isminde pencere 

    //// giriş ekranı
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

    // Sevşye cıkıs Ekrani
    Texture lvlScreenExit("Jpg/jLevelEkranicıkıs.jpg"); // lvl CıkısEkrani
    if (!lvlScreenExit.loadFromFile("Jpg/jLevelEkranicıkıs.jpg")) {
        cout << "SeviyeCıkısEkranı yüklenemedi" << endl;
        return -1;
    }
    Sprite lvlCıkısEkrani(lvlScreenExit);
    lvlCıkısEkrani.setScale(Vector2f(800.0f / 1024.0f , 640.0f / 571.0f));

    // oyunEkrani
    Texture gameScreen;  // oyunEkrani
    if (!gameScreen.loadFromFile("Jpg/joyunEkrani.jpg")) {
        cout << "Oyun Ekranı yüklenemedi" << endl;
        return -1;
    }
    Sprite oyunEkrani(gameScreen);
    oyunEkrani.setScale(Vector2f(800.0f / 1024.0f , 640.0f /572.0f));


    //--------------------------------------------------
    //Çalacak Müzüik

    Music girisMuzigi; // oyun açıldığında çalacak olam müzik
    if (!girisMuzigi.openFromFile("Ses/sGirisMuzigi.mp3")) { // eğer dosyaya ulaşamaz ise false döndürüp programı kapatır
        cout << "Error" << endl;  // hata 
        return -1;
    }
    girisMuzigi.setLooping(true);  // girisEkranını Müzik ayarları
    girisMuzigi.setVolume(50);     // girisEkranın SesSeviyesi
    girisMuzigi.setVolume(50);     // girisEkranın SesSeviyesi
    girisMuzigi.setPitch(0.75f);   // girisEkranın sesFrekansı
    girisMuzigi.play(); // müzik eklendi  // 

    // bomba sesi
    SoundBuffer tiklmamaMuzigi("Ses/stiklamaSesi.wav"); // bombaSesi.wav buffere 
    if (!tiklmamaMuzigi.loadFromFile("Ses/stiklamaSesi.wav")) {  // eğer dosyaya ulaşamaz ise false döndürüp programı kapatır 
        cout << "Error" << endl;
    }
    Sound tiklamaSesi(tiklmamaMuzigi);

    tiklamaSesi.setVolume(50.f); // tıklama sesi azaltma 



     // eğer windowun yanına & koyarsak window nesnenin bellek adresi göndermiş oluruz

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is <Event::Closed>()) {
                window.close(); // pencere kapandı ;

            }
            //----------------------------------- // bu komutlar oynun her durumunda olmalı
            if (const auto* basilanTus = event->getIf <Event::KeyReleased>()) {
                volumeChange(basilanTus->code, girisMuzigi); // basılacak tuşun kendisi gönderiyoruz
                volumeChange(basilanTus->code, tiklamaSesi); //
            }
            // -------------------------------------------- // bu komutlar oynun her durumunda olmalı 
            if (const auto* mouseClick = event->getIf < Event::MouseButtonPressed>()) {  // mouse sol tıkına basıldığında bomba sesi çıkar
                if (mouseClick->button == Mouse::Button::Left) {
                    tiklamaSesi.play(); // tıklama sesi eklendi 

                }
            }//--------------------------------
            if (const  auto* basilanTus = event->getIf <Event::KeyReleased>()) { // basilan tuşu burada yakaladık

                if (basilanTus->scancode == Keyboard::Scancode::Escape) { // scan kod  ile direk klavyedeki yer hedeflenir bu sayede klavye farklılıkları englellenir
                    // oynun hangi ekranda olduğunu kontrol ediyoruz 
                    
                    if (gameState == GameState::cıkısEkrani) {
                        gameState = GameState::girisEkrani; // ekran durumu girisEkrani yapildi
                        ekranCiz(window, girisEkrani); 
                    }
                    else if (gameState == GameState::girisEkrani) {
                        gameState = GameState::cıkısEkrani; // ekran durumu cıkısEkrani yapildi 
                        ekranCiz(window, cikisEkrani);
                    }
                    else if (gameState == GameState::lvlCıkısEkrani) {
                        gameState = GameState::lvlCıkısEkrani;
                        ekranCiz(window, seviyeEkrani);
                    }
                
                } // ekran cıkısEkranı mi yoksa giriş ekranı mı kontrol ediliyor
                if (basilanTus->scancode == Keyboard::Scancode::Enter) {
                    
                    if (gameState == GameState::girisEkrani) {
                        gameState = GameState::levelEkrani;
                        ekranCiz(window, seviyeEkrani);
                    }
                    else if (gameState == GameState::cıkısEkrani) {
                        window.close();
                    }
                    else if (gameState== GameState::lvlCıkısEkrani) {
                        gameState = GameState::girisEkrani;
                        ekranCiz(window, girisEkrani);
                    }
                    else if (gameState == GameState::levelEkrani) {
                        gameState = GameState::oyunEkrani;
                        ekranCiz(window, oyunEkrani);
                    }
                }

               
            }//komutların karışmasını engellemek için koşul şartlar 

        }// Sart kontrol
        
       
        ekranCiz(window, girisEkrani);
    }// ekranı sürekli açık tutan döngü



    return 0;
}// end of main


void ekranCiz(RenderWindow& window, Sprite jpg) { // nesneler referanslar ile fonksiyonlara bildirilir 
    // & operatörü bize direk nesnenin kendisi ile uğraşma imkanı sunar

    window.clear();
    window.draw(jpg);
    window.display();
}// end of function

void volumeChange(Keyboard::Key basilanTus,SoundSource& sesKaynagi) {
    
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
            sesKaynagi.setVolume(max(mevcutSes - 5.0f, 0.0f));
            cout << "Ses azaltildi" << endl;
        }
        else {
            cout << "Ses 0 dan kucuk olamaz" << endl; // minumum 100 olabilir
        } // ses azaltma son 


    }

   
    
}// end of function 


// sonra dönecem
bool resimYukleVeCiz(RenderWindow &window,Texture &texture , Sprite &sprite , const std::string &dosyaYolu , float Uzunluk ,float Genislik) 
{ // eğer dosya yüklenemez ise
    if (!texture.loadFromFile(dosyaYolu)) {
        cout << "Dosya yüklenemedi" << endl;
        return false;
    }

    sprite.setTexture(texture);  // texture sprite nesnesine atandı ;

    Vector2f oran = Vector2f(texture.getSize()); // textureun yükseklik ve genişlik leri alındı
    if (oran.x == 0 && oran.y == 0) {
        cout << "Uzunluk yada genişik 0 olan bir sprite olamaz" << endl;
        return false;
        
    }

    float hedefuzunluk = Uzunluk / (oran.x);  //ekrana göre uyarlandı 
    float hedefgenislik = Genislik / (oran.y);
    
    sprite.setScale(Vector2f(hedefgenislik, hedefuzunluk));
    ekranCiz(window, sprite);

    return true;
}
