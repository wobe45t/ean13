#include "EAN.h"
#include "Bitmap.h"
#include "characters.h"
#include <iostream>
#include <bitset>
using namespace std;

EAN::EAN()
{

  const int width = 200;
  const int height = 400;
  const int bytes = 3;
  currentWidthPixel = 20;
  image = new unsigned char **[width];
  for (int i = 0; i < width; i++)
  {
    image[i] = new unsigned char *[height];
    for (int j = 0; j < height; j++)
    {
      image[i][j] = new unsigned char[bytes];
      for (int k = 0; k < bytes; k++)
      {
        image[i][j][k] = 255;
      }
    }
  }

  imageFileName = (char *)"bitmapImage.bmp";

  cout << "Podaj 12 cyfr kodu EAN : ";
  cin >> eanCode;
  char digit = calculateCheckSum() + '0';
  eanCode += digit;
}

string EAN::encode(int firstDigit, int index)
{
  string result = "";
  switch (firstDigit)
  {
  case 0:
    result = "LLLLLL";
    break;
  case 1:
    result = "LLGLGG";
    break;
  case 2:
    result = "LLGGLG";
    break;
  case 3:
    result = "LLGGGL";
    break;
  case 4:
    result = "LGLLGG";
    break;
  case 5:
    result = "LGGLLG";
    break;
  case 6:
    result = "LGGGLL";
    break;
  case 7:
    result = "LGLGLG";
    break;
  case 8:
    result = "LGLGGL";
    break;
  case 9:
    result = "LGGLGL";
    break;
  }
  return result;
}

string EAN::encodeDigitL(char digit)
{
  string result;
  switch (digit)
  {
  case '0':
    result = "0001101";
    break;
  case '1':
    result = "0011001";
    break;
  case '2':
    result = "0010011";
    break;
  case '3':
    result = "0111101";
    break;
  case '4':
    result = "0100011";
    break;
  case '5':
    result = "0110001";
    break;
  case '6':
    result = "0101111";
    break;
  case '7':
    result = "0111011";
    break;
  case '8':
    result = "0110111";
    break;
  case '9':
    result = "0001011";
    break;
  }
  return result;
}
string EAN::encodeDigitG(char digit)
{
  string result;
  switch (digit)
  {
  case '0':
    result = "0100111";
    break;
  case '1':
    result = "0110011";
    break;
  case '2':
    result = "0011011";
    break;
  case '3':
    result = "0100001";
    break;
  case '4':
    result = "0011101";
    break;
  case '5':
    result = "0111001";
    break;
  case '6':
    result = "0000101";
    break;
  case '7':
    result = "0010001";
    break;
  case '8':
    result = "0001001";
    break;
  case '9':
    result = "0010111";
    break;
  }
  return result;
}
string EAN::encodeDigitR(char digit)
{
  string result;
  switch (digit)
  {
  case '0':
    result = "1110010";
    break;
  case '1':
    result = "1100110";
    break;
  case '2':
    result = "1101100";
    break;
  case '3':
    result = "1000010";
    break;
  case '4':
    result = "1011100";
    break;
  case '5':
    result = "1001110";
    break;
  case '6':
    result = "1010000";
    break;
  case '7':
    result = "1000100";
    break;
  case '8':
    result = "1001000";
    break;
  case '9':
    result = "1110100";
    break;
  }
  return result;
}
void EAN::drawCode(string bits)
{
  for (int i = 0; i < bits.size(); i++)
  {
    if (bits[i] == '0')
    {
      currentWidthPixel += 3;
    }
    else
    {
      for (int j = bottomEanHeight; j < topEanHeight; j++)
      {
        image[j][currentWidthPixel] = (unsigned char *)"000000";
        image[j][currentWidthPixel + 1] = (unsigned char *)"000000";
        image[j][currentWidthPixel + 2] = (unsigned char *)"000000";
      }
      currentWidthPixel += 3;
    }
  }
}
void EAN::drawCharacter(unsigned char *character, int width, int height)
{
  string temp;
  for (int i = 0; i < 8; i++)
  {
    std::bitset<8> x(character[i]);
    temp = x.to_string();
    for (int j = 0; j < 8; j++)
    {
      if (temp[j] == '1')
      {
        image[height - i][width - j] = (unsigned char *)"000000";
      }
    }
  }
}
void EAN::drawEanCharacters()
{
  Characters *chars = new Characters();
  int height = bottomEanHeight - 10;
  int width = 45;
  drawCharacter(chars->bitmapCharacters[eanCode[0] - '0'], 28, height);
  width += 10;
  for (int i = 1; i < 7; i++)
  {
    drawCharacter(chars->bitmapCharacters[eanCode[i] - '0'], width - 11, height);
    width += 21;
  }
  width += 15;
  for (int i = 7; i < 13; i++)
  {
    drawCharacter(chars->bitmapCharacters[eanCode[i] - '0'], width - 11, height);
    width += 21;
  }
}
void EAN::drawFirstGroupBars(string digits, string encoding)
{
  string bits;
  for (int i = 0; i < 6; i++)
  {
    if (encoding[i] == 'L')
    {
      bits = encodeDigitL(digits[i]);
      cout << "drawing L code for digit " << digits[i] << " - " << bits << endl;
      drawCode(bits);
    }
    else if (encoding[i] == 'G')
    {
      bits = encodeDigitG(digits[i]);
      cout << "drawing G code for digit " << digits[i] << " - " << bits << endl;
      drawCode(bits);
    }
  }
}

void EAN::drawSecondGroupBars(string digits)
{
  for (int i = 0; i < 6; i++)
  {
    drawCode(encodeDigitR(digits[i]));
  }
}

int EAN::calculateCheckSum()
{
  int temp;
  int sum = 0;
  for (int i = 0; i < 12; i++)
  {
    temp = eanCode[i] - '0';
    if (i % 2 == 0)
    {
      sum += temp;
    }
    else
    {
      sum += temp * 3;
    }
  }
  int i = 0;
  int value = 0;
  while (value < sum)
  {
    ++i;
    value += 10;
  }
  return value - sum;
}
void EAN::generateBitmap()
{
  Bitmap *bitmap = new Bitmap();
  Characters *chars = new Characters();
  string firstGroupEncoding;
  string firstGroupDigits;
  string secondGroupDigits;
  int digit;
  digit = eanCode[0] - '0';
  firstGroupEncoding = encode(digit, 0);
  firstGroupDigits = eanCode.substr(1, 6);
  secondGroupDigits = eanCode.substr(7, 12);
  cout << "first group digits : " << firstGroupDigits << endl;
  cout << "second group digits : " << secondGroupDigits << endl;
  cout << "first group encoding : " << firstGroupEncoding << endl;

  drawCode("101");
  drawFirstGroupBars(firstGroupDigits, firstGroupEncoding);
  drawCode("01010");
  drawSecondGroupBars(secondGroupDigits);
  drawCode("101");

  drawEanCharacters();

  bitmap->generateBitmapImage(image, height, width, imageFileName);
}