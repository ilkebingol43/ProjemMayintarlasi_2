
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
#include <cmath> // alanı kontrol etmek içinin
#include <ctime>  // mayınları rastgele yerleştirmek için
#include <cstdlib> // mayınları rastgele yerleştirmek için

struct HucreAyarları
{
    float sutunSayisi;
    float satirSayisi;          // hücrenin kenar uzunluklarını hesaplamak için
    float HucreGenisligi;        // hücre her ne kadar kare gibi olsada görüntüden dolayısıyla pikseller dikdörtgen boyutunda
    float HucreYuksekligi;
    float BaslangıcX;
    float BaslangıcY;  // 1-) hücre ayarları  için struct yapıldı önceden fonksiyon içindeydi  
    float BitisX;
    float BitisY;
    //-----------------------------------
};

 struct Hucre {
    bool bombaVarmi = false ;   // otomatil olarak girilen rastgele değerleri engellemek için 
    bool bayrakVarmi = false ;
    bool acıkMi = false;
    int komsuHucreMayinSayisi = 0 ;
   
};

enum class OyunZorlugu { // oyun zorluğu bu kısımda yapılıyor 
     kolay,
     orta,
     zor,

};
enum class GameState {  // kontrolcü çakışmasını engellemek için ve hangi ekrandayken hangi tuş ne işe yarayacak sorunun çözmek için enum class
    girisEkrani,
    oyunEkrani,
    levelEkrani,
    cıkısEkrani,
    duraklamaEkrani,
    lvlCıkısEkrani,
    gameOverScreen,
};
void bombaYerlestir(vector<vector<Hucre>>& alan, int mayinSayisi);
Vector2i TiklananHucreyiBul(float fareX, float fareY, OyunZorlugu& zorluk);
bool solTiklamayiIsle(vector <vector<Hucre>>& alan, float fareX, float fareY, OyunZorlugu secilenzorluk);
bool resimYukleVeCiz(Texture& texture, Sprite& sprite, const std::string& dosyaYolu, float Uzunluk, float Genislik);
void volumeChange(Keyboard::Key basilanTus, SoundSource& sesKaynagi);
void ekranCiz(RenderWindow& window, Sprite jpg); //1. Prototipi referans olacak şekilde ayarlıyoruz
void ekranıGüncelle(RenderWindow& window,vector <vector<Hucre>>& alan, Sprite acıkKutujpg, OyunZorlugu zorluk, Sprite bombaJpg,Font &font);
int komsuMayınlariSay(vector <vector<Hucre>>& alan, int satir, int sutun);
void komsuMayinlariHesapla(vector<vector<Hucre>>& alan);
int zorlukSeviyesi(vector <vector<Hucre> >& alan, OyunZorlugu secilenSeviye);
HucreAyarları hucreAyarlarıAyarla(OyunZorlugu zorluk);

