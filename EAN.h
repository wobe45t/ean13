#include <iostream>
using namespace std;

class EAN
{
public:
  //Zmienne określające strukturę generowanego kodu kreskowego
  const int height = 200;
  const int width = 400;
  const int bytes = 3;
  const int bottomEanHeight = height / 4;
  const int topEanHeight = height / 2 + bottomEanHeight;

  //Zmienna przechowująca aktualny indeks piksela na którym rysowane są moduły
  int currentWidthPixel;
  //Zmienna przechowująca składowe R,G,B (Red, Green, Blue) do wygenerowania bitmapy
  unsigned char ***image;
  char *imageFileName;
  string eanCode;

  EAN();
  string encode(int firstDigit);
  void drawFirstGroupBars(string digits, string encoding);
  void drawSecondGroupBars(string digits);
  int calculateCheckSum();
  void generateBitmap();
  void generatePdf();
  string encodeDigitL(char digit);
  string encodeDigitG(char digit);
  string encodeDigitR(char digit);
  void drawCode(string bits);
  void drawCharacter(unsigned char *character, int width, int height);
  void drawEanCharacters();
};
