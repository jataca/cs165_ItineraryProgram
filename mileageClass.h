#ifndef MILEAGE_H
#define MILEAGE_H
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <map>

using namespace std;


// Global constant
const char PLANE = 0x10;
const char FOOT = 0x01;
const char BICYCLE = 0x02;
const char CAR = 0x04;
const char BUS = 0x08;
const char ALL_MODES = 0x1f;

/****************************************************************************
* Combines the start city and destination cities together to make a "KEY" for
* the STL map.
****************************************************************************/
struct MileageKey
{
   string startCity;
   string destCity;
};

/***************************************************************************
* A struct used for the "data" stored in the STL map.
* int  miles - mileage between cities
* char modes - transportation mode bit mask
* Each bit in the byte represents a transportation mode.
* If the corresponding bit is set, the transportation mode is available.
* bit 0 = foot, bit 1 = bicycle, bit 2 = car, bit 3 = bus, bit 4 = plane
* Example:  0x1F - all modes available; 0x10 = plane only; 0x01 = foot only
***************************************************************************/
struct MileageData
{
   int miles;   // miles between the two cites
   char modes;  // byte indicating available transportation modes
};

/*****************************************************************************
* Class for creating a Mileage Map used to determine the mileage between
* different cities thoughout the U.S. and some major cities in foreign
* countries. Once the map object is created you call the "getMileage"
* function passing it the start city/state and the destination city/state
* and it will return the mileage between the two cities and the available
* transportation modes.
******************************************************************************/
class MileageMap
{
   public:

      MileageMap();     // default constructor
      MileageMap(string usFileName, string overSeasFileName);
      bool getMileage(string startCity, string destCity, MileageData &milesModes);
      bool addMileage(string startCity, string destCity, MileageData milesModes);

   private:
      string fileName;
      map < MileageKey, MileageData, less < MileageKey > > mileageChart;
      map < MileageKey, MileageData, less < MileageKey > > readFiles
	(string usFile, string overSeasFile);
      string fixCityStateFormat(string cityState);
      void addToMileageFile(MileageKey key, MileageData milesModes);
      void readDataFile(map < MileageKey, MileageData, less < MileageKey > > &map);
      bool operator < (const MileageKey &rightSide);
};

/*****************************************************************************
* Default constructor - Reads the two input files & builds the STL map object
*****************************************************************************/
MileageMap::MileageMap()
{
   fileName = "otherCities.txt";
   mileageChart = readFiles("mileage.txt",
			    "overSeas.txt");
}

/****************************************************************************
* Constructor -  Reads the two input files and builds the STL map object
****************************************************************************/
MileageMap::MileageMap(string usFileName, string overSeasFileName)
{
  fileName = "otherCities.txt";
  mileageChart = readFiles(usFileName, overSeasFileName);
}


/**************************************************************************
* Name: getMileage
* Desc: Given the start city/state and the destination city/state
*       it returns the miles between the two cities and the
*       transportation modes available between the two cities.
* Inputs: start city, destination city
* Outputs: struct - MileageData -  miles(int), transportation mode (char)
* returns: bool - mileage found (false = not found, true = found
**************************************************************************/
bool MileageMap::getMileage(string startCity,
                            string destCity,
                            MileageData &milesModes)
{
   MileageKey key;
   key.startCity = fixCityStateFormat(startCity);
   key.destCity = fixCityStateFormat(destCity);
   if (mileageChart.find(key) == mileageChart.end())
   {
      return false;              // mileage between cities not found
   }
   else
   {
      milesModes = mileageChart[key];  // mileage found
      return true;
   }

}

/*****************************************************************************
* Name: addMileage
* Desc: Given the start city/state, the destination city/state, the
*       miles between the cities, and the available transportation mode
*       and adds the data to the map.
* Inputs: start city, destination city
*         struct - MileageData  (miles (int), tranportation mode (char))
* Outputs: none
* returns: true - mileage and transportation modes between cities added.
*          false - mileage between these cities already in table - not added.
*****************************************************************************/
bool MileageMap::addMileage(string startCity,
                            string destCity,
                            MileageData milesModes)
{
   MileageKey key;
   MileageKey key2;
   key.startCity = fixCityStateFormat(startCity);
   key.destCity = fixCityStateFormat(destCity);
   key2.startCity = key.destCity;   // make a key for other travel direction
   key2.destCity = key.startCity;
   if (mileageChart.find(key) == mileageChart.end())
   {
      mileageChart[key] = milesModes;
      mileageChart[key2] = milesModes;
      addToMileageFile(key, milesModes);     // add to the extension data file
      return true;     // successfull add
   }
   else
   {
      return false;    // did not add, data already in mileage map
   }
}

