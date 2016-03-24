/***********************************************************************
* Program:
*    Assignment 14, itinerary program
*    Brother Ercanbrack, CS165
* Author:
*    Jacob Casperson
* Summary: 
*    This program allows a user to create itineraries. These trips the
*    user makes can then be saved and opened to and from files. Each
*    trip will include a start city, destination city, start time,
*    transportation mode, and will calculate the travel time, and the
*    destination date and time. 
*
*    Estimated:  12.0 hrs   
*    Actual:     16.0 hrs
*      Error handling is beastly
************************************************************************/
#include "mileageClass.h"
#include <iostream>
using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

#ifndef TIME_H
#define TIME_H

/*****************************************************************************
 * Class Name: Time
 * Summary: This class contains methods for the the creation,
 *          manipulation, input, and output of time objects
 *****************************************************************************/
class Time
{
   public:

      Time(int theHour, int theMin, int theSec);
      Time(int theHour, int theMin);
      Time();
      void setMinute(int theMinute);
      void setHour(int theHour);
      void setSecond(int theSecond);
      void addSeconds(int seconds2Add);
      void addMinutes(int minutes2Add);
      // add hours and returns # of days of overflow
      int  addHours(int hours2Add);
      // display as military time - default no seconds (hh:mm)
      void displayMilitaryTime(bool displaySeconds = false);
      // display the prompt and read data into the time object
      void promptForTime(string prompt, bool readSeconds = false);
      // add two time objects & return the resulting time object
      Time operator + (const Time time1);                           

      // logical operators
      bool operator < (const Time &time1);
      bool operator > (const Time &time1);
      bool operator == (const Time &time1);
      bool operator <= (const Time &time1);
      bool operator >= (const Time &time1);

      // displays the time object in the format "hh:mm"
      friend ostream& operator << (ostream &out, const Time &time1);
      // reads into the time object.  Expects format "hh:mm"
      friend istream& operator >> (istream &in, Time &time1);

protected:
      int hour;
      int minute;
      int second;
};
#endif

class Date : public Time
{
public:
   Date();
   Date(int cMonth, int cDay, int cYear, int cHour, int cMinute);
   Date(int& cYear, int& cMonth, int& cDay);
   // setters!
   void setDay(int setDay);
   void setMonth(int setMonth);
   void setYear(int setYear);
   // adders! 
   void addTime(int& addDay, int& addHour, int& addMinute);
   void addMinutes(int minutes2Add);
   void addHours(int hours2Add);
   void addDays(int daysToAdd);
   void addMonths(int month2Add);
   void addYears(int years2Add);
   //prompt for information
   int promptForDate(string prompt);
   int promptForTime(string prompt);
   // display 
   void displayDateAndTime();
   void displayLDSDateAndTime();
   // calculate how many days are in the month
   int daysInMonth();
   friend ostream& operator << (ostream &out, const Date &date1);
   
private:
   int day;
   int month;
   int year;
  
};
   
/***********************************************************************
* Default Constructor. Sets private variables all to 0.
***********************************************************************/
Time::Time()
{
   hour = 0;
   minute = 0;
   second = 0;
}

/***********************************************************************
* Second Constructor. Passes in hour and minute as parameters with
* preset values and sets second to 0. 
***********************************************************************/
Time::Time(int cHour, int cMinute)
      :hour(cHour), minute(cMinute)
{
   second = 0;
}

/***********************************************************************
* Third constructor. Passes in all variables as parameters with preset
* values. 
***********************************************************************/
Time::Time(int cHour, int cMinute, int cSecond)
      :hour(cHour), minute(cMinute), second(cSecond)
{
   
}

/************************************************************************
 *  int getInt(string prompt, string errMsg)
 *
 *  Desc: Error safe - read integer routine
 *        On error, this function will re-prompt for the integer
 *        until there is no input error.
 *
 *  Inputs:  prompt - string used to prompt for the integer
 *           errMsg - error message you wish displayed if you get an error.
 *  Outputs: none
 *  return:  integer - read from the console
 **************************************************************************/
