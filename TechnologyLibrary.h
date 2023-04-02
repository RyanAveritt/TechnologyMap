#include <string>
#include <vector>
#include <unordered_map> //hash
#pragma once

//static library
class TechnologyLibrary {
    public:
        enum class Component { UNK, INPUT, OUTPUT, NOT, NAND2, AND2,  NOR2, OR2, AOI21, AOI22 };

        struct param{
            std::string name, symbol = "";
            int numberOfArguments, cost;
            param(std::string _name, int _numberOfArguments, int _cost):name(_name),numberOfArguments(_numberOfArguments),cost(_cost){};
            param(std::string _name, std::string _symbol, int _numberOfArguments, int _cost):name(_name),symbol(_symbol),numberOfArguments(_numberOfArguments),cost(_cost){};
        };

        struct TreeNode {
            Component component;
            TreeNode* child1 = nullptr;
            TreeNode* child2 = nullptr;
            TreeNode(Component _component) : component(_component){};
            TreeNode() : component(Component::UNK){};
            void setComponent(Component _component){component = _component;}
        };


        static param componentToStruct(Component component){
            if(componentLookup_.find(component) != componentLookup_.end()) return componentLookup_.at(component);
            return componentLookup_.at(Component::UNK);
        }

        static Component stringToComponent(std::string component){
            for (auto& element : componentLookup_) {
                if(element.second.name == component) return element.first;
            }
            return Component::UNK;
        }

        static std::pair<TreeNode*, TreeNode**> componentNandNot(Component component){
            std::pair<TreeNode*, TreeNode**> pair;
            TreeNode* parentNode = nullptr;
            pair.second = new TreeNode*[4];
            pair.second[0] = nullptr;
            pair.second[1] = nullptr;
            pair.second[2] = nullptr;
            pair.second[3] = nullptr;            
            switch (component) {
                case Component::INPUT:
                    parentNode = new TreeNode(Component::INPUT);
                case Component::OUTPUT:
                    parentNode = new TreeNode(Component::OUTPUT);
                    break;
                case Component::NOT:
                    parentNode = new TreeNode(Component::NOT);
                    parentNode->child1 = new TreeNode();
                    pair.second[0] = parentNode->child1;
                    break;
                case Component::NAND2:
                    parentNode = new TreeNode(Component::NAND2);
                    parentNode->child1 = new TreeNode();
                    parentNode->child2 = new TreeNode();
                    pair.second[0] = parentNode->child1;
                    pair.second[1] = parentNode->child2;
                    break;
                case Component::AND2:
                    parentNode = new TreeNode(Component::NOT);
                    parentNode->child1 = new TreeNode(Component::NAND2);
                    parentNode->child1->child1 = new TreeNode();
                    parentNode->child1->child2 = new TreeNode();
                    pair.second[0] = parentNode->child1->child1;
                    pair.second[1] = parentNode->child1->child2;
                    break;
                case Component::NOR2:
                    parentNode = new TreeNode(Component::NOT);
                    parentNode->child1 = new TreeNode(Component::NAND2);
                    parentNode->child1->child1 = new TreeNode(Component::NOT);
                    parentNode->child1->child2 = new TreeNode(Component::NOT);
                    parentNode->child1->child1->child1 = new TreeNode();
                    parentNode->child1->child2->child1 = new TreeNode();
                    pair.second[0] = parentNode->child1->child1->child1;
                    pair.second[2] = parentNode->child1->child2->child1;
                    break;
                case Component::OR2:
                    parentNode = new TreeNode(Component::NAND2);
                    parentNode->child1 = new TreeNode(Component::NOT);
                    parentNode->child2 = new TreeNode(Component::NOT);
                    parentNode->child1->child1 = new TreeNode();
                    parentNode->child2->child1 = new TreeNode();
                    pair.second[0] = parentNode->child1->child1;
                    pair.second[2] = parentNode->child2->child1;
                    break;
                case Component::AOI21:
                    parentNode = new TreeNode(Component::NOT);
                    parentNode->child1 = new TreeNode(Component::NAND2);
                    parentNode->child1->child1 = new TreeNode(Component::NAND2);
                    parentNode->child1->child2 = new TreeNode(Component::NOT);
                    parentNode->child1->child1->child1 = new TreeNode();
                    parentNode->child1->child1->child2 = new TreeNode();
                    parentNode->child1->child2->child1 = new TreeNode();
                    pair.second[0] = parentNode->child1->child1->child1;
                    pair.second[1] = parentNode->child1->child1->child2;
                    pair.second[2] = parentNode->child1->child2->child1;
                    break;
                case Component::AOI22:
                    parentNode = new TreeNode(Component::NOT);
                    parentNode->child1 = new TreeNode(Component::NAND2);
                    parentNode->child1->child1 = new TreeNode(Component::NAND2);
                    parentNode->child1->child2 = new TreeNode(Component::NAND2);
                    parentNode->child1->child1->child1 = new TreeNode();
                    parentNode->child1->child1->child2 = new TreeNode();
                    parentNode->child1->child2->child1 = new TreeNode();
                    parentNode->child1->child2->child2 = new TreeNode();
                    pair.second[0] = parentNode->child1->child1->child1;
                    pair.second[1] = parentNode->child1->child1->child2;
                    pair.second[2] = parentNode->child1->child2->child1;
                    pair.second[3] = parentNode->child1->child2->child2;
                    break;
                default:
                    break;
            }
            pair.first = parentNode;
            return pair;
        }

    private:
        typedef std::unordered_map<Component, param> componentLookup;
        inline static componentLookup componentLookup_ = {
            {Component::UNK, param("UNK", 0, 0)}, 
            {Component::INPUT, param("INPUT", 0, 0)}, 
            {Component::OUTPUT, param("OUTPUT", 0, 0)}, 
            {Component::NOT, param("NOT", 1, 2)},
            {Component::NAND2, param("NAND", 2, 3)},
            {Component::AND2, param("AND", "*", 2, 4)},
            {Component::NOR2, param("NOR", 2, 6)},
            {Component::OR2, param("OR", "+", 2, 4)},
            {Component::AOI21, param("AOI21", 3, 7)},
            {Component::AOI22, param("AOI22", 4, 7)}
        };
};