/**********************************************************************************
* Name: addToMileageFile
* Desc: This function adds a new start & destination city (with the mileage
*       and available transportations) to the data file.
*       File Format:    startcity,
**********************************************************************************/
void MileageMap::addToMileageFile(MileageKey key, MileageData milesModes)
{
   ofstream outStream;
   outStream.open(fileName.c_str(),ios::app);
   if (outStream.good())
   {
      outStream << key.startCity << ":" << key.destCity
                << ":" << milesModes.miles << ":" << (int)milesModes.modes << endl;
      outStream.close();
   }
   else
   {
      cout << "Couldn't open Mileage data file - " << fileName << endl;
      cout << "Mileage between " << key.startCity << " and " << key.destCity
           << " not added to " << fileName << "!" << endl;
   }
}

/*********************************************************************************
* Name: readDataFile
* Desc: Reads the data file created with the "addMileage" function,
*       and adds the cities in that file to the mileage map.
* Inputs:  MileageMap -  map with cities and mileages between them.
*          fileName - name of the data file generated the "addMileage" function.
* Outputs: MileageMap - updated with entries from the file
**********************************************************************************/
void MileageMap::readDataFile(map < MileageKey, MileageData,
                                  less < MileageKey > > &mileageMap)
{
   ifstream instream;
   MileageKey key;
   MileageKey key2;
   MileageData milesModes;
   int miles;
   int modes;
   string sModes;
   string sMiles;

   instream.open(fileName.c_str());
   if (instream.good())
   {
      getline(instream,key.startCity,':');
      while (!instream.eof())
      {
         getline(instream,key.destCity,':');
         getline(instream,sMiles,':');
         milesModes.miles = atoi(sMiles.c_str());
         getline(instream, sModes);
         modes = atoi(sModes.c_str());
         milesModes.modes = (char)modes;
         mileageMap[key] = milesModes;   // add entry to mileage map
         key2.startCity = key.destCity;  // reverse the start and dest cities
         key2.destCity = key.startCity;
         mileageMap[key2] = milesModes;  // add reversed enty to mileage map

         getline(instream,key.startCity,':');  // read next start city
      }
      instream.close();
   }
}


