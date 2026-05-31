#include <iostream>
#include "MyLibary.hpp"
#include <cstdlib>


using namespace std;
using namespace sf;

void ekraniGuncelle(RenderWindow& window, vector <vector<Hucre>>& alan, Sprite acikKutujpg, OyunZorlugu zorluk, Sprite bombaJpg, Font& font, Sprite bayrakJpg) {
    HucreAyarlari ayarlar = hucreAyarlariAyarla(zorluk);

    // Açık kutu ölçeklendirme
    float orjinalAcikX = acikKutujpg.getTexture().getSize().x;
    float orjinalAcikY = acikKutujpg.getTexture().getSize().y;
    acikKutujpg.setScale(Vector2f(ayarlar.HucreGenisligi / orjinalAcikX, ayarlar.HucreYuksekligi / orjinalAcikY));

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
            float positionX = ayarlar.BaslangicX + (sutun * ayarlar.HucreGenisligi);
            float positionY = ayarlar.BaslangicY + (satir * ayarlar.HucreYuksekligi);

            if (alan[satir][sutun].acikMi) {
                // --- HÜCRE AÇIKSA YAPILACAKLAR ---
                if (alan[satir][sutun].bombaVarmi) {
                    bombaJpg.setPosition(Vector2f(positionX, positionY));
                    window.draw(bombaJpg);
                }
                else {
                    acikKutujpg.setPosition(Vector2f(positionX, positionY));
                    window.draw(acikKutujpg);

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

HucreAyarlari hucreAyarlariAyarla(OyunZorlugu zorluk) {
    HucreAyarlari ayar;

    if (zorluk == OyunZorlugu::kolay) {
        ayar.BaslangicX = 171.0f;
        ayar.BaslangicY = 97.0f;
        ayar.BitisX = 600.0f;
        ayar.BitisY = 568.0f;
        ayar.sutunSayisi = 12.0f;
        ayar.satirSayisi = 11.0f;
    }
    else if (zorluk == OyunZorlugu::orta) {
        ayar.BaslangicX = 32.0f;
        ayar.BaslangicY = 144.0f;
        ayar.BitisX = 764.0f;
        ayar.BitisY = 536.0f;
        ayar.sutunSayisi = 27.0f;
        ayar.satirSayisi = 13.0f;
    }
    else if (zorluk == OyunZorlugu::zor) {
        ayar.BaslangicX = 1.0f;
        ayar.BaslangicY = 80.0f;
        ayar.BitisX = 795.0f;
        ayar.BitisY = 634.0f;
        ayar.sutunSayisi = 40.0f;
        ayar.satirSayisi = 20.0f;
    }

    ayar.HucreGenisligi = (ayar.BitisX - ayar.BaslangicX) / ayar.sutunSayisi;
    ayar.HucreYuksekligi = (ayar.BitisY - ayar.BaslangicY) / ayar.satirSayisi;

    return ayar;
} // end of funciton

Vector2i TiklananHucreyiBul(float fareX, float fareY, OyunZorlugu& zorluk) {
    HucreAyarlari ayarlar = hucreAyarlariAyarla(zorluk);

    if (fareX >= ayarlar.BaslangicX && fareX <= ayarlar.BitisX && fareY >= ayarlar.BaslangicY && fareY <= ayarlar.BitisY) {
        int sutunIndeksi = (fareX - ayarlar.BaslangicX) / ayarlar.HucreGenisligi;
        int satirIndeksi = (fareY - ayarlar.BaslangicY) / ayarlar.HucreYuksekligi;
        return Vector2i(sutunIndeksi, satirIndeksi);
    }
    return Vector2i(-1, -1);
} // end of function

int komsuMayinlariSay(vector <vector<Hucre>>& alan, int satir, int sutun) {
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
                alan[satir][sutun].komsuHucreMayinSayisi = komsuMayinlariSay(alan, satir, sutun);
            }
        }
    }
} // end of function 

// Flood Fill Algoritması 
void FlopFill(vector <vector<Hucre>>& alan, int satir, int sutun) {
    if (satir < 0 || satir >= alan.size() || sutun < 0 || sutun >= alan[0].size() || alan[satir][sutun].acikMi == true) {
        return;
    }

    alan[satir][sutun].acikMi = true; // Hücre açıldı

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
    Vector2i tiklananKoordinat = TiklananHucreyiBul(fareX, fareY, secilenzorluk);

    if (tiklananKoordinat.x != -1 && tiklananKoordinat.y != -1) {
        int satir = tiklananKoordinat.y;
        int sutun = tiklananKoordinat.x;

        if (alan[satir][sutun].acikMi == false) {

            // Bombaya basılırsa
            if (alan[satir][sutun].bombaVarmi) {
                alan[satir][sutun].acikMi = true;
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
            if (alan[satir][sutun].bombaVarmi == false && alan[satir][sutun].acikMi == false) {

                return false; // oyunu kazanamadı
            }

        }
    }

    return true;
}
