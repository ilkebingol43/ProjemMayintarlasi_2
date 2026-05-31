Mayın Tarlasi Dönem Projesi

Ad Soyad : İlke Bingöl
Öğrenci Numarası : 240229039

Sistem Gereksinimleri ve Bağımlılıklar

- C++20 destekleyen bir derleyici
- CMake 3.21 veya üzeri
- SFML 3.x kütüphanesi
- Grafik, pencere ve ses desteği olan bir işletim sistemi
- Projenin çalışması için Jpg, Font ve Ses klasörleri exe ile aynı çalışma dizininde bulunmalıdır.

Windows için proje verilen GitHub bağlantısındaki Releases bölümünden indirilebilir:
https://github.com/ilkebingol43/ProjemMayintarlasi_2/releases

Mac ve Linux Sistemlerde Projeyi Çalıştırma

1. SFML 3.x sistemde kurulu olmalıdır.

   macOS için Homebrew ile:
   brew install sfml

   Linux için paket yöneticisi ile:
   sudo apt install libsfml-dev

2. Proje klasöründe terminal açılır.

3. CMake build klasörü oluşturulur:
   cmake -S . -B build

4. Proje derlenir:
   cmake --build build

5. Program çalıştırılır:
   ./build/Proje_Mayintarlasi_2

Not: SFML sürümü veya sistem ayarları farklıysa CMake çalıştırılırken SFML_DIR yolu ayrıca verilmelidir.
