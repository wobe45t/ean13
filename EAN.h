#include <iostream>
using namespace std;

class EAN
{
public:
  const string sideBars = "101";
  const string middleBars = "01010";
  const int height = 200;
  const int width = 400;
  const int bytes = 3;
  const int bottomEanHeight = height / 4;
  const int topEanHeight = height / 2 + bottomEanHeight;
  int currentWidthPixel;
  unsigned char ***image;
  char *imageFileName;
  string eanCode;

  EAN();
  string encode(int firstDigit, int index);
  void drawFirstGroupBars(string digits, string encoding);
  void drawSecondGroupBars(string digits);
  void drawSideBars();
  void drawMiddleBars();
  int calculateCheckSum();
  void generateBitmap();
  string encodeDigitL(char digit);
  string encodeDigitG(char digit);
  string encodeDigitR(char digit);
  void drawCode(string bits);
  void drawCharacter(unsigned char *character, int width, int height);
  void drawEanCharacters();
};