int getInt(string prompt, string errMsg = "\nValue must be an integer!\n")
{
   int input;
   cout << prompt;
   
   while (! (cin >> input))
   {
      cout << errMsg;
      cin.clear();
      cin.ignore(80, '\n');
   }
      
   return input;
}

/***********************************************************************
* Set the minute to the value passed in as a parameter
***********************************************************************/
void Time::setMinute(int theMinute)
{
   minute = theMinute;
}

/***********************************************************************
* Set the hour to the value passed in as a parameter
***********************************************************************/
void Time::setHour(int theHour)
{
   hour = theHour;
}

/***********************************************************************
* Set the second to the value passed in as a parameter
***********************************************************************/
void Time::setSecond(int theSecond)
{
   second = theSecond;
}

/***********************************************************************
* Add to the second the amount of seconds passed in. Don't go over 60,
* for every 60 add an minute.
***********************************************************************/
void Time::addSeconds(int seconds2Add)
{
   // add seconds
   second += seconds2Add;

   // everytime seconds reach 60 add an hour and set seconds to 0
   while (second >= 60)
   {
      second -= 60;
      addMinutes(1);
   }
}

/***********************************************************************
* Add to the minute the amount of minutes passed in. Don't go over 60,
* for every 60 add an hour.
***********************************************************************/
void Time::addMinutes(int minutes2Add)
{
   // add minutes
   minute += minutes2Add;

   // everytime minutes reach 60 add an hour and set minutes to 0
   while (minute >= 60)
   {
      minute -= 60;
      addHours(1);
   }
}


/***********************************************************************
* Add to the hours the amount of minutes passed in. Don't go over 24,
* for every 24 add a day.
***********************************************************************/
int Time::addHours(int hours2Add)
{
   int days = 0;

   // add hours
   hour += hours2Add;

   // everytime hours reach 24 add a day and set hours to 0
   while (hour >= 24)
   {
      hour -= 24;
      days++;
      
   }
   
   return days;
}

/***********************************************************************
* Display function. Displays in form hh:mm or hh:mm:ss
***********************************************************************/
void Time::displayMilitaryTime(bool displaySeconds)
{
   // display hours
   if (hour < 10)
      cout << "0" << hour;
   else
      cout << hour;
   
   cout << ":";

   // display minutes
   if (minute < 10)
      cout << "0" << minute;
   else
      cout << minute;

   // if we want we can display seconds
   if (displaySeconds == true)
   {
      cout << ":";
      if (second < 10)
         cout << "0" << second;
      else
         cout << second;
   }  
}

/***********************************************************************
* Prompts the user to input time from the keyboarrd
***********************************************************************/
void Time::promptForTime(string prompt, bool readSeconds)
{
   cout << prompt;
   cin >> hour;
   cin.ignore();
   cin >> minute;

     if (readSeconds == true)
     {
        cin.ignore();
        cin >> second;
     }
     cin.ignore();
}

/***********************************************************************
* Add two times together
***********************************************************************/
Time Time::operator + (const Time time1) 
{
   return Time(hour + time1.hour, minute + time1.minute, second + time1.second);
}

/**********************************************************************
 * Determines if it is a leap year or not
 ***********************************************************************/
bool isLeapYear(int year)
{
   // return a leap year if year / 4 has no remainder and year / 100
   // does have a remainder. Or return leap year if year / 400 = 0
   if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
      return true;

   else
      return false; 
}

/**********************************************************************
 * Default constructor, sets everything to 0. Calls the default time
 * constructor to set those to 0, no need to explicitly call it.
 **********************************************************************/
Date::Date()
{
   day = 0;
   month = 0;
   year = 0;
}
/**********************************************************************
 * constructor, sets month, day and year to whatever is passed to it,
 * calls default time constructor on its own to set time variables to 0 
 **********************************************************************/
Date::Date(int& cMonth, int& cDay, int& cYear)
      :month(cMonth), day(cDay), year(cYear)
{
}

/**********************************************************************
 * constructor, sets month, day, and year to whatever is passed to it,
 * also calls a constructor explicitly to set hour and minute to what
 * was passed in as well
 **********************************************************************/
