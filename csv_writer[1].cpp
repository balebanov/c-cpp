#include <iostream>
#include <fstream>
using namespace std;
int main( int argc, char* argv[] )
{
      ofstream myfile;
      myfile.open ("example.csv");
      myfile << "This is the first cell in the first column.\n";
      myfile << "a;b;c;\n";
      myfile << "c;s;v;\n";
      myfile << "1,2,3;456\n";
      myfile << "semicolon;test";
      myfile.close();
      return 0;
}