int main()
{

    

    srand(time(NULL)); // sürekli olarak rastgele yerlere mayın yerleştirecek ;

    GameState gameState = GameState::girisEkrani; // kod çakışmasını engellemek için  oyun hangi ekranda ise o ekranı kontrol etmeliyiz
    OyunZorlugu secilenlevel = OyunZorlugu::zor;  // oynun seviyesi otomatik olarak ayarlandı


    RenderWindow window; // ekran nesnesi yaratıldı
    window.create(VideoMode({ 800,640 }), "Mayin Tarlasi"); // 800 e 640lık mayın Tarlasi isminde pencere 
    vector<vector<Hucre>> oyunTahtasi;
    zorlukSeviyesi(oyunTahtasi, secilenlevel); // vektöre otomatik olarak değer atadık

    Texture openblock;
    if (!openblock.loadFromFile("Jpg/jacikhucre.jpg")) {
        cout << "Error" << endl;

    }
    Sprite acıkHucre(openblock); // hücre boyutunu fonksiyon içinde yapıyoruz 

    // font
    Font font;
    if (!font.openFromFile("Font/MarielleFranco.ttf")) {
        cout << "Error" << endl;
    }

    // bomba jpg
    Texture bomba;
    if (!bomba.loadFromFile("Jpg/jbombajpg.jpg")) {
        cout << "Resim yüklenemedi" << endl;
        return -1;
    }
    Sprite bomb(bomba);

    //// giriş ekranı
    Texture firstScreen("Jpg/jgirisEkrani.jpg"); // Oyun için giriş ekranı
    if (!firstScreen.loadFromFile("Jpg/jgirisEkrani.jpg")) { // png dosyası yüklendi  //GirisEkrani olmadı
        cout << "GirisEkrani Yüklenemedi";  // hata                                  // c++ ta /G escape sequencetir
        return -1;
    }
    Sprite girisEkrani(firstScreen);
    girisEkrani.setScale(Vector2f(800.0f / 1698.0f, 640.0f / 926.0f)); // 2820*1532 olarak aldığımız dosyayı // parametre olarak oran alır 800.f / 2816.f anlamı bu fotorafı yüzde 28 küçült
   
   
    // cıkıs ekranı
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
    seviyeEkrani.setScale(Vector2f(800.0f / 1536.0f, 640.0f / 1024.0f)); // seviye Ekranı  pencereye göre ayarlandı // eğer f koyulmazsa 640 /768 yaklaşık 0.20 dir burdada 0 olarak kabul edilir ve siyah olur 

    // Sevşye cıkıs Ekrani
    Texture lvlScreenExit("Jpg/jLevelEkranicıkıs.jpg"); // lvl CıkısEkrani
    if (!lvlScreenExit.loadFromFile("Jpg/jLevelEkranicıkıs.jpg")) {
        cout << "SeviyeCıkısEkranı yüklenemedi" << endl;
        return -1;
    }
    Sprite lvlCıkısEkrani(lvlScreenExit);
    lvlCıkısEkrani.setScale(Vector2f(800.0f / 1677.0f , 640.0f / 938.0f)) ;

    // oyunEkrani
    Texture gameScreen1;  // oyunEkrani
    if (!gameScreen1.loadFromFile("Jpg/joyunEkraniZorMode.jpg")) {
        cout << "Oyun Ekranı yüklenemedi" << endl;
        return -1;
    }
    Sprite oyunEkraniZor(gameScreen1);
    oyunEkraniZor.setScale(Vector2f(800.0f / 1672.0f , 640.0f /940.0f));

    Texture gameScreen2;
    if (!gameScreen2.loadFromFile("Jpg/joyunEkraniOrtaMode.jpg")) {
        cout << "Oyun Ekranı yüklenemedi" << endl;
        return -1;
    }
    Sprite oyunEkraniOrta(gameScreen2);
    oyunEkraniOrta.setScale(Vector2f(800.0f / 1509.0f, 640.0f / 1042.0f));


    Texture gameScreen3;
    if (!gameScreen3.loadFromFile("Jpg/joyunEkraniKolayMode.jpg")) {
        cout << "Oyun Ekrani yüklenemedi" << endl;
        return -1;
    }
    Sprite oyunEkraniKolay(gameScreen3);
    oyunEkraniKolay.setScale(Vector2f(800.0f / 1536.0f , 640.0f /1024.0f));



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


     ekranCiz(window, girisEkrani);
     // eğer windowun yanına & koyarsak window nesnenin bellek adresi göndermiş oluruz
     bool ilkTıklama = true; // kullanıcı ilk tıklamasında mayına basıp ölmesin diye kontrol koyuyoruz
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

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                // Sadece sol tıka basıldıysa
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    if (gameState == GameState::oyunEkrani) {
                        // Tek satırda bütün işlemi ve hesaplamayı hallettik!
                        Vector2i koordinat = TiklananHucreyiBul(mousePressed->position.x, mousePressed->position.y, secilenlevel);

                        if (koordinat.x != -1 && koordinat.y != -1) { // oynabilir ekranda ise
                            int satir = koordinat.y;
                            int sutun = koordinat.x;

                            if (ilkTıklama == true) {
                                if (secilenlevel == OyunZorlugu::kolay) {
                                    bombaYerlestir(oyunTahtasi, 10); // kolay mod için 10 tane mayın yerleştirdik
                                }
                                if (secilenlevel == OyunZorlugu::orta) {
                                    bombaYerlestir(oyunTahtasi, 20);
                                }
                                if (secilenlevel == OyunZorlugu::zor) {
                                    bombaYerlestir(oyunTahtasi, 40);
                                }
                                ilkTıklama = false;

                                komsuMayinlariHesapla(oyunTahtasi);
                            }
                            bool mayinVarmi = solTiklamayiIsle(oyunTahtasi, mousePressed->position.x, mousePressed->position.y, secilenlevel);
                            if (mayinVarmi == true) { // bu fonksiyon sayesinde karmaşık  spagetti kodlardan kurtulduk //true ise bomba vardemek ve oynu bitirir
                                
                                gameState = GameState::gameOverScreen;
                                // ekranıguncelle  ile buraya gameOverScreen koyucaz ku
                            }
                        }
                       
                    }// kordinat hesaplama **********
                }
            }
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
                    else if (gameState == GameState::levelEkrani) {
                        gameState = GameState::lvlCıkısEkrani;
                        ekranCiz(window,lvlCıkısEkrani);

                    }
                    else if (gameState == GameState::lvlCıkısEkrani) {
                        gameState = GameState::levelEkrani;
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
                    
                }
                 else if (gameState == GameState::levelEkrani) {

                     // 1 tuşuna basılıp çekildiyse (Numpad veya normal rakam)
                     if (basilanTus->scancode == Keyboard::Scancode::Numpad1 || basilanTus->scancode == Keyboard::Scancode::Num1) {
                         gameState = GameState::oyunEkrani;
                         secilenlevel = OyunZorlugu::kolay; // oyun zorluğu kolay seviye atandı
                         ekranCiz(window, oyunEkraniKolay);
                     }
                     // 2 tuşuna basılıp çekildiyse
                     else if (basilanTus->scancode == Keyboard::Scancode::Numpad2 || basilanTus->scancode == Keyboard::Scancode::Num2) {
                         gameState = GameState::oyunEkrani;
                         secilenlevel = OyunZorlugu::orta;
                         ekranCiz(window, oyunEkraniOrta);
                     }
                     // 3 tuşuna basılıp çekildiyse
                     else if (basilanTus->scancode == Keyboard::Scancode::Numpad3 || basilanTus->scancode == Keyboard::Scancode::Num3) {
                         gameState = GameState::oyunEkrani;
                         secilenlevel = OyunZorlugu::zor;
                         ekranCiz(window, oyunEkraniZor);
                     }
                 }

               
            }//komutların karışmasını engellemek için koşul şartlar 

        }// Sart kontrol
        ekranıGüncelle(window, oyunTahtasi, acıkHucre, secilenlevel, bomb, font);
       
        
    }// ekranı sürekli açık tutan döngü



    return 0;
}// end of main