Date::Date(int cMonth, int cDay, int cYear, int cHour, int cMinute)
     :Time(cHour, cMinute)
{
   year = cYear;
   month = cMonth;
   day = cDay; 
}

/**********************************************************************
 * sets day to whatever is passed in
 **********************************************************************/
void Date::setDay(int setDay)
{
   day = setDay;
}

/**********************************************************************
 * sets month to whatever is passed in
 **********************************************************************/
void Date::setMonth(int setMonth)
{
   month = setMonth;
}

/**********************************************************************
 * sets year to whatever is passed in
 **********************************************************************/
void Date::setYear(int setYear)
{
   year = setYear;
}

/***********************************************************************
* Add to the hours the amount of minutes passed in. Don't go over 24,
* for every 24 add a day.
***********************************************************************/
void Date::addMinutes(int minutes2Add)
{
   minute += minutes2Add;

   while (minute >= 60)
   {
      minute -= 60;
      addHours(1);
   }
}

/***********************************************************************
* Add to the hours the amount of minutes passed in. Don't go over 24,
* for every 24 add a day.
***********************************************************************/
void Date::addHours(int hours2Add)
{
   // add hours
   hour += hours2Add;

   // everytime hours reach 24 add a day and set hours to 0
   while (hour >= 24)
   {
      hour -= 24;
      addDays(1);
      
   }
}

/**********************************************************************
 * add days to your date
 **********************************************************************/
void Date::addDays(int daysToAdd)
{
   day += daysToAdd;
   
   bool checkMonth = true;

   // as long as we're checking the month see if we need to add a month
   while (checkMonth == true)
   {
      int daysNMonth = daysInMonth();
      
      if (day > daysNMonth)
      {
         day -= daysNMonth;
         checkMonth = true;
         addMonths(1);
      }
      else
      {
         checkMonth = false;
      }
   }
   
}

/**********************************************************************
 * add months to the date
 *********************************************************************/
void Date::addMonths(int months2Add)
{
   month += months2Add;

   while (month > 12)
   {
      month -= 12;
      year++;
   }
}

/**********************************************************************
 *
 *********************************************************************/
void Date::addYears(int years2Add)
{
   year += years2Add;

}

/**********************************************************************
 * prompt the user to enter a date: month/day/year. for some reason
 * my cin.clear()s are not flushing the buffer properly, the last
 * digits on the year are remaining and will stick onto the month or
 * day. 
 **********************************************************************/
int Date::promptForDate(string prompt)
{
   bool accepted;
   string input;
   char cstring[4];
    do
    {
       cout << prompt;
       getline(cin, input);
       
       // cin.ignore(256, '\n');
       int i = 0;
       
       // set the month
       while(input[i] != '/' & i < input.length())
       {
          cstring[i] = input[i];
          i++;
       }
       month = atoi(cstring);
       
       // erase the month input
       input.erase(0, i + 1);
       
       //set the day
       i = 0;
       
       cstring[1] = NULL;
       
       if (input.length() > 5)
       {
          while(input[i] != '/' & i < input.length())
          {
             cstring[i] = input[i];
             i++;
          }
          
          day = atoi(cstring);
         
          // erase the day input
          input.erase(0, i + 1);
       }

       if (input.length() > 3)
       {
          // set the year
          i = 0;
          for (int j = 0; i < input.length(); j++)
          {
             cstring[i] = input[i];
             i++;
          }
          
          year = atoi(cstring);
       }
       
       // error handling month
       if (month < 1 || month > 12)
       {
          cout << "\nError " << month << " is invalid. "
             // << "Please enter a month number from 1 to 12\n";
               << "You will have to restart the program.\n";
             accepted = false;
          input.clear();
          cin.clear();
          //cin.ignore(256, '\n');
          cout << input;
       }  
       else
          accepted = true;

       // error handling day
       if (day < 1 || day > daysInMonth())
       {
          cout << "\nError " << day << " is invalid. "
             // << "Please enter a valid day for the given month\n";
               << "you will have to restart the program\n";
          accepted = false;
          cin.clear();
       }
       else
          accepted = true;

       // error handling year
       if (year < 0)
       {
          cout << "\nError " << year << " is invalid. "
               << "Please enter a valid year\n";
          accepted = false;
          cin.clear();
          cin.ignore(256, '\n');
       }
       else
          accepted = true;
       cin.clear();
    }
    while (accepted == false);

   return 0;

}

