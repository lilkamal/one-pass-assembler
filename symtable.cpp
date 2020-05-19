#include <unordered_map>
#include <iostream>

#include "symtable.h"

using namespace std;

Symtable* Symtable::unique = 0;
unordered_map<string, Sym*> symtable;

Symtable::Symtable() {}

Symtable* Symtable::getInstance() {
	if(Symtable::unique == 0)
		Symtable::unique = new Symtable();
	return Symtable::unique;
}

void Symtable::insert(string symbol) {
	Sym* sym = new Sym();
	sym->name = symbol;
	sym->address = "*";
	symtable[symbol] = sym;
}

void Symtable::insert(std::string symbol, std::string address, bool abs, int block, int sec) {
	Sym* sym = new Sym();
	sym->name = symbol;
	sym->address = address;
	sym->absolute = abs;
	sym-> control_section = sec;
	sym -> program_block = block;
	symtable[symbol] = sym;
}

Sym* Symtable::getSymbol(string symbol) {
	std::unordered_map<string, Sym*>::const_iterator got = symtable.find(symbol);
    if(got == symtable.end())
    	return 0;
    return got->second;
}

/*int main() {
	Symtable* sys = Symtable::getInstance();
	sys->insert("COPY");
	Sym* syu = sys->getSymbol("COPY");
	cout << syu-> name << "\t" << syu->address << endl;
} */