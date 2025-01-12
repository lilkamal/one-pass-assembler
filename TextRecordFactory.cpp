#include <iostream>
#include <iomanip>
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>

#include "TextRecordFactory.h"
#include "Textcodes.h"
#include "symtable.h"
using namespace std;

TextRecordFactory* TextRecordFactory::textRecFactory  = nullptr;

bool dataGenerationDirectives(string oper)
{
    if (oper == "BYTE" || oper == "WORD" || oper == "RESW" || oper == "RESB")
        return true;
    else
        return false;
}

TextRecordFactory::TextRecordFactory(){

    textRecord = new Textcodes();
    textRecord->newText(0);
    valid = true;
}

TextRecordFactory* TextRecordFactory::getInstance()
{
    if (textRecFactory == nullptr)
        textRecFactory = new TextRecordFactory();

    return textRecFactory;
}
Sym* TextRecordFactory::modifyPrevAddress(string label) {

    if (label == "")
        return nullptr;
    Symtable* table = Symtable::getInstance();
    Sym* symb = table->getSymbol(label);

    if (symb == nullptr || symb->operandsNeedThisLabel.size() == 0)
        return nullptr;

    return symb;
}
void TextRecordFactory::breakText(int locctr)
{
    if(textRecord->length > 0){
        cout << textRecord->tostring() << endl;
        textRecord->newText(locctr);
    }

}
void TextRecordFactory::addTextRecord(const vector<string>& statement, int locctr,long long flags,int numofbites ){

    //cout << statement[1] << "***************************" <<  '\n';
    int status = 0;
    Sym* symb = modifyPrevAddress(statement.at(0));
    if (dataGenerationDirectives(statement[1]))
    {
      //  cout << "First If******************************"<<endl;

        textRecord->addText(statement, locctr);
        if (symb != nullptr)
        {
            if (textRecord->length > 0) {
            //cout << textRecord->tostring() << '\n';
                textRecord->newText(locctr);
            }
            textRecord->addText(statement, symb);
            textRecord->newText(locctr);
        }
    }
    else if (symb != nullptr)
    {
        if (textRecord->length > 0) {
            cout << textRecord->tostring() << '\n';
        }
        textRecord->addText(statement, symb);
        textRecord->newText(locctr);
    }
    else {
        // i dont know the limit :(
       if (textRecord->length  + numofbites >= 30){
            cout << textRecord->tostring() << '\n';
            textRecord->newText(locctr);
        }
       else  if(textRecord->length == 0)
        textRecord->newText(locctr);
      textRecord->addText(statement,flags,numofbites, locctr);
      //to see cuurtext
     cout<< textRecord->tostring()<<endl;
    }
}
 string TextRecordFactory::gettextrecord(){
     return textRecord->tostring();
     }