/**********************************************************************
 * prompt for time. Will work as long as letters are not entered in
 * the place of hours and minutes.
 **********************************************************************/
int Date::promptForTime(string prompt)
{
   char colon;
   string errorHandle;
   bool fail = false;
   do
   {
      cout << prompt;
      cin >> hour;
      if (cin.fail())
      {
         getline(cin, errorHandle);
         cin.clear();
         cin.ignore(256, '\n');
         fail = true;
      }
      cin >> colon;
      cin >> minute;
      if (cin.fail())
      {
         getline(cin, errorHandle);
         cin.clear();
         cin.ignore(256, '\n');
         fail = false;
      }
      
      if (hour < 0 || hour > 24 || minute < 0 || minute > 60 || colon != ':'|| fail == true)
      {
         cout << "Error - That is an invalid time format.\n"
              << setw(5) << "Must be 24 hour military time (hh:mm)\n";
      }
   }
   while (hour < 0 || hour > 24 || minute < 0 || minute > 60 || colon != ':');
   
}

/**********************************************************************
 * returns how many days are in the month            
 *********************************************************************/
int Date::daysInMonth()
{
   switch (month)
   {
      case 0:
         return 0;
         break;
      case 1:
         return 31;
         break;
      case 2:
         if (isLeapYear(year) == false)
            return 28;
         else
            return 29;
         break;
      case 3:
         return 31;
         break;
      case 4:
         return 30;
         break;
      case 5:
         return 31;
         break;
      case 6:
         return 30;
         break;
      case 7:
         return 31;
         break;
      case 8:
         return 31;
         break;
      case 9:
         return 30;
         break;
      case 10:
         return 31;
         break;
      case 11:
         return 30;
         break;
      case 12:
         return 31;
         break;
   }
}
/**********************************************************************
 * display the date in time in regular USA format
 **********************************************************************/
void Date::displayDateAndTime()
{
   //display months
   if (month < 10)
      cout << "0" << month;
   else cout << month;

   cout << "/";

     //display days
   if (day < 10)
      cout << "0" << day;
   else
      cout << day;

   cout << "/";

   //display years
   if (year == 0)
      cout << "000" << year;
   else 
      cout << year;

   cout << " ";
   
   // display hours
   if (hour < 10)
      cout << "0" << hour;
   else
      cout << hour;
   
   cout << ":";

   // display minutes
   if (minute < 10)
      cout << "0" << minute;
   else
      cout << minute;
}

/**********************************************************************
 * display the date in LDS format: dd Mon yyyy hh:mm
 **********************************************************************/
void Date::displayLDSDateAndTime()
{
   //display days
   if (day < 10)
      cout << "0" << day;
   else
      cout << day;

   cout << " ";

   //display months

   switch (month)
   {
      case 1:
         cout << "Jan";
         break;
      case 2:
         cout << "Feb";
         break;
      case 3:
         cout << "Mar";
         break;
      case 4:
         cout << "Apr";
         break;
      case 5:
         cout << "May";
         break;
      case 6:
         cout << "Jun";
         break;
      case 7:
         cout << "Jul";
         break;
      case 8:
         cout << "Aug";
         break;
      case 9:
         cout << "Sep";
         break;
      case 10:
         cout << "Oct";
         break;
      case 11:
         cout << "Nov";
         break;
      case 12:
         cout << "Dec";
         break;
   }
   cout << " ";

   //display years
   if (year == 0)
      cout << "000" << year;
   else 
      cout << year;

   cout << " ";
   
   // display hours
   if (hour < 10)
      cout << "0" << hour;
   else
      cout << hour;
   
   cout << ":";

   // display minutes
   if (minute < 10)
      cout << "0" << minute;
   else
      cout << minute;
      
}

