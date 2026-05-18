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

using namespace std;
using namespace sf;

struct HucreAyarları {
    float sutunSayisi;
    float satirSayisi;
    float HucreGenisligi;
    float HucreYuksekligi;
    float BaslangıcX;
    float BaslangıcY;
    float BitisX;
    float BitisY;
};

struct Hucre {
    bool bombaVarmi = false;
    bool bayrakVarmi = false;
    bool acıkMi = false;
    int komsuHucreMayinSayisi = 0;
    // ilk değer olarak saçma bir değer atanmasını engelliyoruz 
};

enum class OyunZorlugu {
    kolay,
    orta,
    zor,
};

enum class GameState {
    girisEkrani,
    oyunEkrani,
    levelEkrani,
    cıkısEkrani,
    duraklamaEkrani,
    lvlCıkısEkrani,
    gameOverScreen,
    winScreen,
};

// Fonksiyon Prototipleri
void bombaYerlestir(vector<vector<Hucre>>& alan, int mayinSayisi, int ilksatir, int ilksutun);
Vector2i TiklananHucreyiBul(float fareX, float fareY, OyunZorlugu& zorluk);
bool solTiklamayiIsle(vector <vector<Hucre>>& alan, float fareX, float fareY, OyunZorlugu secilenzorluk);
void volumeChange(Keyboard::Key basilanTus, SoundSource& sesKaynagi);
void ekranıGüncelle(RenderWindow& window, vector <vector<Hucre>>& alan, Sprite acıkKutujpg, OyunZorlugu zorluk, Sprite bombaJpg, Font& font, Sprite bayrakJpg);
int komsuMayınlariSay(vector <vector<Hucre>>& alan, int satir, int sutun);
void komsuMayinlariHesapla(vector<vector<Hucre>>& alan);
int zorlukSeviyesi(vector <vector<Hucre> >& alan, OyunZorlugu secilenSeviye);
HucreAyarları hucreAyarlarıAyarla(OyunZorlugu zorluk);
void FlopFill(vector <vector<Hucre>>& alan, int satir, int sutun); // EKLENDİ
bool oyunKazanildimi(vector <vector<Hucre>>& alan);

