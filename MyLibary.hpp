#pragma once

#include <SFML/Graphics.hpp>    
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
using namespace std;
using namespace sf;


struct HucreAyarlari {
    float sutunSayisi;
    float satirSayisi;
    float HucreGenisligi;
    float HucreYuksekligi;
    float BaslangicX;
    float BaslangicY;
    float BitisX;
    float BitisY;
};

struct Hucre {
    bool bombaVarmi = false;
    bool bayrakVarmi = false;
    bool acikMi = false;
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
    cikisEkrani,
    duraklamaEkrani,
    lvlCikisEkrani,
    gameOverScreen,
    winScreen,
};

// Fonksiyon Prototipleri

void bombaYerlestir(vector<vector<Hucre>>& alan, int mayinSayisi, int ilksatir, int ilksutun);
Vector2i TiklananHucreyiBul(float fareX, float fareY, OyunZorlugu& zorluk);
bool solTiklamayiIsle(vector <vector<Hucre>>& alan, float fareX, float fareY, OyunZorlugu secilenzorluk);
void volumeChange(Keyboard::Key basilanTus, SoundSource& sesKaynagi);
void ekraniGuncelle(RenderWindow& window, vector <vector<Hucre>>& alan, Sprite acikKutuJpg, OyunZorlugu zorluk, Sprite bombaJpg, Font& font, Sprite bayrakJpg);
int komsuMayinlariSay(vector <vector<Hucre>>& alan, int satir, int sutun);
void komsuMayinlariHesapla(vector<vector<Hucre>>& alan);
int zorlukSeviyesi(vector <vector<Hucre> >& alan, OyunZorlugu secilenSeviye);
HucreAyarlari hucreAyarlariAyarla(OyunZorlugu zorluk);
void FlopFill(vector <vector<Hucre>>& alan, int satir, int sutun); // EKLENDİ
bool oyunKazanildimi(vector <vector<Hucre>>& alan);