/**********************************************************************
 * add time, days, hours, minutes all in one function call
 **********************************************************************/
void Date::addTime(int& addDay, int& addHour, int& addMinute)
{
   addDays(addDay);  
   hour += addHour;
   minute += addMinute;
}

/***********************************************************************
 * Used so we can cout a Date object
***********************************************************************/
ostream& operator << (ostream &out, const Date &date1)
{  
   if (date1.month < 10)
      out << "0" << date1.month;
   else
      out << date1.month;
   out << "/";
   if (date1.day < 10)
      out << "0" << date1.day;
   else
      out << date1.day;

   out << "/" << date1.year << " ";
   
   if (date1.hour < 10)
      out << "0" << date1.hour;
   else
      out << date1.hour;

   out << ":";
   
   if (date1.minute < 10)
      out << "0" << date1.minute;
   else
      out << date1.minute;

   return out;
}

/****************************************************************************
* Template class for building a node object.  The data for the node can be
* any datatype.  
****************************************************************************/
template <class T>
class Node
{
   public:
      Node(const T &theData, Node<T>* theLink)
      : data(theData), link(theLink)
      {
         // left blank intentionally
      }
      Node<T>* getLink( )    { return link; }
      T& getData( )          { return data; }
      void setData(const T &theData) { data = theData; }
      void setLink(Node<T>* pointer) { link = pointer; }

   private:
      T data;            // The data object
      Node<T>* link;     // pointer to next node (i.e. link)
};

/****************************************************************************
* Desc: headInsert - creates a new node and inserts it at the beginning of
*       the linked list.
*
* Input: head - points to the first node in the linked list.
*        theData -  data object to be stored in the new node that is
*                   inserted at the beginning of the linked list.
* Output: head - points to the new first node in the linked list.
****************************************************************************/
template <class T>
void headInsert(Node<T>* &head, const T &theData)
{
   head = new Node<T>(theData, head);
}


/******************************************************************************
* Desc: insert - Creates new node (initializing it with "theData") and inserts
*       it in the linked list after the node pointed to by "afterMe".
*
* Input:  afterMe - pointer to node in the linked list to insert after.
*         theData - Data to be stored in the new node being inserted
* Output: afterMe - link is modified to point at inserted node
*         Note: if afterMe was Null when passed in, a head insert is done
*               and afterMe will now point at the inserted node, which is the
*               first node in the linked list.  
******************************************************************************/
template <class T>
void insert(Node<T>* &afterMe, const T &theData)
{
   if (afterMe != NULL)
   {
      afterMe->setLink(new Node<T>(theData, afterMe->getLink( )));
   }
   else
   {
      headInsert(afterMe,theData);
   }
}

/******************************************************************************
* Class to be used for the data object that is stored in your node class above
* You must implement each of the public functions.
******************************************************************************/
class Leg
{
   private:
      string startCity;
      string destCity;
      string transMode; // transportation mode between cities (car, plane, etc)
      int milesBetween; // mileage between start and destination cities
      Date startDateTime;
      Date arrivalDateTime;
   public:
      Leg();
      Leg(string sCity, string dCity, string transMode, int miles,
          Date startDate, Date arrivalDate);
      void display();
      void display2File(ofstream &outputFile);
     
      bool operator == (Leg rightSide);
      bool operator != (Leg rightSide);
};


/**********************************************************************
* Default Constructor - calls constructors for two Date class member 
* variables and initializes the other member variables.
*********************************************************************/
Leg::Leg()
  : startDateTime(), arrivalDateTime()
{
   startCity = "";
   destCity = "";   
   transMode = "Car";
   milesBetween = 0;
}

