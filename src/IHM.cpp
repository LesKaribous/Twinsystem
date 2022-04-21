#include "IHM.h"
#include "Settings.h"
#include "Pin.h"

namespace IHM
{
	
	const byte _LOGO_KARIBOUS_bits[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x00,
		0x00, 0xff, 0xff, 0x01, 0x00, 0xfe, 0xff, 0x01, 0x80, 0xff, 0xff, 0x01,
		0x00, 0xff, 0xff, 0x00, 0x80, 0xff, 0xff, 0x01, 0x00, 0xff, 0x7f, 0x00,
		0x80, 0xff, 0xff, 0x01, 0x80, 0xff, 0x3f, 0x00, 0x80, 0xff, 0xff, 0x01,
		0xc0, 0xff, 0x3f, 0x00, 0x80, 0xff, 0xff, 0x01, 0xc0, 0xff, 0x1f, 0x00,
		0x80, 0xff, 0xff, 0x01, 0xe0, 0xff, 0x0f, 0x00, 0x80, 0xff, 0xff, 0x01,
		0xf0, 0xff, 0x07, 0x00, 0x80, 0xff, 0xff, 0x01, 0xf0, 0xff, 0x03, 0x00,
		0x80, 0xff, 0xff, 0x01, 0xf8, 0xff, 0x01, 0x00, 0x00, 0xff, 0xff, 0x01,
		0xfc, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x01, 0xfc, 0xff, 0x00, 0x00,
		0x00, 0xff, 0xff, 0x01, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0xff, 0xff, 0x01,
		0xff, 0x3f, 0x00, 0x00, 0x00, 0xff, 0xff, 0x81, 0xff, 0x1f, 0x00, 0x00,
		0x00, 0xff, 0xff, 0x81, 0xff, 0x0f, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc1,
		0xff, 0x07, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe1, 0xff, 0x03, 0x00, 0x00,
		0x00, 0xff, 0xff, 0xf1, 0xff, 0x01, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xf1,
		0xff, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xf9, 0xff, 0x00, 0x00, 0x00,
		0x00, 0xfe, 0xff, 0xfd, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff,
		0x3f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00,
		0x00, 0xfe, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff,
		0x3f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00,
		0x00, 0xfe, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff,
		0x7f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
		0x00, 0xfc, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff,
		0xff, 0x03, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xef, 0xff, 0x03, 0x00, 0x00,
		0x00, 0xfc, 0xff, 0xc7, 0xff, 0x07, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xc1,
		0xff, 0x0f, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x80, 0xff, 0x0f, 0x00, 0x00,
		0x00, 0xf8, 0x7f, 0x80, 0xff, 0x1f, 0x00, 0x00, 0x00, 0xf8, 0x3f, 0x00,
		0xff, 0x3f, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0xff, 0x7f, 0x00, 0x00,
		0x00, 0xf8, 0x0f, 0x00, 0xff, 0x7f, 0x00, 0x00, 0x00, 0xf8, 0x07, 0x00,
		0xfe, 0xff, 0x00, 0x00, 0x00, 0xf8, 0x07, 0x00, 0xfc, 0xff, 0x01, 0x00,
		0x00, 0xf8, 0x07, 0x00, 0xfc, 0xff, 0x03, 0x00, 0x00, 0xf0, 0x07, 0x00,
		0xfc, 0xff, 0x03, 0x00, 0x00, 0xf0, 0x07, 0x00, 0xf8, 0xff, 0x07, 0x00,
		0x00, 0xf0, 0x07, 0x00, 0xf0, 0xff, 0x0f, 0x00, 0x00, 0xf0, 0x07, 0x00,
		0xf0, 0xff, 0x1f, 0x00, 0x00, 0xf0, 0x07, 0x00, 0xe0, 0xff, 0x1f, 0x00,
		0x00, 0xf0, 0x07, 0x00, 0xe0, 0xff, 0x3f, 0x00, 0x00, 0xf0, 0x07, 0x00,
		0xc0, 0xff, 0x7f, 0x00, 0x00, 0xe0, 0x0f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	// Declaration de l'ecran
	U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI 
	_u8g2(U8G2_R3, Pin::Lcd::Cs, Pin::Lcd::Dc, Pin::Lcd::Rs);  

	// Declaration DFPlayerMini
	SoftwareSerial DFPlayerSerial(27, 26); // RX, TX
	DFRobotDFPlayerMini mp3;

	// Logo Karibous
	const byte
		_LOGO_KARIBOUS_width = 64,
		_LOGO_KARIBOUS_height = 64;

	// Declaration des variables IHM
	int _buttonState[8];

	int _page = 0;
	int _tabPage[4];
	const int _nbrPage = 4;

	bool
		_tirette 		= false,
		_detection 		= false,
		_strategie 		= false,
		_check 			= false,
		_robot 			= false,
		_lidar 			= false,	
		_recalage 		= false,
		_arrowUp		= false,
		_arrowDown		= false,
		_prevUpState 	= false,
		_prevDownState 	= false,	
		_typeRobot 		= Settings::PRIMARY,
		_equipe 		= Settings::EQUIPE_JAUNE,
		_testBras 		= false,	
		_optionStrat01	= false,
		_optionStrat02	= false,
		_optionStrat03	= false,
		freezed 		= false;

	// TODO -> Change list
	// Liste d'action de la check list:
	char *_titreList[6] = {
		"Robot",
		"Experi.",
		"Palets",
		"Table",
		"Balise",
		"GO !"};

	// TODO -> Change list
	// Liste d'action de la check list:
	char *_list[6] = {
		"Installer le robot secondaire ",
		"Installer l'experience        ",
		"Verifier les palets           ",
		"Nettoyer la table             ",
		"Poser la balise adversaire    ",
		"Bon match !                   "};

void init()
{
	pinMode(Pin::tirette		, INPUT_PULLUP);
	pinMode(Pin::checkButton	, INPUT_PULLUP);

	pinMode(Pin::latchMux	, OUTPUT);
	pinMode(Pin::clockMux	, OUTPUT);
	pinMode(Pin::dataMux	, INPUT);

	digitalWrite(Pin::latchMux, LOW);
	digitalWrite(Pin::clockMux, LOW);

	// Init and configuration of the MP3 module DFPlayer Mini
	DFPlayerSerial.begin(9600);
	mp3.begin(DFPlayerSerial);
	Debugger::log("DFPlayer launched");

	mp3.setTimeOut(500);
	mp3.volume(29);
	mp3.EQ(DFPLAYER_EQ_NORMAL);

	// Init and configuration for the LCD
	_u8g2.begin(); 
	_u8g2.setFontRefHeightExtendedText();
	_u8g2.setDrawColor(1);
	_u8g2.setFontPosTop();
	_u8g2.setFontDirection(0);

	// Splashscreen with the K logo
	LCD::splashScreen();
}

void menu(){
	updateButtonIHM();
	LCD::startMenu();
	//LCD::menuScreen();
}

//----------------GESTION DES BOUTTONS DE L'IHM----------------
void updateButtonIHM()
{
	readButtonState();
	getArrowUp();
	getArrowDown();
	getTirette();
	getDetection();
	getRobot();
	getStrategie();
	getOption01();
	getOption02();
	getOption03();
	getCheck();
	getEquipe();
}

void readButtonState()
{
	digitalWrite(Pin::latchMux, LOW	);
	digitalWrite(Pin::latchMux, HIGH);
	for (int i = 0; i <= 7; i++)
	{
		// on note l'état de la sortie du HC165
		_buttonState[7 - i] = digitalRead(Pin::dataMux);
		// et on envoi un front montant sur la pin 2 pour décaler les valeurs
		digitalWrite(Pin::clockMux, HIGH);
		digitalWrite(Pin::clockMux, LOW);
	}
}

//---Gestion Etat Robot---
void setRecalage(bool state){
	//if(!freezed) ??
		_recalage = state;
}
bool getRecalage(){
	return _recalage;
}
void freezeSettings(){
	freezed = true;
}
//---Gestion Boutons physiques---
bool getTirette(){
	if(!freezed)
		_tirette = !digitalRead(Pin::tirette);
	return _tirette;
}
bool getRobot(){
	if(!freezed) 
		_robot = digitalRead(Pin::robotSelect);
	return _robot;
}
bool getCheck(){
	if(!freezed)
		_check = digitalRead(Pin::checkButton);
	return _check;
}
//---Gestion Boutons Multiplexeur
bool getArrowUp()
{
	if(!freezed) 
		_arrowUp 	= !_buttonState[4];
	return _arrowUp;
}
bool getEquipe(){
	if(!freezed)
		_equipe = _buttonState[5];
	return _equipe;
}
bool getDetection(){
	if(!freezed) 
		_detection = _buttonState[6];
	return _detection;
}
bool getStrategie(){
	if(!freezed)
		_strategie = _buttonState[7];
	return _strategie;
}
bool getOption01()
{
	if(!freezed) 
		_optionStrat01 = _buttonState[0];
	return _optionStrat01;
}
bool getOption02()
{
	if(!freezed) 
		_optionStrat02 = _buttonState[1];
	return _optionStrat02;
}
bool getOption03()
{
	if(!freezed) 
		_optionStrat03 = _buttonState[2];
	return _optionStrat03;
}
bool getArrowDown()
{
	if(!freezed) 
		_arrowDown 	= !_buttonState[3];
	return _arrowDown;
}

	namespace LCD{
		//----------------GESTION DES PAGES LCD-------------------
		void splashScreen()
		{
			// Lecture de son
			Sound::playSound(LOADING_SOUND);
			//Demarrage affichage
			_u8g2.clearBuffer();
			// Affichage du menu de base
			baseMenu();

			// Affichage de l'image K
			_u8g2.setDrawColor(1);
			_u8g2.setBitmapMode(0);
			_u8g2.drawXBMP(0, 32, _LOGO_KARIBOUS_width, _LOGO_KARIBOUS_height, _LOGO_KARIBOUS_bits);

			_u8g2.sendBuffer();

			delay(2000);
		}

		void baseMenu(){

			_u8g2.setFont(u8g2_font_unifont_t_75);
			_u8g2.setFontPosCenter();
			// Fleche de menu
			if (_arrowUp) {
				_u8g2.drawGlyph(57, 2, 0x25b2);
				_prevUpState = true ;
			}
			else {
				_u8g2.drawGlyph(57, 2, 0x25b3);
				if (_prevUpState) {
					_prevUpState = false ;
					_page++;
					Sound::playSound(UP_SOUND);
					if (_page == _nbrPage) _page = 0;
				}
			}

			if (_arrowDown) {
				_u8g2.drawGlyph(57, 123, 0x25bc);
				_prevDownState = true ;
			}
			else {
				_u8g2.drawGlyph(57, 123, 0x25bd);
				if (_prevDownState) {
					_prevDownState = false ;
					_page--;
					Sound::playSound(DOWN_SOUND);
					if (_page < 0) _page = _nbrPage-1;
				}
			}

			// Indicateur de Page
			// Raz marqueur page
			for(int i=0;i<_nbrPage;i++) _tabPage[i] = 0x25ab ;
			// Selection page 
			_tabPage[_page] = 0x25ae ;
			// Affichage des breadCrum
			for(int i=0;i<_nbrPage;i++){
				_u8g2.drawGlyph(15+i*5, 2, _tabPage[i]);
			}

			// Afficher info version
			// TODO
			// Affichages de la date de compilation
			_u8g2.setFont(u8g2_font_micro_mr);
			_u8g2.setFontPosTop();
			_u8g2.drawStr(1, 111, __DATE__);
			_u8g2.drawStr(1, 119, __TIME__);
		}

		void startMenu(){
			_u8g2.clearBuffer();
			// Affichage du menu de base
			baseMenu();
			// Affichage de la page
			affichePage();

			_u8g2.sendBuffer();
		}

		void affichePage(){
			switch (_page) {
				case 0:
					page01();
					break;
				case 1:
					page02();
					break;
				default:
					_u8g2.setFont(u8g2_font_5x7_mr);
					_u8g2.drawStr(2, 60, "error 404");
					break;
			}
		}

		void page01(){
			// Alignements
			const int marginLeft = 2;
			int wChaine;
			const int yTirette 		= 20;
			const int yLidar 		= 32;
			const int yRecalage 	= 44;
			const int yInferieur	= 60;
			const int xBox	= 50;

			// Affichage etat Tirette
			_u8g2.setFont(u8g2_font_micro_mr);
			_u8g2.drawStr(marginLeft, yTirette, "Tirette ");
			_u8g2.setFont(u8g2_font_m2icon_9_tf);
			if (_tirette)
				_u8g2.drawGlyph(xBox, yTirette, 0x0046);
			else
				_u8g2.drawGlyph(xBox, yTirette, 0x0045);

			// Affichage etat Lidar
			_u8g2.setFont(u8g2_font_micro_mr);
			_u8g2.drawStr(marginLeft, yLidar, "Lidar ");
			_u8g2.setFont(u8g2_font_m2icon_9_tf);
			if (_lidar)
				_u8g2.drawGlyph(xBox, yLidar, 0x0046);
			else
				_u8g2.drawGlyph(xBox, yLidar, 0x0045);

			// Affichage etat recalage départ
			_u8g2.setFont(u8g2_font_micro_mr);
			_u8g2.drawStr(marginLeft, yRecalage, "Recalage ");
			_u8g2.setFont(u8g2_font_m2icon_9_tf);
			if (_recalage)
				_u8g2.drawGlyph(xBox, yRecalage, 0x0046);
			else
				_u8g2.drawGlyph(xBox, yRecalage, 0x0045);

			// Partie inférieure
			_u8g2.setFont(u8g2_font_micro_mr);
			_u8g2.drawStr(0, yInferieur, "---------------");
			_u8g2.drawStr(marginLeft, yInferieur + 10, "    X:         ");
			_u8g2.drawStr(marginLeft, yInferieur + 20, "    Y:         ");
			_u8g2.drawStr(marginLeft, yInferieur + 30, "alpha:      deg");
			_u8g2.drawStr(0, yInferieur + 40, "---------------");

		}

		void page02(){
			// Alignements
			const int marginLeft = 2;
			int wChaine;
			const int yTeam = 21;
			const int yAvoid = 37;
			const int yStrat = 53;
			const int yOption01 = 69;
			const int yOption02 = 85;
			const int yOption03 = 101;

			_u8g2.setFont(u8g2_font_micro_mr);
			_u8g2.setFontPosCenter();

			//Gestion equipe
			wChaine = _u8g2.drawStr(marginLeft, yTeam, "Equipe");
			if (_equipe == Settings::EQUIPE_JAUNE)
				_u8g2.drawStr(marginLeft + wChaine, yTeam, " jaune");
			else
				_u8g2.drawStr(marginLeft + wChaine, yTeam, " violet");
			
			//Gestion evitement
			wChaine = _u8g2.drawStr(marginLeft, yAvoid, "Evit.");
			if (_detection)
				_u8g2.drawStr(marginLeft + wChaine, yAvoid, " simple");
			else
				_u8g2.drawStr(marginLeft + wChaine, yAvoid, " complexe");

			//Gestion Stratégie
			wChaine = _u8g2.drawStr(marginLeft, yStrat, "Strat.");
			if (_strategie)
				_u8g2.drawStr(marginLeft + wChaine, yStrat, " homologation");
			else
				_u8g2.drawStr(marginLeft + wChaine, yStrat, " match");

			//Gestion Option 01 Stratégie
			_u8g2.setFont(u8g2_font_micro_mr);
			wChaine = _u8g2.drawStr(marginLeft, yOption01, " - Option 01 ");
			_u8g2.setFont(u8g2_font_m2icon_9_tf);
			if (_optionStrat01)
				_u8g2.drawGlyph(marginLeft + wChaine, yOption01, 0x0045);
			else
				_u8g2.drawGlyph(marginLeft + wChaine, yOption01, 0x0046);

			//Gestion Option 02 Stratégie
			_u8g2.setFont(u8g2_font_micro_mr);
			wChaine = _u8g2.drawStr(marginLeft, yOption02, " - Option 02 ");
			_u8g2.setFont(u8g2_font_m2icon_9_tf);
			if (_optionStrat02)
				_u8g2.drawGlyph(marginLeft + wChaine, yOption02, 0x0045);
			else
				_u8g2.drawGlyph(marginLeft + wChaine, yOption02, 0x0046);

			//Gestion Option 03 Stratégie
			_u8g2.setFont(u8g2_font_micro_mr);
			wChaine = _u8g2.drawStr(marginLeft, yOption03, " - Option 03 ");
			_u8g2.setFont(u8g2_font_m2icon_9_tf);
			if (_optionStrat03)
				_u8g2.drawGlyph(marginLeft + wChaine, yOption03, 0x0045);
			else
				_u8g2.drawGlyph(marginLeft + wChaine, yOption03, 0x0046);

		}
		
		void initScreen()
		{
			_u8g2.clearBuffer();
			_u8g2.setFont(u8g2_font_logisoso32_tr);
			_u8g2.drawStr(3, 2, "Init");
			_u8g2.sendBuffer();
			delay(1500);
		}

		void checkListScreen()
		{
			for (int i = 0; i < 6; i++)
			{
				int x = 128; //On commence le texte à droite
				do
				{
					_u8g2.clearBuffer();

					_u8g2.drawBox(22, 0, 2, 33); //Ligne de séparation numéro
					for (int j = 0; j <= 128; j = j + 4)
						_u8g2.drawBox(j, 40, 2, 1); //Ligne de séparation texte - tirets

					_u8g2.setFont(u8g2_font_logisoso32_tn); //Changer la font pour le numero
					_u8g2.setCursor(0, 0);
					_u8g2.print(i + 1); //Afficher le numero de l'action

					_u8g2.setFont(u8g2_font_courB08_tf); //Changer la font pour la description
					_u8g2.drawStr(28, 0, "Action :");	 //Afficher le titre de l'action

					_u8g2.setFont(u8g2_font_logisoso22_tf); //Changer la font pour la description
					_u8g2.drawStr(25, 10, _titreList[i]);	//Afficher le titre de l'action

					_u8g2.setFont(u8g2_font_courB08_tf); //Changer la font pour la description
					_u8g2.drawStr(x, 48, _list[i]);		 //Afficher l'action

					_u8g2.sendBuffer();
					delay(100);
					x -= 10; //Scrolling
				} while (getCheck());
				delay(1000);
			}
		}

		void goScreen()
		{
			_u8g2.clearBuffer();
			_u8g2.setFont(u8g2_font_logisoso32_tr);
			_u8g2.drawStr(5, 48, "GO!");
			_u8g2.sendBuffer();
			delay(1000);
		}

		void matchScreen(int score, int tempsRestant, int nbrBadCRC)
		{
			_u8g2.clearBuffer();
			// Titre
			_u8g2.setFont(u8g2_font_logisoso16_tr);
			_u8g2.drawStr(8, 1, "Match");
			_u8g2.drawHLine(0, 20, 64);

			// Affichage partie Score
			_u8g2.setFont(u8g2_font_micro_mr);
			_u8g2.drawStr(4, 23, "Score:");
			_u8g2.drawStr(38, 66, "points");

			// Score Chiffre
			_u8g2.setFont(u8g2_font_logisoso32_tr);
			_u8g2.setCursor(2, 31);
			_u8g2.print(score);

			_u8g2.drawHLine(0, 75, 64);

			// Partie inférieure
			_u8g2.setFont(u8g2_font_micro_mr);
			_u8g2.drawStr(3, 77, "Temps:      sec");
			_u8g2.drawStr(3, 84, "    X:         ");
			_u8g2.drawStr(3, 91, "    Y:         ");
			_u8g2.drawStr(3, 98, "alpha:      deg");

			_u8g2.setCursor(36, 77);
			_u8g2.print(tempsRestant);

			// Erreurs
			_u8g2.drawHLine(0, 119, 64);
			_u8g2.drawStr(0, 120, "erreurs :");
			_u8g2.setCursor(40, 120);
			_u8g2.print(nbrBadCRC);

			_u8g2.sendBuffer();
		}

		#ifdef DANSE
		void danseScreen(){
			_u8g2.clearBuffer();
			// Titre
			_u8g2.setFont(u8g2_font_logisoso16_tr);
			_u8g2.drawStr(8, 1, "Danse !");

			_u8g2.sendBuffer();
		}
		#endif
	}

	namespace Sound{

		void playSound (int soundfile){
			mp3.playMp3Folder(soundfile);
		}

		void switchSound (bool state){
			if(state) Sound::playSound(SWITCH_RIGHT_SOUND);
			else Sound::playSound(SWITCH_LEFT_SOUND);
		}
	}

}