int main()
{
    srand(time(NULL));

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
                    if (gameState == GameState::cıkısEkrani) gameState = GameState::girisEkrani;
                    else if (gameState == GameState::girisEkrani) gameState = GameState::cıkısEkrani;
                    else if (gameState == GameState::levelEkrani) gameState = GameState::lvlCıkısEkrani;
                    else if (gameState == GameState::lvlCıkısEkrani) gameState = GameState::levelEkrani;

                    // --- BUNU YENİ EKLEDİK ---
                    // Oyun bittiğinde veya kazanıldığında ESC'ye basılırsa Ana Menüye (Giriş Ekranı) dön
                    else if (gameState == GameState::gameOverScreen || gameState == GameState::winScreen) {
                        gameState = GameState::girisEkrani;
                    }
                }
                // Enter Tuşu Kontrolü
                else if (basilanTus->scancode == Keyboard::Scancode::Enter) {
                    if (gameState == GameState::girisEkrani) gameState = GameState::levelEkrani;
                    else if (gameState == GameState::cıkısEkrani) window.close();
                    else if (gameState == GameState::lvlCıkısEkrani) gameState = GameState::girisEkrani;

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
                            if (!oyunTahtasi[satir][sutun].acıkMi) {
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
        else if (gameState == GameState::cıkısEkrani) {
            window.draw(cikisEkrani);
        }
        else if (gameState == GameState::levelEkrani) {
            window.draw(seviyeEkrani);
        }
        else if (gameState == GameState::lvlCıkısEkrani) {
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

            ekranıGüncelle(window, oyunTahtasi, acıkHucre, secilenlevel, bomb, font, bayrak);

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

// ---------------- FONKSİYONLAR ----------------

void ekranıGüncelle(RenderWindow& window, vector <vector<Hucre>>& alan, Sprite acıkKutujpg, OyunZorlugu zorluk, Sprite bombaJpg, Font& font, Sprite bayrakJpg) {
    HucreAyarları ayarlar = hucreAyarlarıAyarla(zorluk);

    // Açık kutu ölçeklendirme
    float orjinalAcikX = acıkKutujpg.getTexture().getSize().x;
    float orjinalAcıkY = acıkKutujpg.getTexture().getSize().y;
    acıkKutujpg.setScale(Vector2f(ayarlar.HucreGenisligi / orjinalAcikX, ayarlar.HucreYuksekligi / orjinalAcıkY));

    // Bomba ölçeklendirme
    float orjinalBombaX = bombaJpg.getTexture().getSize().x;
    float orjinalBombaY = bombaJpg.getTexture().getSize().y;
    bombaJpg.setScale(Vector2f(ayarlar.HucreGenisligi / orjinalBombaX, ayarlar.HucreYuksekligi / orjinalBombaY));

    // BAYRAK ÖLÇEKLENDİRME (Eklendi)
    float orjinalBayrakX = bayrakJpg.getTexture().getSize().x;
    float orjinalBayrakY = bayrakJpg.getTexture().getSize().y;
    bayrakJpg.setScale(Vector2f(ayarlar.HucreGenisligi / orjinalBayrakX, ayarlar.HucreYuksekligi / orjinalBayrakY));


    for (int satir = 0; satir < alan.size(); satir++) {
        for (int sutun = 0; sutun < alan[0].size(); sutun++) {

            // Pozisyon X ve Y'yi "acıkMi" şartının DIŞINA aldık.
            // Çünkü hücre kapalıyken de bayrak çizmek için bu koordinatlara ihtiyacımız var.
            float positionX = ayarlar.BaslangıcX + (sutun * ayarlar.HucreGenisligi);
            float positionY = ayarlar.BaslangıcY + (satir * ayarlar.HucreYuksekligi);

            if (alan[satir][sutun].acıkMi) {
                // --- HÜCRE AÇIKSA YAPILACAKLAR ---
                if (alan[satir][sutun].bombaVarmi) {
                    bombaJpg.setPosition(Vector2f(positionX, positionY));
                    window.draw(bombaJpg);
                }
                else {
                    acıkKutujpg.setPosition(Vector2f(positionX, positionY));
                    window.draw(acıkKutujpg);

                    int mayinSayisi = alan[satir][sutun].komsuHucreMayinSayisi;

                    if (mayinSayisi > 0) {
                        Text sayi(font);
                        sayi.setString(to_string(mayinSayisi));
                        sayi.setCharacterSize(static_cast<int>(ayarlar.HucreYuksekligi * 0.7f));
                        sayi.setFillColor(Color::White);

                        float yaziX = positionX + (ayarlar.HucreGenisligi * 0.25f);
                        float yaziY = positionY - (ayarlar.HucreYuksekligi * 0.05f);

                        sayi.setPosition(Vector2f(yaziX, yaziY));
                        window.draw(sayi);
                    }
                }
            }
            else {
                // --- HÜCRE KAPALIYSA YAPILACAKLAR (Eklendi) ---
                if (alan[satir][sutun].bayrakVarmi) {
                    bayrakJpg.setPosition(Vector2f(positionX, positionY));
                    window.draw(bayrakJpg);
                }
            }
        }
    }
}

void volumeChange(Keyboard::Key basilanTus, SoundSource& sesKaynagi) {
    if (basilanTus == Keyboard::Key::F1) {
        sesKaynagi.pause();
    }
    if (basilanTus == Keyboard::Key::F3) {
        if (sesKaynagi.getStatus() != SoundSource::Status::Playing) sesKaynagi.play();

        float mevcutSes = sesKaynagi.getVolume();
        if (mevcutSes < 100.0f) {
            sesKaynagi.setVolume(min(mevcutSes + 5.0f, 100.0f));
        }
    }
    if (basilanTus == Keyboard::Key::F2) {
        if (sesKaynagi.getStatus() != SoundSource::Status::Playing) sesKaynagi.play();

        float mevcutSes = sesKaynagi.getVolume();
        if (mevcutSes > 0.0f) {
            sesKaynagi.setVolume(max(mevcutSes - 5.0f, 0.0f));
        }
    }
}

void bombaYerlestir(vector <vector <Hucre>>& alan, int mayinSayisi, int ilksatir, int ilksutun) {
    int satirSayisi = alan.size();
    int sutunSayisi = alan[0].size();
    int yerlesTirlenMayinSayisi = 0;

    while (yerlesTirlenMayinSayisi < mayinSayisi) {
        int r = rand() % satirSayisi;
        int c = rand() % sutunSayisi;

        if (alan[r][c].bombaVarmi == false && (r != ilksatir || c != ilksutun)) {
            alan[r][c].bombaVarmi = true;
            yerlesTirlenMayinSayisi++;
        }
    }
}

int zorlukSeviyesi(vector <vector<Hucre> >& alan, OyunZorlugu secilenSeviye) {
    int toplamMayin = 0;

    switch (secilenSeviye) {
    case OyunZorlugu::kolay:
        alan.assign(11, vector<Hucre>(12));
        toplamMayin = 10;
        break;

    case OyunZorlugu::orta:
        alan.assign(13, vector<Hucre>(27));
        toplamMayin = 30;
        break;

    case OyunZorlugu::zor:
        alan.assign(20, vector<Hucre>(40));
        toplamMayin = 40;
        break;
    default:
        break;
    }
    return toplamMayin;
} // end of function 

HucreAyarları hucreAyarlarıAyarla(OyunZorlugu zorluk) {
    HucreAyarları ayar;

    if (zorluk == OyunZorlugu::kolay) {
        ayar.BaslangıcX = 171.0f;
        ayar.BaslangıcY = 97.0f;
        ayar.BitisX = 600.0f;
        ayar.BitisY = 568.0f;
        ayar.sutunSayisi = 12.0f;
        ayar.satirSayisi = 11.0f;
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

    ayar.HucreGenisligi = (ayar.BitisX - ayar.BaslangıcX) / ayar.sutunSayisi;
    ayar.HucreYuksekligi = (ayar.BitisY - ayar.BaslangıcY) / ayar.satirSayisi;

    return ayar;
} // end of funciton

Vector2i TiklananHucreyiBul(float fareX, float fareY, OyunZorlugu& zorluk) {
    HucreAyarları ayarlar = hucreAyarlarıAyarla(zorluk);

    if (fareX >= ayarlar.BaslangıcX && fareX <= ayarlar.BitisX && fareY >= ayarlar.BaslangıcY && fareY <= ayarlar.BitisY) {
        int sutunIndeksi = (fareX - ayarlar.BaslangıcX) / ayarlar.HucreGenisligi;
        int satirIndeksi = (fareY - ayarlar.BaslangıcY) / ayarlar.HucreYuksekligi;
        return Vector2i(sutunIndeksi, satirIndeksi);
    }
    return Vector2i(-1, -1);
} // end of function

int komsuMayınlariSay(vector <vector<Hucre>>& alan, int satir, int sutun) {
    int sayac = 0;
    int SatirSayisi = alan.size();
    int sutunSayisi = alan[0].size();

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

            int yeniSatir = satir + i;
            int yeniSutun = sutun + j;

            if (yeniSatir >= 0 && yeniSatir < SatirSayisi && yeniSutun >= 0 && yeniSutun < sutunSayisi) {
                if (alan[yeniSatir][yeniSutun].bombaVarmi) {
                    sayac++;
                }
            }
        }
    }
    return sayac;
} // end ıf function

void komsuMayinlariHesapla(vector<vector<Hucre>>& alan) {
    int satirSayisi = alan.size();
    int sutunSayisi = alan[0].size();

    for (int satir = 0; satir < satirSayisi; satir++) {
        for (int sutun = 0; sutun < sutunSayisi; sutun++) {
            if (!alan[satir][sutun].bombaVarmi) {
                alan[satir][sutun].komsuHucreMayinSayisi = komsuMayınlariSay(alan, satir, sutun);
            }
        }
    }
} // end of function 

// Flood Fill Algoritması 
void FlopFill(vector <vector<Hucre>>& alan, int satir, int sutun) {
    if (satir < 0 || satir >= alan.size() || sutun < 0 || sutun >= alan[0].size() || alan[satir][sutun].acıkMi == true) {
        return;
    }

    alan[satir][sutun].acıkMi = true; // Hücre açıldı

    if (alan[satir][sutun].komsuHucreMayinSayisi != 0) {
        return; // Sayı varsa dur
    }

    // 8 yöne özyinelemeli (recursive) ilerleme
    FlopFill(alan, satir - 1, sutun - 1);
    FlopFill(alan, satir - 1, sutun);
    FlopFill(alan, satir - 1, sutun + 1);
    FlopFill(alan, satir, sutun - 1);
    FlopFill(alan, satir, sutun + 1);
    FlopFill(alan, satir + 1, sutun - 1);
    FlopFill(alan, satir + 1, sutun);
    FlopFill(alan, satir + 1, sutun + 1);
} // end of function

bool solTiklamayiIsle(vector <vector<Hucre>>& alan, float fareX, float fareY, OyunZorlugu secilenzorluk) {
    Vector2i tıklananKoordinat = TiklananHucreyiBul(fareX, fareY, secilenzorluk);

    if (tıklananKoordinat.x != -1 && tıklananKoordinat.y != -1) {
        int satir = tıklananKoordinat.y;
        int sutun = tıklananKoordinat.x;

        if (alan[satir][sutun].acıkMi == false) {

            // Bombaya basılırsa
            if (alan[satir][sutun].bombaVarmi) {
                alan[satir][sutun].acıkMi = true;
                return true;
            }
            // Güvenli alana basılırsa FlopFill çalıştır
            else {
                FlopFill(alan, satir, sutun);
            }
        }
    }
    return false; // Oyun devam eder
} // end of function 


bool oyunKazanildimi(vector <vector<Hucre>>& alan) {

    for (int satir = 0; satir < alan.size();satir++) {
        for (int sutun = 0; sutun < alan[0].size();sutun++) {
            if (alan[satir][sutun].bombaVarmi == false && alan[satir][sutun].acıkMi == false) {

                return false; // oyunu kazanamadı
            }

        }
    }

    return true;
}