/*****************************************************************************
* Constructor -
* Parameters are passed to this constructor to initialize all the
* member variables for the Leg object.
* Inputs:  sCity - start city
*          dCity - destination city
*   theTransMode - transportation mode ("car", "bus", "bike", "foot", "Plane")
*          miles - mileage between start city and destination city
*      startDate - "Date" object that specifies when leaving the start city.
*    arrivalDate - "Date" object that specifies expected arrival in dest city 
******************************************************************************/
Leg::Leg(string sCity, string dCity, string theTransMode, int miles,
    Date startDate, Date arrivalDate)
{
   startCity = sCity;
   destCity = dCity;
   transMode = theTransMode;
   milesBetween = miles;
   startDateTime = startDate;
   arrivalDateTime = arrivalDate;
}

/***************************************************************************
* Overload Equals operator - compares two leg objects to see if they both
* contain the same start and destination cities.
* returns:  true - start city and destination city in both objects are
*                  the same.       
*           false - start and destination cities are not the same.
***************************************************************************/
bool Leg::operator == (Leg rightSide)
{
   return (startCity == rightSide.startCity && destCity == rightSide.destCity);
}

/****************************************************************************
* Overloaded "not equals" operator
* return: True - The left side object's start and destination cities
*                are not equal to the rightside object's start and destination cities.
*       False -  The start and destination cities of the two objects are equal
*****************************************************************************/
bool Leg::operator != (Leg rightSide)
{
   return (startCity != rightSide.startCity && destCity != rightSide.destCity);
}

/***************************************************************************
* Display - displays the object
*    startCity, startDate, destCity, arrivalDate, transportation mode, Miles 
***************************************************************************/
void Leg::display()
{
  
   cout << left << setw(21) << startCity << startDateTime
        << "   " << setw(21) << destCity << arrivalDateTime
        << "   " << right << setw(0)<< transMode  << setw(13-(transMode.length()))
        << milesBetween << endl;
}

/***************************************************************************
* Display - displays the object
*    startCity, startDate, destCity, arrivalDate, transportation mode, Miles 
***************************************************************************/
void Leg::display2File(ofstream &outputFile)
{
  
   outputFile << left << setw(21) << startCity << startDateTime
              << "   " << setw(21) << destCity << arrivalDateTime
              << "   " << right << setw(0)<< transMode
              << setw(13-(transMode.length())) << milesBetween << endl;
}

/******************************************************************************
 * setStartCity
******************************************************************************/
string setStartCity(string startCity)
{
   string sCity = startCity;
   return sCity;
}

/******************************************************************************
 * Menu function display the three options that this program gives
******************************************************************************/
int menu()
{
   int input;
   string errorHandle;
   do
   {
    
      cout << "1. Plan new trip\n"
           << "2. Display old trip\n"
           << "3. Quit\n";
      cin >> input;

      if (cin.fail())
      {
         getline(cin, errorHandle);
         cin.clear();
         cin.ignore(256, '\n');      
      }

      try
      {
         if (input != 1 && input != 2 && input != 3)
            throw "Please enter a number from 1 - 3\n";
      }
      catch (const char* message)
      {
         cout << message << endl;
      }
   }
   while (input != 1 && input != 2 && input != 3);
   cin.ignore();
   return input;
}


/******************************************************************************
 * Handles what to do if the user enters an unknown city. Returns the amount
 * of miles between the cities. 
******************************************************************************/
int mileageNotFound(MileageData milesModes, string startCity, string destCity)
{
   char answer;
   
   cout << "\n\tError: Mileage between " << startCity << " and "
        << destCity << " is unknown!\n";
   cout << "\nWould you still like to use these cities (y/n)? ";
   cin >> answer;
  
   if (toupper(answer) == 'Y')
   {
      cout << "Enter the mileage between " << startCity << " and "
           << destCity << ": ";
      cin >> milesModes.miles;
      cin.ignore();
   }
   else
      milesModes.miles = 0;

   return milesModes.miles;
}

