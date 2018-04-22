/*
 * Joseph Baccary, CS3377, jcb150630@utd.edu
 * CDK display code is from Dr Perkins' example
 */

#include <iostream>
#include "cdk.h"
#include <stdint.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <sstream> 

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;


/*Declare the classes that we'll be working with. Yes, this would usually bein 
the header file. No, I can't be bothered to add another when it isn't necessary.*/
class BinaryFileHeader
{
public:
  uint32_t magicNumber; /* Should be 0xFEEDFACE in the example*/
  uint32_t versionNumber;
  uint64_t numRecords;
};
/*
 * Records in the file have a fixed length buffer
 * that will hold a C-Style string. This is the
 * size of the fixed length buffer.
 */
const int maxRecordStringLength = 25;
class BinaryFileRecord
{
public:
  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};



/* Convert the ints to hex strings curtesy of stackoverflow:
   https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c
   Plus a tweak for uppercase*/
template< typename T >
std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << "0x" <<std::uppercase 
         << std::setfill ('0') << std::setw(sizeof(T)*2) 
         << std::hex << i;
  return stream.str();
}


int main()
{

  //Initialize file. 
  ifstream binFile ("cs3377.bin", ios::in | ios::binary);
  //binFile.open ("cs3377.bin");
  if(!binFile.is_open()){
    cout<<"Error: cannot open cs3377.bin"<<endl;
    return 1;
  }


  BinaryFileHeader *header = new BinaryFileHeader();
  BinaryFileRecord *record1 = new BinaryFileRecord();
  BinaryFileRecord *record2 = new BinaryFileRecord();
  BinaryFileRecord *record3 = new BinaryFileRecord();
  BinaryFileRecord *record4 = new BinaryFileRecord();

  /*Note, non-portable code here:
    Being careful not to read in records that aren't there. Of course, 
    this assumes that there's a header and that numRecords is correct.
  */
  binFile.read((char *) header, sizeof(BinaryFileHeader));
  if(header->numRecords>0)
    binFile.read((char *) record1, sizeof(BinaryFileRecord));
  if(header->numRecords>1)
    binFile.read((char *) record2, sizeof(BinaryFileRecord));
  if(header->numRecords>2)
    binFile.read((char *) record3, sizeof(BinaryFileRecord));
  if(header->numRecords>3)
    binFile.read((char *) record4, sizeof(BinaryFileRecord));


 

 WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "R1", "R2", "R3", "R4", "R5"};
  const char 		*columnTitles[] = {"C0", "C1", "C2", "C3", "C4", "C5"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);


  /*
   * Display things
   */

  //Set first row:
  setCDKMatrixCell(myMatrix, 1, 1, string("Magic: " + int_to_hex(header->magicNumber)).c_str());
  setCDKMatrixCell(myMatrix, 1, 2, string("Version: " + to_string(header->versionNumber)).c_str());
  setCDKMatrixCell(myMatrix, 1, 3, string("NumRecords: " + to_string(header->numRecords)).c_str());
  //Set second row:
  //For each of these, we use numrecords to insure that something valid is realy there.
  if(header->numRecords>0){
    setCDKMatrixCell(myMatrix, 2, 1, string("strlen: " + to_string(record1->strLength)).c_str());
    setCDKMatrixCell(myMatrix, 2, 2, string(record1->stringBuffer).c_str());
  }
  //set third row
  if(header->numRecords>1){
    setCDKMatrixCell(myMatrix, 3, 1, string("strlen: " + to_string(record2->strLength)).c_str());
    setCDKMatrixCell(myMatrix, 3, 2, string(record2->stringBuffer).c_str());
  }
  //set fourth row
  if(header->numRecords>2){
    setCDKMatrixCell(myMatrix, 4, 1, string("strlen: " + to_string(record3->strLength)).c_str());
    setCDKMatrixCell(myMatrix, 4, 2, string(record3->stringBuffer).c_str());
  }
  //set last row
  if(header->numRecords>3){
    setCDKMatrixCell(myMatrix, 5, 1, string("strlen: " + to_string(record4->strLength)).c_str());
    setCDKMatrixCell(myMatrix, 5, 2, string(record4->stringBuffer).c_str());
  }


  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
  //Cleanup memory
  delete record1;
  delete record2;
  delete record3;
  delete record4;
  delete header;

  return 0;
}
