#include<iomanip>
#include<stdlib.h>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <math.h>
#include <algorithm>
#include <cstdio>

#include "opcodes.h"
#include "Textcodes.h"
#include "symtable.h"
using namespace std;


  Textcodes::Textcodes(){
  length=0;
  }

  bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
 void Textcodes::newText(int locctr)
    {
        stringstream stemp;
        stemp<<setw(6)<<setfill('0')<<locctr;
        start = "T^" + stemp.str() + "^";
        length=0;
        currtext = "";
    }
 void  Textcodes::addText(vector<string> data,long long flags,int numofbites)
    {
       Opcodes* opcod = Opcodes::getInstance();
        string data1 = data[1];
       if(numofbites == 4)
            data1 = data1.substr(1, data1.size() - 1);
       string opcode = opcod->getopcode(data1);
       //cout<<"  "<<opcode << endl;
       currtext+="^";

       Symtable* sys = Symtable::getInstance();
       string data2=data[2];
       if (data2[0] == '#' || data2[0] == '@')
        data2 = data2.substr(1, data2.size() - 1);
       if (data2.size() > 2 && data2[data2.size() - 1] == 'X' && data2[data2.size() - 2] == ',') {
           data2 = data2.substr(0, data2.size() - 2);
       }
       //cout << data1 << ' ' << data2 << '\n';
       long long curraddress = 0;
       stringstream strs;
       strs.str("");
     if(is_number(data2)){
       //flags &= 61;
       curraddress = stoi(data2);
       //cout << curraddress << "Makram" <<'\n';

     }else{

        Sym* syu = sys->getSymbol(data2);
       if( syu == nullptr || syu->address == "*"){
        curraddress = 0;
        flags &= 61;
       // cout<<"symm"<<endl;
       }
       else {
        strs << (syu->address);
        strs >> curraddress;
        strs.str("");
       }

    }
    long long objectCode=0;
    long long lopcode,lcurr;
    //std::string s = "0xfffefffe";
    lopcode = std::stoul(opcode, nullptr, 16);
   /* strs.str("");
    strs << hex <<opcode;
    strs >> lopcode;*/
   // cout<<lopcode<<" opp1"<<endl;

    if ((flags & (1 << 4)) != 0 && numofbites != 2)
        lopcode++;
    if ((flags & (1 << 5)) != 0 && numofbites != 2)
        lopcode+=2;
    //cout<<lopcode<<" opp2"<<endl;
   // cout << currtext << endl;
        strs.clear();

    strs.str("");
    strs<<setw(2)<<setfill('0')<<hex<<lopcode;
    string sol;
    strs >> sol;

    currtext+=sol;
    //cout << currtext << "here 1" << endl;
   // cout<<"sda"<<lopcode<<endl;
    if (numofbites == 3){
        for (int i = 0; i <= 3; i++){
            if ((flags & (1 << i)) != 0)
                objectCode |= (1 << (i + 12));
         }
    }
    else if (numofbites == 4){
        for (int i = 0; i <= 3; i++){
            if ((flags & (1 << i)) != 0)
                objectCode |= (1 << (i + 20));
         }
    }
    objectCode |= curraddress;

      //cout << objectCode << "here 2" << endl;
      stringstream str2;
      //sol = "";
      str2<<setw(4)<<setfill('0')<<hex<<objectCode;
      str2 >> sol;
      //sprintf(sol, "%X", objectCode);
      currtext+=sol;
      length+=numofbites;
    }
void Textcodes::addText(std::vector<std::string> data, Sym* label) {

    stringstream str;
    str << setw(4) << setfill('0') << hex << label->address;
    str >> currtext;
    length = currtext.size() / 2;

    for (int i = 0; i < label->operandsNeedThisLabel.size(); i++)
    {
        stringstream stream;
        std::list<std::string>::iterator it = label->operandsNeedThisLabel.begin();
        std::advance(it, i);
        stream << setw(6) << setfill('0') << *it;
        start = "T^" + stream.str() + "^";
        cout << tostring() << '\n';
    }
}

string ToHex(const string& s, bool upper_case /* = true */)
{
    ostringstream ret;

    for (string::size_type i = 0; i < s.length(); ++i)
        ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << (int)s[i];

    return ret.str();
}

void Textcodes::addText(std::vector<std::string> data, int locctr)
{

    if (data[1] == "RESW" || data[1] == "RESB")
    {
        if (length != 0) {
            cout << tostring() << '\n';
        }
        if (data[1] == "RESW")
            newText(locctr + 3 * stoi(data[2]));
        else
            newText(locctr +  stoi(data[2]));
    }
    else {
        if (data[1] == "WORD")
        {
            if (length + 3 > 30)
            {
                cout << tostring() << '\n';
                newText(locctr);
            }
            stringstream streamo;
            streamo << setw(6) << setfill('0') << hex << data[2];
         //   cout << data[2] << ' ' << stream.str() << "******&&&&&&&&&&&&&&&&&&&&&" << endl;
            string ans;
            streamo >> ans;
            streamo.str("");
            currtext += "^" + ans;
            length += 3;
        }
        else {
            string hexa;
            if (data[2][0] == 'C' || data[2][0] == 'c')
            {
                hexa = ToHex(data[2].substr(2, data[2].size() - 3), true);
            }
            else {
                hexa = data[2].substr(2, data[2].size() - 3);
            }
            if (length + (hexa.size() / 2) > 30)
            {
                cout << tostring() << '\n';
                newText(locctr);
            }
            currtext += "^" + hexa;
            length += (hexa.size() / 2);
        }
    }
}

 string Textcodes::tostring()
  {
        stringstream stemp;
        stemp<<setw(2)<<setfill('0')<<hex<<length;
        return start+ stemp.str()+"^" +currtext;
  }