/******************************************************************************
 * Choose transportation mode, must be an available method of travel. 
******************************************************************************/
int chooseTMode(int milesModes)
{
   int input;
   string errorHandle;
   bool planeAvailable = false;
   bool carAvailable = false;
   bool busAvailable = false;
   bool bikeAvailable = false;
   bool footAvailable = false;
   bool available = false;
   
   do
   {
      if(milesModes & 0x10)
      {
      cout << "1.Plane\n";
      planeAvailable = true;
      }
      else
         cout << "1.Plane (Unavailable)\n"; 
      if(milesModes & 0x04)
      {
         cout << "2.Car\n";
         carAvailable = true;
      }
      else
         cout << "2.Car (Unavailable)\n"; 
      if(milesModes & 0x08)
      {
         cout << "3.Bus\n";
         busAvailable = true;
      }
      else
         cout << "3.Bus (Unavailable)\n";
      if(milesModes & 0x02)
      {
         cout << "4.Bicycle\n";
         bikeAvailable = true;
      }
      else
         cout << "4.Bicycle (Unavailable)\n";
      if(milesModes & 0x01)
      {
         cout << "5.Foot\n";
         footAvailable = true;
      }
      else
         cout << "5.Foot (Unavailable)\n";

      cin >> input;
      cin.ignore();

      if (cin.fail())
      {
         getline(cin, errorHandle);
         cin.clear();
         cin.ignore(256, '\n');      
      }

      if ((input == 1 && (planeAvailable == true)) || (input == 2 && (carAvailable == true)) ||
          (input == 3 && (busAvailable == true)) || (input == 4 && (bikeAvailable == true)) ||
          (input == 5 && (footAvailable == true)))
      {
         available = true;
      }
                
      try
      {
         if (input != 1 && input != 2 && input != 3 && input != 4 && input != 5)
            throw "\nPlease enter a number from 1 - 5\n";
         if (available == false)
            cout << "\nThat method of travel is unavailable.\n";
         
      }
      catch (const char* message)
      {
         cout << message << endl;
         cin.clear();
         cin.ignore(256, '\n');
      }
   }
   while ((input != 1 && input != 2 && input != 3 && input != 4 && input != 5) ||
          (available == false));
      
   return input;
}

/******************************************************************************
 * Calculate time it takes to travel between two cities
******************************************************************************/
double calculateTime(int transMode, int miles, string &transport)
{
   double hoursTraveled;
   
   switch(transMode)
   {
      case 1:
         transport = "Plane";
         hoursTraveled = (double)miles / 500.0;
         return hoursTraveled;
      case 2:
         transport = "Car";
         hoursTraveled = (double)miles / 70.0;
         return hoursTraveled;
      case 3:
         transport = "Bus";
         hoursTraveled = (double)miles / 55.0;
         return hoursTraveled;
      case 4:
         transport = "Bicycle";
         hoursTraveled = (double)miles / 15.0;
         return hoursTraveled;
      case 5:
         transport = "Foot";
         hoursTraveled = (double)miles / 4.0;
         return hoursTraveled;
   }
}

/******************************************************************************
 * Converts hours in 1.5 format to 1:30:00 format
******************************************************************************/
void convertTime(double hours2Convert, int &hours, int &minutes, int &seconds)
{
   hours = hours2Convert;

   double minutes2Convert = hours2Convert - hours;

   minutes = minutes2Convert * 60.0;

   double seconds2Convert = minutes2Convert * 60.0 - minutes;

   seconds = seconds2Convert * 60.0;
}

/******************************************************************************
 * Writes a trip to a file.
******************************************************************************/
void writeTrip2File(Node<Leg>* head)
{
   string fileName;
   cout << "Enter where you want to save your trip (please include .txt at "
        << " the end of your file name): \n";

   getline(cin, fileName);

   ofstream fout;

   fout.open(fileName.c_str());
   
   fout << endl << "Your trip:\n";
   fout << "Start City           Start Date         Destination City     Arrival Date       Mode    Miles\n";     
   fout << "-------------------- ----------------   -------------------- ----------------   -----   -----\n";
   for (Node<Leg> * p = head; p; p = p->getLink())
      p->getData().display2File(fout);
   fout << endl;

   fout.close();
}