//----------------------------------------------
void ekranıGüncelle(RenderWindow& window,vector <vector<Hucre>>&alan ,Sprite acıkKutujpg ,OyunZorlugu zorluk ,Sprite bombaJpg,Font &font) {
    

    HucreAyarları ayarlar = hucreAyarlarıAyarla(zorluk);

    float orjinalAcikX = acıkKutujpg.getTexture().getSize().x ; // acık hucreyi uyarlıyoruz
    float orjinalAcıkY = acıkKutujpg.getTexture().getSize().y ;

    acıkKutujpg.setScale(Vector2f(ayarlar.HucreGenisligi / orjinalAcikX, ayarlar.HucreYuksekligi / orjinalAcıkY));
         
    float orjinalBombaX = bombaJpg.getTexture().getSize().x;  // bombayı hücreye uyarlıyoruz
    float orjinalBombaY = bombaJpg.getTexture().getSize().y;

    bombaJpg.setScale(Vector2f(ayarlar.HucreGenisligi / orjinalBombaX, ayarlar.HucreYuksekligi / orjinalBombaY));


    for (int satir = 0; satir < alan.size() ; satir++) {
        for (int sutun = 0; sutun < alan[0].size(); sutun++) {

            if (alan[satir][sutun].acıkMi == true) {
                float positionBombOrSafeX = ayarlar.BaslangıcX + (sutun * ayarlar.HucreGenisligi);
                float positionBombOrSafeY = ayarlar.BaslangıcY + (satir * ayarlar.HucreYuksekligi);

                if (alan[satir][sutun].bombaVarmi == true) {
                    bombaJpg.setPosition(Vector2f(positionBombOrSafeX,positionBombOrSafeY));
                    window.draw(bombaJpg);

                }
                else {
                    // 1. Önce arka plana açık kutu görselini (zemin) çiz
                    acıkKutujpg.setPosition(Vector2f(positionBombOrSafeX, positionBombOrSafeY));
                    window.draw(acıkKutujpg);

                    // 2. Hafızadan o hücrenin etrafındaki mayın sayısını çek
                    int mayinSayisi = alan[satir][sutun].komsuHucreMayinSayisi;

                    // 3. Sadece etrafında mayın varsa (1,2,3... ise) ekrana rakam çiz
                    if (mayinSayisi > 0) {
                        Text sayi(font); // Fontu atayarak Text nesnesi oluştur

                        // İçine yazılacak metni int'ten string'e çevir
                        sayi.setString(to_string(mayinSayisi));

                        // Karakter boyutunu hücrenin yüksekliğine göre dinamik ayarla (%70'i kadar)
                        sayi.setCharacterSize(static_cast<int>(ayarlar.HucreYuksekligi * 0.7f));

                        // Yazının rengini siyah yap (Şimdilik)
                        sayi.setFillColor(Color::White);

                        // Yazıyı, kutunun sol üstünden biraz içeriye (ortaya) doğru kaydır
                        float yaziX = positionBombOrSafeX + (ayarlar.HucreGenisligi * 0.25f);
                        float yaziY = positionBombOrSafeY - (ayarlar.HucreYuksekligi * 0.05f); // SFML textleri bazen aşağı kayar, hafif yukarı alıyoruz

                        sayi.setPosition(Vector2f(yaziX, yaziY));

                        // Rakamı en üst katmana (zemin resminin üstüne) çiz
                        window.draw(sayi);
                    }
                }

            }
        }
    }// hücrelerin açık olup olmadığını kontrol ediyoruz 

}// end of function 