/**************************************************************************
* Name: readFiles
* Desc: Reads two mileage chart data files and builds a STL map.
*       The map's key is made up of the "start city" + "destination city"
*       The map's data is a struct containing an int - miles between cities,
*       and a char - bit map indicating the available transportation modes.
* Inputs:  usFile -  filename for text file with the mileages between cities
*          overSeasFile - filename for text file with mileages between U.S
*                         cities and overseas cities
*          dataFileName - additional cities (file generated by this program)
* Outputs: none
* Returns: Map object - contains a map of cities and mileages between cities
**************************************************************************/
map < MileageKey, MileageData, less < MileageKey > > MileageMap::readFiles
    (string usFile, string overSeasFile)
{
   const int NO_DATA_FILES = 2;
   int rowCount;
   int colCount;
   MileageKey key;
   MileageData data;

   map < MileageKey, MileageData, less < MileageKey > > mileageMap;

   // Input file stream
   ifstream inStream;
   string fileNames[NO_DATA_FILES];
   fileNames[0] = usFile;
   fileNames[1] = overSeasFile;

   for (int fileNo = 0; fileNo < NO_DATA_FILES; fileNo++)
   {
      // Check for an error on the open
      inStream.open(fileNames[fileNo].c_str());
      while (inStream.fail())
      {
         cout << "File not found: " << fileNames[fileNo] << endl << endl;
         cout << "Enter name of mileage data file #" << fileNo << ": ";
         cin >> fileNames[fileNo];
         cin.ignore(80,'\n');      // throw out newline
         inStream.open(fileNames[fileNo].c_str());
      }

      inStream >> rowCount;
      inStream >> colCount;
      inStream.ignore(80,'\n');    // throw out newline

      string* rowNames = new string [rowCount];
      string* colNames = new string [colCount];

      // dynamically allocate a two dimensional array (int mileage[rowCount][colCount])
      int** mileage = new int*[rowCount];
      for (int i = 0; i < rowCount; i++)
      {
         mileage[i] = new int[colCount];
      }


      // read in the row city name and then the mileages for the row
      for (int row = 0; row < rowCount; row++)
      {
         getline(inStream, rowNames[row]);

         for (int col = 0; col < colCount; col++)
         {
            inStream >> mileage[row][col];
         }
         inStream.ignore(80,'\n');    // throw out newline
      }

      // read in the column city names
      for (int col = 0; col < colCount; col++)
      {
         getline(inStream, colNames[col]);
      }

      // Build mileage map
      for (int row = 0; row < rowCount; row++)
      {
         for (int col = 0; col < colCount; col++)
         {
            key.startCity = rowNames[row];
            key.destCity = colNames[col];
            data.miles = mileage[row][col];
            if (data.miles < 200)
            {
               data.modes = FOOT | CAR | BUS | BICYCLE;
            }
            else if (data.miles > 2000 && fileNo != 1)
            {
               data.modes = CAR | BUS | BICYCLE | PLANE;
            }
            else if (fileNo == 1) //this file is international air travel only
            {
               data.modes = PLANE;
            }
            else
            {
               data.modes = ALL_MODES;
            }

            mileageMap[key] = data;
            // cerr << key.startCity + key.destCity << endl;
            // cerr << mileageMap[key].miles << endl;
            //Build new key for traveling other direction
            string temp = key.startCity;
            key.startCity = key.destCity;
            key.destCity = temp;
            mileageMap[key] = data;
            // cerr << key.startCity + key.destCity << endl;
            // cerr << mileageMap[key].miles << endl;
         }
      }
      // Close the file
      inStream.close();
      delete [] rowNames;  // free memory
      delete [] colNames;  // free memory
      // free dynamically allocated two dimensional array
      for (int i = 0; i < rowCount; i++)
      {
         delete [] mileage[i];
      }
      delete [] mileage;
      readDataFile(mileageMap);

   }  // end for
   return mileageMap;
}


/*************************************************************************
* Name: fixCityStateFormat
* Desc: fixes the city and state string so it is the proper format for
*       the KEY into the mileage chart MAP.
*       It uppercases the first letter of the city name.
*       It removes any spaces after the comma.
*       It uppercases the state name.
* Inputs: string - city and state (e.g. Boise, Id)
* Output: none
* Returns: string - city and state corrected format (e.g. Boise,ID)
**************************************************************************/
string MileageMap::fixCityStateFormat(string cityState)
{
   int commaPos;
   int lastSpacePos;

   cityState[0] = toupper(cityState[0]); // upper case 1st letter of city name

   commaPos = cityState.find(",",0);     // search for comma's position
   if (commaPos == string::npos)         // if comma is not found, insert one
   {
      lastSpacePos = cityState.find_last_of(" ",cityState.length());
      if (lastSpacePos != string::npos)
      {
         cityState.insert(lastSpacePos,",");
         commaPos = lastSpacePos;
      }
   }

   for (int i = commaPos; i < cityState.length(); i++)
   {
      if (cityState[i] == ' ')           // remove any spaces after the comma
      {
         cityState.erase(i,1);
         i--;
      }
      else
      {
         cityState[i] = toupper(cityState[i]);  // upper case the State Name
      }
   }
   return cityState;
}


/**************************************************************************
* Overloaded less-than operator for the mapKey object.
* This combines the startCity name and destination city name
* together for make the less-than comparison STL maps key.
**************************************************************************/
bool operator < (const MileageKey &leftSide, const MileageKey &rightSide)
{
   if (leftSide.startCity + leftSide.destCity <
       rightSide.startCity + rightSide.destCity)
   {
      return true;
   }
   else
   {
      return false;
   }
}

#endif
