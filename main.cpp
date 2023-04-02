#include "TechnologyMap.h"
void testRead();
void testTree();
void actual();

int main(){
    actual();
    return 0;
}

void actual(){
    TechnologyMap tm = TechnologyMap();
    tm.readNetListFile("netlist.txt");
    tm.netlistToTree();
    tm.computeMinimalCost();
    tm.writeOutputFile();
}

void testRead(){
    TechnologyMap tm = TechnologyMap();
    tm.readNetListFile("netlist.txt");
    tm.printNetlist();
    tm.printvectorIO();
}

void testTree(){
    TechnologyMap tm = TechnologyMap();
    tm.readNetListFile("netlist.txt");
    tm.netlistToTree();
    tm.printTree();
}
