#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <functional>
#include <unordered_map>
#include "TechnologyLibrary.h"
#pragma once

using Component = TechnologyLibrary::Component;
using TreeNode = TechnologyLibrary::TreeNode;
using param = TechnologyLibrary::param;

//Netlist Struct
struct Equation{
    Equation(){};
    Equation(std::string _arg1, std::string _arg2, Component _component);
    void printEquation();
    std::string arg1, arg2;
    Component component;
};

struct IO{
    //0 - output, 1 - input
    IO(){};
    IO(std::string _name, bool _inputOrOutput);
    void printIO();
    std::string name;
    bool inputOrOutput;
};

void println(std::string str);

class TechnologyMap{
    private:
        std::unordered_map<std::string, bool> vectorIO;
        std::unordered_map<std::string, Equation> netlist;
        TreeNode* root = nullptr;
        unsigned int minimalCost;
    public:
        TechnologyMap();
        ~TechnologyMap();

        //I/O
        void readNetListFile(std::string name = "netlist.txt");
        void writeOutputFile(std::string name = "output.txt");

        //Netlist and IO nodes
        void printNetlist();
        void printvectorIO();

        //Netlist to tree
        void netlistToTree();
        void printTree();

        //pattern match
        bool patternMatch(TreeNode* tree, TreeNode* pattern);
        
        int computeMinimalCost();
    private:
        void deleteTree(TreeNode*);
        void printTreeHelper(TreeNode* node, int level);
        TreeNode* netlistToTreeHelper(Equation eqn);
        TreeNode* netlistToTreeHelperRegular(Equation eqn);
        bool patternMatchHelper(TreeNode* node, TreeNode* pattern);
        bool patternMatchAtNode(TreeNode* node, TreeNode* pattern);
        void transverseToDescendantNode(TreeNode* node, TreeNode* pattern, std::vector<TreeNode*> &children);
        std::pair<bool, std::vector<TreeNode*>> patternMatchAtNodeKeepChildren(TreeNode* node, TreeNode* pattern);
        int computeMinimalCostHelper(TreeNode* node, std::unordered_map<Component, std::pair<TreeNode*, TreeNode**>> &patternLookUp);
};