/******************************************************************************
* Opens and displays a files content to the screen, any file will do
******************************************************************************/
int openFile()
{
   string fileName;
   string file;

   cout << "\nEnter the name of the file that holds the itinerary you wish to open\n";

   getline(cin, fileName);

   ifstream fin;

   fin.open(fileName.c_str());

   if (fin.fail())
   {
      cout << "\nCould not open/find file, returning to main menu\n";
      return 0;
   }

   while (getline(fin, file))
      cout << file << endl;

   fin.close();
   return 0;
}

/*****************************************************************************
* Does everything to create a trip. Asks for start city and destination city.
* Depending if the mileage is known it calls the corresponding functions.
* Calculates the time it will take depending on the method of travel
* Get start date and time
* Calculate destination date and time
* Create linked list for the trip
* When finished making the trip, display it!
* Save the trip to a file
******************************************************************************/
void tripCreator(MileageMap &mileageChart)
{
   MileageData milesModes;
   string startCity;
   string destCity;
   string addCity;
   string transport;
   char answer;
   int transportationMode;
   int tMode;
   bool mileageFound;
   int modeNum;
   double hoursCalculated;
   Date startDate;
   Date endDate;
   int hours;
   int minutes;
   int seconds;
   Node<Leg>* head = NULL;
   Node<Leg>* endLeg = head;

   do
   {
      modeNum = 0;
      cout << "\nStart City (e.g. Miami,FL): ";
      getline(cin, startCity);
      cout << "End City   (e.g. Boise,ID): ";
      getline(cin, destCity);
      mileageFound = mileageChart.getMileage(startCity, destCity, milesModes);

      if (mileageFound)      
      {
         cout << endl << "Distance from " << startCity << " to " << destCity
              << " = " << milesModes.miles << " miles" << endl
              << "Select your mode of transportation:\n";

         transportationMode = chooseTMode(milesModes.modes);
      }
      else  // mileage between cities not found in mileage map
      {
         milesModes.miles = mileageNotFound(milesModes, startCity, destCity);
         if (milesModes.miles > 0)
            transportationMode = chooseTMode(31);         
      }
      
      if (milesModes.miles > 0)
      {
         hoursCalculated = calculateTime(transportationMode, milesModes.miles, transport);

         startDate.promptForDate("What date will you leave? (mm/dd/yyyy) ");
         
         startDate.promptForTime("What time will you leave? (hh:mm) ");
         
         convertTime(hoursCalculated, hours, minutes, seconds);
         
         endDate = startDate;
         
         endDate.addMinutes(minutes);
         
         endDate.addHours(hours);
         
         Leg newLegObject(startCity, destCity, transport, milesModes.miles, startDate, endDate);

         // createLinkedList(head, endLeg, newLegObject);
         if (head == 0)
         {
            headInsert(head, newLegObject);
            endLeg = head;
         }
   
         // insert(endLeg, newLegObject);
         else
         {   
            insert(endLeg, newLegObject);
            endLeg = endLeg->getLink();
         }
         
      }
      if (milesModes.miles > 0)
         cout << "\nYou're arrival date and time in " << destCity << " is " <<  endDate << endl;
      cin.ignore();
      cout << "\nWould you like to continue your trip? (Y/N)";
      getline(cin, addCity);
   }

   // display the trip on the screen 
   while (addCity == "Y" || addCity == "y");
   cout << endl << "Your trip:\n";
   cout << "Start City           Start Date         Destination City     Arrival Date       Mode    Miles\n";     
   cout << "-------------------- ----------------   -------------------- ----------------   -----   -----\n";
   for (Node<Leg> * p = head; p; p = p->getLink())
      p->getData().display();
   cout << endl;

   // finish with saving the trip to a file
   writeTrip2File(head); 
}

/******************************************************************************
* Main! at last! main basically runs the menu over and over as long as you
* want to use the program and will either call trip creator, open file, or quit
******************************************************************************/
int main()
{
   MileageMap mileageChart;
   int input;
   
   // menu
   cout << "---------Itinerary Planner----------\n";
   do
   {
      input = menu();
      
      switch(input)
      {
         case 1: 
            tripCreator(mileageChart);
            break;
         case 2:
            openFile();
            break;
         case 3:
            return 0; 
      }
      cout << endl;
   }
   while (input != 3);
   return 0;
}
