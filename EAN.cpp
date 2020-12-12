#include "EAN.h"
#include "Bitmap.h"
#include "characters.h"
#include <iostream>
#include <bitset>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <complex>
#include "pdf.h"
#include <vector>
using namespace std;

//Konstruktor który inicjalizuje elementy macierzy 2D składowymi RGB(255,255,255) - kolor biały
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
  //Nazwa bliku
  imageFileName = (char *)"ean.bmp";

  //Wprowadzenie danych przez użytkownika
  cout << "Podaj 12 cyfr kodu EAN : ";
  cin >> eanCode;
  while (eanCode.size() != 12)
  {
    cout << "[Blad] Podaj kod ponownie: ";
    cin >> eanCode;
  }
  //Obliczanie sumy kontrolnej, dodawnie kodu '0' w celu zmiany int na char
  // i dodanie do kodu na 13 pozycję
  char digit = calculateCheckSum() + '0';
  eanCode += digit;
}
//Generowanie kodowania dla pierwszej grupy cyfr (pozycje 2-7) na podstawie pierwszej cyfry kodu.
string EAN::encode(int firstDigit)
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
//Kodowanie cyfry kodem L
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
//Kodowanie cyfry kodem G
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
//Kodowanie cyfry kodem R
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

// Funkcja rysująca zakodowane cyfry. Dla każdego bitu sprawdzane jest czy jest on równy 1
// Jeżeli tak, generowany jest czarny moduł o szerokości 3px. W przeciwnym razie nie rysujemy
// (tło jest białe domyślnie). Zwiększana jest zmienna przechwująca pozycję rysowania modułu
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
//Funckja rysująca jeden ze znaków zdefiniowanych w klasie Characters
//znaki są w postaci 8x8 pixeli - 8 kolejnych bajtów. Potrzebne jest zrzutowanie
// na ciąg binarny i sprawdzenie ustawionych jedynek. Wtedy rysowany jest czarny pixel.
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
// Funkcja rysująca cyfry kodu na określonych (na sztywno) pozycjach.
// Znaki pobierane są z kodu ean wpisanego przez użytkownika a następnie
// pobierany jest odpowiedni znak z kalsy Characters
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
// Funckja (wrapper) która sprawdza otrzymane kodowanie pierwszej grupy cyfr i
// w zależności od litery reprezentującej kod (L,G) koduje cyfrę i przekazuje kod
// do funkcji rysującej moduły.
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
// Funckja (wrapper) która koduje cyfrę i przekazuje kod
// do funkcji rysującej moduły.
void EAN::drawSecondGroupBars(string digits)
{
  for (int i = 0; i < 6; i++)
  {
    drawCode(encodeDigitR(digits[i]));
  }
}
// Funckja obliczająca sumę kontrolną kodu ean na podstawie wzoru.
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
//Główna funkcja rysująca bitmapę. Tworzy obiekty klas Characters (cyfry 8x8px)
// oraz obiekt Bitmap. Rozdziela cyfry kodu na dwie grupy i wywłuje funkcje rysujące
void EAN::generateBitmap()
{
  Bitmap *bitmap = new Bitmap();
  Characters *chars = new Characters();
  string firstGroupEncoding;
  string firstGroupDigits;
  string secondGroupDigits;
  int digit;
  digit = eanCode[0] - '0';
  firstGroupEncoding = encode(digit);
  firstGroupDigits = eanCode.substr(1, 6);
  secondGroupDigits = eanCode.substr(7, 12);

  //Funkcja rysująca znacznik startowy (moduł 101)
  drawCode("101");
  // Rysowanie pierwszej grupy cyfr
  drawFirstGroupBars(firstGroupDigits, firstGroupEncoding);
  //Funkcja rysująca znacznik środka (moduł 101)
  drawCode("01010");
  // Rysowanie drugiej grupy cyfr
  drawSecondGroupBars(secondGroupDigits);
  //Funkcja rysująca znacznik końcowy (moduł 101)
  drawCode("101");

  //Rysowanie cyfr pod kodem kreskowym
  drawEanCharacters();

  // Wywolanie funkcji generujacej bitmape.
  bitmap->generateBitmapImage(image, height, width, imageFileName);
}
// Funkcja generuje plik pdf z kodem kreskowym.
// Wykorzystana została mała biblioteka obsługująca m.in rysowanie kształtów,
// wpisywanie tekstu, generowanie bitmapy.
void EAN::generatePdf()
{
  //Tworzenie nazwy pliku
  ostringstream out;
  out << "ean.pdf";
  string fileName = out.str();

  //Tworzenie obietku PDF
  PDF p;

  //Tworzenie obietku Image.
  Image anImage;

  //Obiekt Image jest vectorem obiektów ImageRow, który z kolei
  // jest vectorem obiektów RGB, który przetrzymuje zmienne składowe
  // R, G, B w postaci unsigned char. Pozwala to na bezpośrednie wpisanie
  // przetrzymywanych w tablicy image bajtów R,G,B
  for (int j = height - 1; j >= 0; j--)
  {
    ImageRow row;
    for (int i = 0; i < width; i++)
    {
      row.push_back(RGB(image[j][i][0], image[j][i][1], image[j][i][2]));
    }
    anImage.push_back(row);
  }
  // Tworzenie obiektu Image info który generuje odpowiednie nagłówki formatu PDF
  ImageInfo info = p.processImage(anImage);

  int width = 500;
  int height = 500;

  int xValue = 100;
  int yValue = p.getHeight() - 300;

  //Dodawanie obrazu do obiektu PDF
  p.showImage(info, xValue, yValue, 1.0);
  p.newPage();

  string errMsg;

  //Zapisywanie obiektu PDF do pliku
  if (!p.writeToFile(fileName, errMsg))
  {
    cout << errMsg << endl;
  }
  else
  {
    cout << "(File Successfully Written)" << endl;
  }

  cout << endl;
}