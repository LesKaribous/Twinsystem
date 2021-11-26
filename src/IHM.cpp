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
	U8G2_ST7920_128X64_F_SW_SPI
	_u8g2(U8G2_R3, 13, 11, 12, U8X8_PIN_NONE);

	// Logo Karibous
	const byte
		_LOGO_KARIBOUS_width = 64,
		_LOGO_KARIBOUS_height = 64;

	// Declaration des variables IHM
	int _buttonState[8];

	bool
		_tirette = false,
		_detection = false,
		_strategie = false,
		_check = false,
		_recalage = false,
		_typeRobot = Setting::ROBOT_PRIMAIRE,
		_equipe = Setting::EQUIPE_BLEU,
		_testBras = false,
		_oppenentFront = false,
		_oppenentBack  = true,
		freezed = false;

	// Liste d'action de la check list:
	char *_titreList[6] = {
		"Robot",
		"Experi.",
		"Palets",
		"Table",
		"Balise",
		"GO !"};

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
	pinMode(Pin::Tirette, INPUT_PULLUP);

	pinMode(Pin::latchMux, OUTPUT);
	pinMode(Pin::clockMux, OUTPUT);
	pinMode(Pin::clockInhMux, OUTPUT);
	pinMode(Pin::dataMux, INPUT);

	digitalWrite(Pin::latchMux, HIGH);
	digitalWrite(Pin::clockMux, LOW);

	_u8g2.begin(); //Init du LCD
	_u8g2.setFontRefHeightExtendedText();
	_u8g2.setDrawColor(1);
	_u8g2.setFontPosTop();
	_u8g2.setFontDirection(0);

	LCD::splashScreen();
}

//----------------GESTION DES BOUTTONS DE L'IHM----------------
void updateButtonIHM()
{
	readButtonState();
	getDetection();
	getStrategie();
	getCheck();
	getEquipe();
	getTestBras();
}

void readButtonState()
{
	digitalWrite(Pin::latchMux, LOW);
	digitalWrite(Pin::latchMux, HIGH);
	for (int i = 0; i <= 7; i++)
	{
		// on note l'état de la sortie (pin 9) du HC165
		_buttonState[7 - i] = digitalRead(Pin::dataMux);
		// et on envoi un front montant sur la pin 2 pour décaler les valeurs
		digitalWrite(Pin::clockMux, HIGH);
		digitalWrite(Pin::clockMux, LOW);
	}
}

void freezeSettings(){
	freezed = true;
}

bool getTirette(){
	if(!freezed){
		if (analogRead(Pin::Tirette) < 10)
			_tirette = false;
		else
			_tirette = true;
		return _tirette;
	}else return _tirette;
}

bool getDetection()
{
	if(!freezed) 
		_detection = _buttonState[0];
	return _detection;
}
bool getStrategie(){
	if(!freezed)
		_strategie = _buttonState[2];
	return _strategie;
}
bool getCheck(){
	if(!freezed)
		_check = _buttonState[5];
	return _check;
}
bool getEquipe(){
	if(!freezed)
		_equipe = _buttonState[1];
	return _equipe;
}
void setRecalage(bool state){
	//if(!freezed) ??
		_recalage = state;
}
bool getRecalage(){
	return _recalage;
}
bool getTestBras(){
	if(!freezed)
		_testBras = _buttonState[6];
	return _testBras;
}

void setOpponent(int op){
	_oppenentFront = (op == 1) ? true : false;
	_oppenentBack= (op == -1) ? true : false;
}

bool getOpponent(){
	return _oppenentFront;
}


	namespace LCD{
		//----------------GESTION DES PAGES LCD-------------------
		void splashScreen()
		{
			_u8g2.clearBuffer();
			// Affichage de l'image K
			_u8g2.setDrawColor(1);
			_u8g2.setBitmapMode(0);
			_u8g2.drawXBMP(0, 32, _LOGO_KARIBOUS_width, _LOGO_KARIBOUS_height, _LOGO_KARIBOUS_bits);

			// Affichages de la date de compilation
			_u8g2.setFont(u8g2_font_micro_mr);
			_u8g2.drawStr(5, 110, __DATE__);
			_u8g2.drawStr(5, 118, __TIME__);

			_u8g2.sendBuffer();
		}

		void menuScreen(){
			const int ligneEtat = 0;
			const int ligneStrategie = 64;
			const int colonne1 = 2;

			_u8g2.clearBuffer();

			// Affichages des titres :
			_u8g2.setFont(u8g2_font_5x7_mr);

			_u8g2.drawStr(colonne1, ligneEtat, " -- ETAT --");
			_u8g2.drawStr(colonne1 + 3, ligneStrategie, "-- STRAT --");

			// Affichages des sous-titres :
			_u8g2.setFont(u8g2_font_micro_mr);

			_u8g2.drawStr(colonne1 + 15, ligneEtat + 10, "robot ");
			_u8g2.drawStr(colonne1 + 10, ligneEtat + 20, "Tirette");
			_u8g2.drawStr(colonne1 + 10, ligneEtat + 30, "Balise");
			_u8g2.drawStr(colonne1 + 10, ligneEtat + 40, "Recalage");

			_u8g2.drawStr(colonne1, ligneStrategie + 10, "Equipe");

			// Etat type de robot :
			_u8g2.setCursor(colonne1, ligneEtat + 10);
			if (_typeRobot == Setting::ROBOT_PRIMAIRE)
				_u8g2.print("1st");
			else
				_u8g2.print("2nd");
			// Etat equipe :
			_u8g2.setCursor(colonne1 + 28, ligneStrategie + 10);
			if (_equipe == Setting::EQUIPE_JAUNE)
				_u8g2.print("jaune");
			else
				_u8g2.print("bleu");
			// Etat strategie :
			_u8g2.setCursor(colonne1, ligneStrategie + 20);
			if (_strategie)
				_u8g2.print("Homologation");
			else
				_u8g2.print("Match");

			// Symboles :
			_u8g2.setFont(u8g2_font_m2icon_9_tf);
			// Etat tirette :
			if (_tirette)
				_u8g2.drawGlyph(colonne1, ligneEtat + 20 - 1, 0x0045);
			else
				_u8g2.drawGlyph(colonne1, ligneEtat + 20 - 1, 0x0046);
			// Etat balise :
			if (!_detection)
				_u8g2.drawGlyph(colonne1, ligneEtat + 30 - 1, 0x0045);
			else
				_u8g2.drawGlyph(colonne1, ligneEtat + 30 - 1, 0x0046);
			// Etat recalage :
			if (!_recalage)
				_u8g2.drawGlyph(colonne1, ligneEtat + 40 - 1, 0x0045);
			else
				_u8g2.drawGlyph(colonne1, ligneEtat + 40 - 1, 0x0046);
			// Validation de départ :
			_u8g2.setFont(u8g2_font_open_iconic_check_2x_t);
			if (!_tirette && _detection && _recalage)
				_u8g2.drawGlyph(44, 10, 0x41);
			else
				_u8g2.drawGlyph(44, 10, 0x42);

			_u8g2.sendBuffer();
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

			if (_oppenentFront)
			_u8g2.drawStr(3, 105, "Opp: Front");
			else if(_oppenentBack)
			_u8g2.drawStr(3, 105, "Opp: Behind");
			else
			_u8g2.drawStr(3, 105, "Opp: No");

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
}
