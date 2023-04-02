#include "TechnologyMap.h"
//Structures
Equation::Equation(std::string _arg1, std::string _arg2, Component _component) : arg1(_arg1), arg2(_arg2), component(_component){};
IO::IO(std::string _name, bool _inputOrOutput): name(_name), inputOrOutput(_inputOrOutput){};

void println(std::string str){std::cout << str << std::endl;}

void Equation::printEquation(){println(std::string(" = " + arg1 + " " + TechnologyLibrary::componentToStruct(component).name + " " + arg2));}

void IO::printIO(){println(std::string(name + ": " +  (inputOrOutput ? "INPUT" : "OUTPUT")));}

//TechnologyMap
TechnologyMap::TechnologyMap(){}

TechnologyMap::~TechnologyMap(){
    deleteTree(root);
    root = nullptr;
}

void TechnologyMap::deleteTree(TreeNode* node){
    if (node == nullptr) return;
    if(node->child1 != nullptr) deleteTree(node->child1);
    if(node->child2 != nullptr) deleteTree(node->child2);
    delete node;
}

void TechnologyMap::printNetlist(){
    std::cout << "netlist" << std::endl;
    for (auto& element : netlist){
        std::cout << element.first;
        element.second.printEquation();
    }
    std::cout << std::endl;
}

void TechnologyMap::printvectorIO(){
    std::cout << "vectorIO" << std::endl;
    for (auto& element : vectorIO){
        std::cout << element.first << ": " << (element.second ? "INPUT" : "OUTPUT") << std::endl;
    }
    std::cout << std::endl;
}

void TechnologyMap::readNetListFile(std::string name){
    std::ifstream inFile(name);
    if(inFile.is_open()) {
        std::string line;
        while(std::getline(inFile, line)) {
            std::istringstream stream(line);
            std::string token1, token2, token3, token4, token5;
            if(line.find("=") != std::string::npos){
                std::getline(stream, token1, ' ');
                std::getline(stream, token2, ' ');
                std::getline(stream, token3, ' ');
                std::getline(stream, token4, ' ');
                std::getline(stream, token5, ' ');
                //t# = Comp node node
                std::string key = token1;
                Component component = TechnologyLibrary::stringToComponent(token3);
                Equation eqn = Equation(token4, token5, component);
                if (eqn.arg2!="" && eqn.arg1==""){
                    eqn.arg1 = eqn.arg2;
                    eqn.arg2 = "";
                }
                netlist.emplace(key, eqn);
            }else{
                std::getline(stream, token1, ' ');
                std::getline(stream, token2, ' ');
                vectorIO.emplace(token1, token2=="INPUT");
            }
        }
        inFile.close();
    }else std::cout << "Not open." << std::endl;
}

void TechnologyMap::writeOutputFile(std::string name){
    std::ofstream outFile(name);
    outFile << minimalCost;
    outFile.close();
}

void TechnologyMap::netlistToTree(){
    for (auto& element : vectorIO){
        if(element.second == 0){
            if(netlist.find(element.first)!=netlist.end()){
                root = new TreeNode(Component::OUTPUT);
                root->child1 = netlistToTreeHelper(netlist.at(element.first));
                break;
            }
        }
    }
}

TreeNode* TechnologyMap::netlistToTreeHelper(Equation eqn){
    //if [*, *, null, null] => AND
    //if [*, null, * null] => OR
    //if [*, null, null, null] => NOT
    std::pair<TreeNode*, TreeNode**> pair = TechnologyLibrary::componentNandNot(eqn.component);
    TreeNode** input = pair.second;
    if(input[0] != nullptr){
        if(vectorIO.find(eqn.arg1) != vectorIO.end()){
            input[0]->setComponent(Component::INPUT);
        }else{
            TreeNode* node = netlistToTreeHelper(netlist.at(eqn.arg1));
            input[0]->setComponent(node->component);
            input[0]->child1 = node->child1;
            input[0]->child2 = node->child2;
        }
    }

    if(input[1] != nullptr){
        if(vectorIO.find(eqn.arg2) != vectorIO.end()){
            input[1]->setComponent(Component::INPUT);
        }else{
            TreeNode* node = netlistToTreeHelper(netlist.at(eqn.arg2));
            input[1]->setComponent(node->component);
            input[1]->child1 = node->child1;
            input[1]->child2 = node->child2;
        }
    }

    if(input[2] != nullptr){
        if(vectorIO.find(eqn.arg2) != vectorIO.end()){
            input[2]->setComponent(Component::INPUT);
        }else{
            TreeNode* node = netlistToTreeHelper(netlist.at(eqn.arg2));
            input[2]->setComponent(node->component);
            input[2]->child1 = node->child1;
            input[2]->child2 = node->child2;
        }
    }

    return pair.first;
}