//-----------------------------------------------

void ekranCiz(RenderWindow& window, Sprite jpg) { // nesneler referanslar ile fonksiyonlara bildirilir 
    // & operatörü bize direk nesnenin kendisi ile uğraşma imkanı sunar

    window.clear();
    window.draw(jpg);
    window.display();
}// end of function
// ----------------------------------------------

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
//--------------------------------------------
void bombaYerlestir(vector <vector <Hucre>>& alan, int mayinSayisi) {

    int satirSayisi = alan.size(); // satır Sayisini vericek 
    int sutunSayisi = alan[0].size(); // sütünSayısını vericek 

    int yerlesTirlenMayinSayisi = 0;
   
    while (yerlesTirlenMayinSayisi < mayinSayisi) {
        int r = rand() % satirSayisi ;
        int c = rand() % sutunSayisi ;
         
        if (alan[r][c].bombaVarmi == false) {
            alan[r][c].bombaVarmi = true;
            yerlesTirlenMayinSayisi++;

        }
    }

}// end of funciton
// Mayınlar yerleştirme

//---------------------------------------------------
int zorlukSeviyesi(vector <vector<Hucre> >& alan, OyunZorlugu secilenSeviye) {

    int toplamMayin = 0;

    //kullanıcıya hangi Seviyede oynamak istiyor ogrendik
    switch (secilenSeviye) {
     case::OyunZorlugu::kolay: 
         alan.assign(10,vector<Hucre>(10));    // DÜzenleme 1-) alan 10*10 olarak düzenlendi 
         toplamMayin = 10; // kullanici kolay seviyesi seçer ise
         break;

     case::OyunZorlugu::orta:
         alan.assign(13, vector<Hucre>(27));
         toplamMayin = 30;  // kullanıcı orta seviyesi seçerise
         break;

     case::OyunZorlugu::zor:
         alan.assign(20, vector<Hucre>(40));  // kullancı zor seviyesi seçer ise
         toplamMayin = 40; 
         break;
     default:
         cout << "Secilen Seviye mevcut degi" << endl;
         break;
    }

   // hazir kullanici hangi zorluğu oynayacağını seçmiş iken toplam mayınsayısınıda parametre olarak girdik
   
    return toplamMayin; // toplam mayin sayini geri return etmemizin amacı güvenli kutu sayısını bilmek 
                        // güvenliKutuSayisi = toplam kutu sayisi - toplamMayinSayisi

}// end of fuction


