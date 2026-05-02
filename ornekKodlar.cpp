




// AÇı fonksiyonları 
	/*
	using namespace sf; her nesne tanımlama sf:: yazmamızı engeller büyük projelerde tavsyie edilmez
	Angle angle1 = degrees(600); // derece
	Angle angle2 = radians(6.12); // radians
	Angle angle3 = degrees(720);

	angle1 = angle1.wrapUnsigned(); // 0 ile 2pi arasına
	angle3 = angle3.wrapSigned(); // -pi ile pi aralığı arasında

	cout << angle1.asDegrees() << endl;

	cout << sin(angle1.asDegrees()) << endl;

	cout << angle2.asDegrees() << endl;
	cout << cos(angle2.asRadians()) << endl;

	angle2 /= 3.f;
	cout << angle2.asDegrees() << endl;
	*/







	// time 

	  /*
	    
		Time time1 = seconds(10.0f);
	Time time2 = milliseconds(1000);
	Time time3 = microseconds(1000000);

	cout << "time is:" << time1.asSeconds() << endl;  // asSecond
	cout << "time is:" << time2.asSeconds() << endl,  // asmilliSecond
	cout << "time is:" << time3.asSeconds() << endl;  // asMicrosecond


	Time t = chrono::milliseconds(1000); //  eğer explicit (açık) anahtar kelimesi ile işaretlenmemiş ise derleyici bunları veri tipine dönüştürebilir
										// chorono bir kurucu metottur
	cout << "chrono time is :" << t.asSeconds() << endl; // asSecond ile nesneden float birime dönüştürüldü metot  overloading ile engelenebilir

	time1 = time2 + time3;

	cout << "time is " << time1.asSeconds() << endl; // nesneler arası işlem yapılabilir

	Clock clock;
	clock.start();  // başlatır


	Time enapled1 = clock.getElapsedTime();
	cout << "Clock time is " << enapled1.asSeconds() << endl; // float olarak döndürür
	clock.restart(); // zamanı sıfırlar ve float olarak döndürür

	Time enapled2 = clock.getElapsedTime(); // süreyi float olarak döndürür
	cout << "Clock time is" << enapled2.asSeconds() << endl;

	clock.stop(); // sayacı dondurur

	cout << boolalpha << clock.isRunning() << endl;
	clock.reset();  // resetler

	clock.start();  // başlatır
	Time enapled3 = clock.getElapsedTime();
	cout << "clock time is " << enapled3.asSeconds() << endl;


	  */










   // window sınıfı
    /*
	  // Ekran Açma Ve Yönetme

	   Window window(VideoMode({ 800,600 }), "Mayin Tarlasi"); // pencere oluşturma basic code ayrıca cons
	window.setVerticalSyncEnabled(true);  // monitor hertz ile ekrandaki hertzi eşitler böylece tearing engellenir
	 window.setFramerateLimit(60); // unsigned int limit  en fazla kaç kare üretebileceğini sınırlar setVertcialSyncEnabled ile aynı anda kullanılmamalı yoksa çakışma yarabilir
	Style::Default;




	while (window.isOpen()) { // Program, pencere açık olduğu sürece bu döngüde çalışır. çarpı tuşuna basılırsa program false döndürür kapanır
		while (const auto event = window.pollEvent()) {  // pollEvent sırası ile gerçekleşen olayları alır  olay var ise dolu yok ise nullopt değer döndürür

			if (event->is <Event::Closed>()) {  // kullanıcı pencereyi kapatmak istiyorsa
				window.close();
			}

		};


	};
   // Style::Titlebar;//başlık çubuğu ekler
   // Style::Resize; // boyutlandırma seçeneği ekler
   // Style::None;   // hiç bir şey eklemez
   // Style::Close; // kapatma tuşu ekler

	 // State::Windowed  pencereli
		State::Fullscreen;// tam ekran

		 /* altarnatif oalrak bu seçenekte kullanaılabilir  eğer ekran hemen açılması istenilmez ise

			Window window
			window.create(VideoMode({ 800,600 }), "Mayin Tarlasi")


		 
window.setPosition({ 10,50 });
window.setSize({ 640,800 });    // 
window.setTitle("MayinTarlasi"); // başlık

bool focus = window.hasFocus(); // kullanıcının kullandığı pencere aktif mi değil mi ona bakar ekran aktif ise true döndürür




// pencere kontrollerini farklı bir kütüphane kullanarak  kontrol edebiliriz
	
	*/
	
	