TreeNode* TechnologyMap::netlistToTreeHelperRegular(Equation eqn){
    TreeNode* node = new TreeNode(eqn.component);
    if(vectorIO.find(eqn.arg1) != vectorIO.end()){
        node->child1 = new TreeNode(Component::INPUT);
    }else{
        node->child1 = netlistToTreeHelper(netlist.at(eqn.arg1));
    }
    if(vectorIO.find(eqn.arg2) != vectorIO.end()){
        node->child2 = new TreeNode(Component::INPUT);
    }else{
        node->child2 = netlistToTreeHelper(netlist.at(eqn.arg2));
    } 
    return node;
}

void TechnologyMap::printTree(){
    printTreeHelper(root, 0);
}

void TechnologyMap::printTreeHelper(TreeNode* node, int level){
    std::cout << "level: " << level << " " << TechnologyLibrary::componentToStruct(node->component).name << ": " 
    << ((node->child1!=nullptr) ? TechnologyLibrary::componentToStruct(node->child1->component).name : "nullptr") << " "
    << ((node->child2!=nullptr) ? TechnologyLibrary::componentToStruct(node->child2->component).name : "nullptr") << std::endl;

    if(node->child1!=nullptr) printTreeHelper(node->child1, level+1);
    if(node->child2!=nullptr) printTreeHelper(node->child2, level+1);
}

// //based off preorder transversal
// bool TechnologyMap::patternMatchHelper(TreeNode* node, TreeNode* pattern){
//     if (pattern == nullptr) return true;
//     if (node == nullptr) return false;
//     //preorder transversal
//     if (node->component == pattern->component && patternMatchHelper(node->child1, pattern->child1) && patternMatchHelper(node->child2, pattern->child2))
//         return true;
//     //pattern of left or right
//     return patternMatchHelper(node->child1, pattern) || patternMatchHelper(node->child2, pattern);
// }

// bool TechnologyMap::patternMatch(TreeNode* tree, TreeNode* pattern){
//     //base cases
//     if (pattern == nullptr) return true;
//     if (tree == nullptr) return false;
//     //start transversal on head node
//     if (patternMatchHelper(tree, pattern)) return true;
//     //increment to child/ancestor nodes
//     return patternMatch(tree->child1, pattern) && patternMatch(tree->child2, pattern);
// }

bool TechnologyMap::patternMatchAtNode(TreeNode* node, TreeNode* pattern){
    if (pattern == nullptr) return true;
    if (node == nullptr) return false;
    return ((node->component == pattern->component || pattern->component == Component::UNK) && patternMatchAtNode(node->child1, pattern->child1) && patternMatchAtNode(node->child2, pattern->child2));
}

//needs a parent attribute
void TechnologyMap::transverseToDescendantNode(TreeNode* node, TreeNode* pattern, std::vector<TreeNode*> &children) {
    if (node == nullptr || pattern == nullptr) return;
    if (pattern->child1 != nullptr && pattern->child1->component == Component::UNK && node->child1 != nullptr) children.push_back(node->child1);
    if (pattern->child2 != nullptr && pattern->child2->component == Component::UNK && node->child1 != nullptr) children.push_back(node->child2);
    transverseToDescendantNode(node->child1, pattern->child1, children);
    transverseToDescendantNode(node->child2, pattern->child2, children);
}

int TechnologyMap::computeMinimalCost(){
    std::unordered_map<Component, std::pair<TreeNode*, TreeNode**>> patternLookUp = {
    {Component::NOT, TechnologyLibrary::componentNandNot(Component::NOT)},
    {Component::NAND2, TechnologyLibrary::componentNandNot(Component::NAND2)},
    {Component::AND2, TechnologyLibrary::componentNandNot(Component::AND2)},
    {Component::NOR2, TechnologyLibrary::componentNandNot(Component::NOR2)},
    {Component::OR2, TechnologyLibrary::componentNandNot(Component::OR2)},
    {Component::AOI21, TechnologyLibrary::componentNandNot(Component::AOI21)},
    {Component::AOI22, TechnologyLibrary::componentNandNot(Component::AOI22)}};

    minimalCost = computeMinimalCostHelper(root->child1, patternLookUp);

    for (auto& pattern : patternLookUp) {
        delete[] pattern.second.second;           
        deleteTree(pattern.second.first);
    }
    return minimalCost;
}

int TechnologyMap::computeMinimalCostHelper(TreeNode* node, std::unordered_map<Component, std::pair<TreeNode*, TreeNode**>> &patternLookUp){
    if (node == nullptr) return 0;
    int minCost = INT_MAX;
    for (auto& pattern : patternLookUp) {
        //pattern.first = Component
        //pattern.second.first = TreeNode* (parent/head)
        //pattern.second.second = TreeNode** (children)
        if (patternMatchAtNode(node, pattern.second.first)) {
            int patternCost = TechnologyLibrary::componentToStruct(pattern.first).cost;
            std::vector<TreeNode*> children;
            transverseToDescendantNode(node, pattern.second.first, children);
            for (auto& child : children) {
                patternCost += computeMinimalCostHelper(child, patternLookUp);
            }
            return std::min(minCost, patternCost);
        }
    }
    return 0;
}