//--------------------------------------
HucreAyarları hucreAyarlarıAyarla(OyunZorlugu zorluk) {

    HucreAyarları ayar; // Boş paketimizi oluşturduk

    if (zorluk == OyunZorlugu::kolay) {
        ayar.BaslangıcX = 171.0f;
        ayar.BaslangıcY = 97.0f;
        ayar.BitisX = 600.0f;
        ayar.BitisY = 568.0f;
        ayar.sutunSayisi = 10.0f;
        ayar.satirSayisi = 10.0f;
    }
    else if (zorluk == OyunZorlugu::orta) {
        ayar.BaslangıcX = 32.0f;
        ayar.BaslangıcY = 144.0f;
        ayar.BitisX = 764.0f;
        ayar.BitisY = 536.0f;
        ayar.sutunSayisi = 27.0f;
        ayar.satirSayisi = 13.0f;
    }
    else if (zorluk == OyunZorlugu::zor) {
        ayar.BaslangıcX = 1.0f;
        ayar.BaslangıcY = 80.0f;
        ayar.BitisX = 795.0f;
        ayar.BitisY = 634.0f;
        ayar.sutunSayisi = 40.0f;
        ayar.satirSayisi = 20.0f;
    }

    // Matematik işlemini her if bloğunda tekrar yazmak yerine 
    // en sonda tek bir kere yapıyoruz (DRY Kuralı!)
    ayar.HucreGenisligi = (ayar.BitisX - ayar.BaslangıcX) / ayar.sutunSayisi;
    ayar.HucreYuksekligi = (ayar.BitisY - ayar.BaslangıcY) / ayar.satirSayisi;

    return ayar; // Paketi geri gönder
        


}//end of function
Vector2i TiklananHucreyiBul(float fareX , float fareY,OyunZorlugu &zorluk) {
    
    HucreAyarları ayarlar = hucreAyarlarıAyarla(zorluk); // oynanabilir alanı kontrol ediyoruz
    

    // Kontrol aşaması  mouse satırların içindemi değil mi
    if (fareX >= ayarlar.BaslangıcX && fareX <= ayarlar.BitisX && fareY >= ayarlar.BaslangıcY && fareY <= ayarlar.BitisY) { // aralık belirleniyor
        
        int sutunIndeksi = (fareX - ayarlar.BaslangıcX) / ayarlar.HucreGenisligi; // farenin x bileşini hesaplandı
        int satirIndeksi = (fareY - ayarlar.BaslangıcY) / ayarlar.HucreYuksekligi; // farenin y bileşeni hesaplandı

        return Vector2i(sutunIndeksi, satirIndeksi); 
    }
    
        return Vector2i(-1, -1); // tıklanan kare alanın dışında ise 
    

 } // end of funciton
