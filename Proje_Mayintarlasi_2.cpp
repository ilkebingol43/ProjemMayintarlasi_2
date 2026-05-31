#include <SFML/Graphics.hpp>    
#include <iostream>   
#include <vector>
#include <string>
#include <SFML/Window.hpp>
#include <chrono>  
#include <SFML/Window/Event.hpp> 
#include <SFML/Audio.hpp>
#include <algorithm> 
#include <cmath> 
#include <ctime>  
#include <cstdlib> 
#include "MyLibary.hpp"

using namespace std;
using namespace sf;



int main()
{
    srand(time(NULL)); // oyun her başladığında mayınların rastgele dağıtılmasını sağlar bu olmaz ise mayınlar sürekli aynı yerde olur 

    GameState gameState = GameState::girisEkrani;
    OyunZorlugu secilenlevel = OyunZorlugu::zor;

    RenderWindow window(VideoMode({ 800, 640 }), "Mayin Tarlasi");

    vector<vector<Hucre>> oyunTahtasi;
    zorlukSeviyesi(oyunTahtasi, secilenlevel);

    // Açık Hücre Resmi
    Texture openblock;
    if (!openblock.loadFromFile("Jpg/jacikhucre.jpg")) cout << "Error loading jacikhucre.jpg" << endl;
    Sprite acıkHucre(openblock);

    // Font
    Font font;
    if (!font.openFromFile("Font/MarielleFranco.ttf")) cout << "Error loading font" << endl;

    // Bomba Resmi
    Texture bomba;
    if (!bomba.loadFromFile("Jpg/jbombajpg.jpg")) cout << "Resim yuklenemedi" << endl;
    Sprite bomb(bomba);

    //Flag resmi 
    Texture flag;
    if (!flag.loadFromFile("Jpg/jBayrak.jpg")) cout << "Bayrak yüklenemedi " << endl;
    Sprite bayrak(flag);

    // Giriş Ekranı
    Texture firstScreen;
    if (!firstScreen.loadFromFile("Jpg/jgirisEkrani.jpg")) cout << "GirisEkrani Yuklenemedi" << endl;
    Sprite girisEkrani(firstScreen);
    girisEkrani.setScale(Vector2f(800.0f / 1698.0f, 640.0f / 926.0f));

    // Çıkış Ekranı
    Texture exit;
    if (!exit.loadFromFile("Jpg/jCikisEkrani.jpg")) cout << "CikisEkrani yuklenemedi " << endl;;
    Sprite cikisEkrani(exit);
    cikisEkrani.setScale(Vector2f(800.0f / 1600.0f, 640.0f / 872.0f));

    // Level Ekranı
    Texture levelScreen;
    if (!levelScreen.loadFromFile("Jpg/jSeviye.jpg")) cout << "SeviyeEkrani Yuklenemedi" << endl;
    Sprite seviyeEkrani(levelScreen);
    seviyeEkrani.setScale(Vector2f(800.0f / 1536.0f, 640.0f / 1024.0f));

    // Level Çıkış Ekranı
    Texture lvlScreenExit;
    if (!lvlScreenExit.loadFromFile("Jpg/jLevelEkranicıkıs.jpg")) cout << "SeviyeCikisEkrani yuklenemedi" << endl;
    Sprite lvlCıkısEkrani(lvlScreenExit);
    lvlCıkısEkrani.setScale(Vector2f(800.0f / 1677.0f, 640.0f / 938.0f));

    // Oyun Ekranları
    Texture gameScreen1;
    if (!gameScreen1.loadFromFile("Jpg/joyunEkraniZorMode.jpg")) cout << "Oyun Ekrani zor yuklenemedi" << endl;
    Sprite oyunEkraniZor(gameScreen1);
    oyunEkraniZor.setScale(Vector2f(800.0f / 1672.0f, 640.0f / 940.0f));

    Texture gameScreen2;
    if (!gameScreen2.loadFromFile("Jpg/joyunEkraniOrtaMode.jpg")) cout << "Oyun Ekrani orta yuklenemedi" << endl;
    Sprite oyunEkraniOrta(gameScreen2);
    oyunEkraniOrta.setScale(Vector2f(800.0f / 1509.0f, 640.0f / 1042.0f));

    Texture gameScreen3;
    if (!gameScreen3.loadFromFile("Jpg/joyunEkraniKolayMode.jpg")) cout << "Oyun Ekrani kolay yuklenemedi " << endl;
    Sprite oyunEkraniKolay(gameScreen3);
    oyunEkraniKolay.setScale(Vector2f(800.0f / 1536.0f, 640.0f / 1024.0f));

    Texture gameOverScreenTex;
    if (!gameOverScreenTex.loadFromFile("Jpg/jgameoverScreen.jpg")) cout << "Game Over ekranı yuklenemedi" << endl;
    Sprite oyunbitisEkranı(gameOverScreenTex);
    oyunbitisEkranı.setScale(Vector2f(800.f / 1400.0f, 640.0f / 1122.0f));

    Texture winScreenTex;
    if (!winScreenTex.loadFromFile("Jpg/jWinscreen.jpg")) cout << "WinScreen ekranı yuklenemedi " << endl;
    Sprite winEkranı(winScreenTex);
    winEkranı.setScale(Vector2f(800.0f / 1402.0f, 640 / 1122.0f));


    // Müzik ve Sesler
    Music girisMuzigi;
    if (!girisMuzigi.openFromFile("Ses/sGirisMuzigi.mp3")) cout << "Giris muzigi yuklenemedi\n";
    girisMuzigi.setLooping(true);
    girisMuzigi.setVolume(50);
    girisMuzigi.setPitch(0.75f);
    girisMuzigi.play();

    SoundBuffer tiklmamaMuzigi;
    if (!tiklmamaMuzigi.loadFromFile("Ses/stiklamaSesi.wav")) cout << "Tiklama sesi yuklenemedi\n";
    Sound tiklamaSesi(tiklmamaMuzigi);
    tiklamaSesi.setVolume(50.f);

    bool ilkTıklama = true;

    // --- SAYAÇ (KRONOMETRE) AYARLARI ---
    Clock oyunSaati;
    int gecenSure = 0;

    Text sayacMetni(font); // Daha önce tanımladığın 'font'u kullanıyoruz
    sayacMetni.setCharacterSize(35); // Yazı boyutu
    sayacMetni.setFillColor(Color::Red); // Rengimiz kırmızı olsun
    sayacMetni.setPosition(Vector2f(350.f, 20.f)); // Ekranın üst-orta kısmına yerleştirelim

    // ANA OYUN DÖNGÜSÜ
    while (window.isOpen()) {

        // 1. OLAY (EVENT) İŞLEME DÖNGÜSÜ
        while (const auto event = window.pollEvent()) {
            if (event->is <Event::Closed>()) {
                window.close();
            }

            if (const auto* basilanTus = event->getIf <Event::KeyReleased>()) {
                volumeChange(basilanTus->code, girisMuzigi);
                volumeChange(basilanTus->code, tiklamaSesi);

                // ESC Tuşu Kontrolü
                if (basilanTus->scancode == Keyboard::Scancode::Escape) {
                    if (gameState == GameState::cikisEkrani) gameState = GameState::girisEkrani;
                    else if (gameState == GameState::girisEkrani) gameState = GameState::cikisEkrani;
                    else if (gameState == GameState::levelEkrani) gameState = GameState::lvlCikisEkrani;
                    else if (gameState == GameState::lvlCikisEkrani) gameState = GameState::levelEkrani;

                    // --- BUNU YENİ EKLEDİK ---
                    // Oyun bittiğinde veya kazanıldığında ESC'ye basılırsa Ana Menüye (Giriş Ekranı) dön
                    else if (gameState == GameState::gameOverScreen || gameState == GameState::winScreen) {
                        gameState = GameState::girisEkrani;
                    }
                }
                // Enter Tuşu Kontrolü
                else if (basilanTus->scancode == Keyboard::Scancode::Enter) {
                    if (gameState == GameState::girisEkrani) gameState = GameState::levelEkrani;
                    else if (gameState == GameState::cikisEkrani) window.close();
                    else if (gameState == GameState::lvlCikisEkrani) gameState = GameState::girisEkrani;

                    // --- BUNLARI YENİ EKLEDİK ---
                    // Oyuncu kazandıysa veya kaybettiyse, Enter'a basınca seviye seçme ekranına dönsün
                    else if (gameState == GameState::gameOverScreen || gameState == GameState::winScreen) {
                        gameState = GameState::levelEkrani;
                    }
                }
                // Zorluk Seçimi (1, 2, 3)
                else if (gameState == GameState::levelEkrani) {
                    if (basilanTus->scancode == Keyboard::Scancode::Numpad1 || basilanTus->scancode == Keyboard::Scancode::Num1) {
                        gameState = GameState::oyunEkrani;
                        secilenlevel = OyunZorlugu::kolay;
                        zorlukSeviyesi(oyunTahtasi, secilenlevel);
                        ilkTıklama = true; // Yeni oyun için sıfırla
                    }
                    else if (basilanTus->scancode == Keyboard::Scancode::Numpad2 || basilanTus->scancode == Keyboard::Scancode::Num2) {
                        gameState = GameState::oyunEkrani;
                        secilenlevel = OyunZorlugu::orta;
                        zorlukSeviyesi(oyunTahtasi, secilenlevel);
                        ilkTıklama = true;
                    }
                    else if (basilanTus->scancode == Keyboard::Scancode::Numpad3 || basilanTus->scancode == Keyboard::Scancode::Num3) {
                        gameState = GameState::oyunEkrani;
                        secilenlevel = OyunZorlugu::zor;
                        zorlukSeviyesi(oyunTahtasi, secilenlevel);
                        ilkTıklama = true;
                    }
                }
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (gameState == GameState::oyunEkrani) {
                    Vector2i koordinat = TiklananHucreyiBul(mousePressed->position.x, mousePressed->position.y, secilenlevel);

                    if (koordinat.x != -1 && koordinat.y != -1) {
                        int satir = koordinat.y;
                        int sutun = koordinat.x;

                        // ---- SAĞ TIK İŞLEMİ (BAYRAK KOY/KALDIR) ----
                        if (mousePressed->button == sf::Mouse::Button::Right) {
                            // Sadece kapalı hücrelere bayrak konulabilir
                            if (!oyunTahtasi[satir][sutun].acikMi) {
                                oyunTahtasi[satir][sutun].bayrakVarmi = !oyunTahtasi[satir][sutun].bayrakVarmi;
                            }
                        }

                        // ---- SOL TIK İŞLEMİ ----
                        else if (mousePressed->button == sf::Mouse::Button::Left) {
                            // Eğer hücrede bayrak varsa sol tıklamayı yoksay
                            if (oyunTahtasi[satir][sutun].bayrakVarmi) {
                                continue;
                            }

                            tiklamaSesi.play();

                            if (ilkTıklama) {
                                int mayinSayisi = (secilenlevel == OyunZorlugu::kolay) ? 10 : (secilenlevel == OyunZorlugu::orta) ? 30 : 40;
                                bombaYerlestir(oyunTahtasi, mayinSayisi, satir, sutun);
                                ilkTıklama = false;
                                komsuMayinlariHesapla(oyunTahtasi);

                                oyunSaati.restart();
                            }

                            bool mayinaBasildi = solTiklamayiIsle(oyunTahtasi, mousePressed->position.x, mousePressed->position.y, secilenlevel);

                            if (mayinaBasildi) {
                                gameState = GameState::gameOverScreen;
                            }
                            else {
                                // Eğer mayına basmadıysak ve her tıklamadan sonra kazandık mı diye kontrol et:
                                if (oyunKazanildimi(oyunTahtasi)) {
                                    gameState = GameState::winScreen;
                                }
                            }
                        }
                    }
                }
            }
        } // Olay Kontrolü Sonu

        // 2. ÇİZİM DÖNGÜSÜ (RENDER)
        window.clear();

        if (gameState == GameState::girisEkrani) {
            window.draw(girisEkrani);
        }
        else if (gameState == GameState::cikisEkrani) {
            window.draw(cikisEkrani);
        }
        else if (gameState == GameState::levelEkrani) {
            window.draw(seviyeEkrani);
        }
        else if (gameState == GameState::lvlCikisEkrani) {
            window.draw(lvlCıkısEkrani);
        }
        else if (gameState == GameState::gameOverScreen) {
            window.draw(oyunbitisEkranı);
        } // <--- EKSİK OLAN SÜSLÜ PARANTEZ BURAYA EKLENDİ
        else if (gameState == GameState::winScreen) {
            window.draw(winEkranı); // Arka planı çiz

            // Yazının konumunu ekranın ortasına doğru alıyoruz
            // (Kendi arayüzüne göre x ve y değerlerini değiştirebilirsin)
            sayacMetni.setPosition(Vector2f(280.f, 300.f));

            // Ekrana yazılacak metni güncelliyoruz
            sayacMetni.setString("Skorun: " + to_string(gecenSure) + " Saniye");

            // Skoru ekrana çizdiriyoruz
            window.draw(sayacMetni);
        }
        else if (gameState == GameState::oyunEkrani) {
            if (secilenlevel == OyunZorlugu::kolay) window.draw(oyunEkraniKolay);
            else if (secilenlevel == OyunZorlugu::orta) window.draw(oyunEkraniOrta);
            else window.draw(oyunEkraniZor);

            ekraniGuncelle(window, oyunTahtasi, acıkHucre, secilenlevel, bomb, font, bayrak);

            // --- SAYAÇ KONUMUNU SIFIRLAMA ---
            // Kazanma ekranından dönüldüyse sayacın yeri bozulmasın diye tekrar yukarı alıyoruz
            sayacMetni.setPosition(Vector2f(350.f, 20.f));

            // --- SAYAÇ GÜNCELLEME VE ÇİZİMİ ---
            if (!ilkTıklama) {
                gecenSure = oyunSaati.getElapsedTime().asSeconds();
            }
            else {
                gecenSure = 0;
            }

            // Oyun içi sayaç metnini ayarlayıp çizdiriyoruz
            sayacMetni.setString("Sure: " + to_string(gecenSure));
            window.draw(sayacMetni);
        }

        window.display();
    }

    return 0;
}

