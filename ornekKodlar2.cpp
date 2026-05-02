 

// event açıklamaları

/*
   // bu komutlar genelikle tepki anında idealdir (space ile zıplama gibi)
   // window sınıfı ekranın işletim sistemi ile konuşmaısnı sağlar draw yada diğer fonksiyonları yoktur
    RenderWindow window;  // window sınıfından inheritance alır ve 2d çizim metotları sunar  window.draw() vardır
    window.create(VideoMode({800,640}), "Mayin tarlasi");
     // pencere oluşturuldu
    
    Clock oyunSaati;

    while (window.isOpen()) {  // kapatana kadar açık 
        while (const auto event = window.pollEvent()) {
            if (event->is < Event::Closed>()) {   // is true false döndürür sadece ne olduğuna bakar
                window.close();                   // getIf ise hem duruma bakar hemde eğerlere
            }

            if (const auto* resized = event->getIf<Event::Resized>()) { // kullanıcı ekranı sağdan veya soldan çekiştirdğinde göresellerin bouzlması engeller 
                // View view(FloatRect(0, 0, resized->size.x, resized->size.y));sfml2 dümdüz 4 tane sayı bekler
                View view(FloatRect({ 0.f, 0.f }, { static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) })); // sfml 3 2 adev vektör bekler 
                window.setView(view);                                                                                             // Pozisyon Vektörü: {x, y} Boyut Vektörü: {genişlik, yükseklik}

            }

            if (event->is < Event::FocusLost>()) {
                oyunSaati.stop();
            }
            if (event->is <Event::FocusGained>()) {
                oyunSaati.start();
            }
            if (const auto* textEntered = event->getIf<Event::TextEntered>())
            {
                if (textEntered->unicode < 128)
                    cout << "ASCII character typed: " << static_cast<char>(textEntered->unicode) << endl;// hangi tuşa basılırsa o harfi terminala basar ;
            }
            if (const auto* mouseWhellScroled = event->getIf < Event::MouseWheelScrolled>()) {
                switch (mouseWhellScroled->wheel) {
                 case Mouse::Wheel::Horizontal : 
                     cout << "Whell type is horizontal" << endl ;
                     break;
                 case Mouse::Wheel::Vertical :
                     cout << "whell type is vertical" << endl;
              }
                cout << "Whell moment" << mouseWhellScroled->delta << endl;
                cout << "Mouse x" << mouseWhellScroled->position.x << endl;
                cout << "Mouse y" << mouseWhellScroled->position.y << endl;
            }
            // mouseun ekranda yaptığı piksel değişikliğine odaklanır
            if (const auto* mouseMove = event->getIf <Event::MouseMoved>()){
                cout << "New mouse x:" << mouseMove->position.x << endl; // -> ile mouseMoveın posizyon nesnesine .x ilede onun x kordinatına ulaşıyruzo
                cout << "New mouse y:" << mouseMove->position.y << endl; //  

            }// mouseun sensöründen gelen ham veriye odaklanır
            if (const auto* mouseMovedRaw = event->getIf<sf::Event::MouseMovedRaw>())
            {
                cout << "new mouse x: " << mouseMovedRaw->delta.x << endl;
                cout << "new mouse y: " << mouseMovedRaw->delta.y << endl;
            }
            if (event->is <Event::MouseEntered>()) { // mouse içeride olduğunda aktiviteleri başlatmak için
                cout << "Mouse iceride" << endl;
            }
            if (event->is <Event::MouseLeft>()) {   // mouse dışarıda olduğunda başlatmak için
                cout << "Mouse Dışarıda " << endl;
            }

        }
    }
 
 
 
 
 */








  /*
   // bu komutlar genelikle sürekli hareletler için (ctrl ile sürekli eğilmek gibi )

   RectangleShape oyuncuKutusu({ 50.f, 50.f });

    oyuncuKutusu.setPosition({ 400.f, 300.f });

    RenderWindow window;
    window.create(VideoMode({ 800,640 }), ("Mayin Tarlasi"));

    while (window.isOpen()) {
        while (const auto Event = window.pollEvent()) {

            window.clear();
            window.draw(oyuncuKutusu); // Kutumuzu ekrana çizdiriyoruz
            window.display();

            if (Event->is < Event::Closed>()) {
                window.close();
            }

            if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
                oyuncuKutusu.move({ -1.f,0.f });
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
                oyuncuKutusu.move({ -3.f,2.f });
            }
            if (Mouse::isButtonPressed(Mouse::Button::Left)) { // ateş etme gibi sol kliğe basınca yadab asılı tutunca silah ateş eder

            }
        }
     }






        return 0;
  
  
  
  
  */