//-----------------------------------------------------------
int komsuMayınlariSay(vector <vector<Hucre>>&alan , int satir , int sutun) { // vektörümüzü direk değilde bellek adresini veriyoruz ki Kocaman vektörleri kopyalamak uzun sürmesin 
    int sayac = 0 ; // secilen karenin etrafındaki mayınları sayacağız ;

    int SatirSayisi = alan.size(); // vectorun eleman sayısını Kaç satır olduğu
    int sutunSayisi = alan[0].size(); // Sutun Sayısı 
    int yeniSatir, yeniSutun;

    for (int i = -1; i <= 1; i++) { // satır hareketi 0,1
        for (int j = -1; j <= 1; j++) { // sütün hareketi

            if (i == 0 && j == 0) {
                continue;  // seçilen kare 
        }
            yeniSatir = satir + i; 
            yeniSutun = sutun + j;
                
            if (yeniSatir >= 0 && yeniSatir < SatirSayisi && yeniSutun >= 0 && yeniSutun < sutunSayisi) { // eğer kullanıcı köşeye basar ise overFlowu engellemek için  yoksa program çöker alan[-1] gibi bir değere ulaşmaya çalışır 
                if (alan[yeniSatir][yeniSutun].bombaVarmi == true) {
                    sayac++ ;
                }
                
            }
                

        }//second for 
     }// first for


    return sayac ; 
 } // end of function
//-----------------------------------
void komsuMayinlariHesapla(vector<vector<Hucre>>& alan) { // bu fonksiyon ile oyun başladığında tüm hücrelerdeki komşu mayın sayısı hesaplanır 
    int satirSayisi = alan.size();
    int sutunSayisi = alan[0].size();

    for (int satir = 0; satir < satirSayisi; satir++) {
        for (int sutun = 0; sutun < sutunSayisi; sutun++) {

            if (alan[satir][sutun].bombaVarmi == false) {
                alan[satir][sutun].komsuHucreMayinSayisi = komsuMayınlariSay(alan, satir, sutun);
            }
        }
    }
} // end of function 
//-------------------------
bool solTiklamayiIsle(vector <vector<Hucre>>& alan, float fareX, float fareY, OyunZorlugu secilenzorluk) {

    Vector2i tıklananKoordinat = TiklananHucreyiBul(fareX, fareY, secilenzorluk); // tıklananHUcreyi bul fonksiyonu ile kullanıcının tıkladığı hücre  kordinata çevrilcek eğer kullancı hücre dışında bir yere dokunursa -1 değerini döndürecek

    if (tıklananKoordinat.x != -1 && tıklananKoordinat.y != -1) {
        int satir = tıklananKoordinat.y;
        int sutun = tıklananKoordinat.x;

        if (alan[satir][sutun].acıkMi == false) { // eğer rhücre kapali ise açık yapar tekrar tekrar işlem yapmaktan kaçınır
            alan[satir][sutun].acıkMi = true;
            cout << "Acilan Hucre -> Satir: " << satir + 1 << " Sutun: " << sutun + 1 << endl;

            if (alan[satir][sutun].bombaVarmi == true) {

                return true; // oyun biter
            }

        }
        
    } 
    return false; // oyna devam eder
} // end of function

void FlopFill(vector <vector<Hucre>>& alan, int satir, int sutun) { // recursion function

    if (satir < 0 || satir >= alan.size() || sutun < 0 || sutun >= alan[0].size() || alan[satir][sutun].acıkMi == true) {
        return; // hucre açıkmı olduğunu kontrol etme ve vectorun sınırların dısışına çıkmasını engelleme
    }

    alan[satir][sutun].acıkMi = true; // hucre acıldı

    if (alan[satir][sutun].komsuHucreMayinSayisi != 0) { // komşu hucre mayınSayisi  0 dan farklı ise
        return;
    }

    FlopFill(alan,satir-1,sutun-1);
    FlopFill(alan,satir-1,sutun);  // recursion fonksiyonlar ile flopfill algoritması yapıldı böylece bir hücrenin çevresi boş olduğunda tüm hücreler açılcak onların çevressindeki hücrelerde açılacak
    FlopFill(alan,satir-1,sutun+1);
    FlopFill(alan,satir,sutun-1);
    FlopFill(alan,satir,sutun+1);
    FlopFill(alan,satir+1,sutun-1);
    FlopFill(alan,satir+1,sutun);
    FlopFill(alan,satir+1,sutun+1);
}// end of function 













