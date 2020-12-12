#include <iostream>
#include "EAN.h"

using namespace std;

int main()
{

  EAN *ean = new EAN();
  ean->generateBitmap();
  ean->generatePdf();
  cout << "bitmap and pdf created";
}
