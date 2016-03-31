/**
 * Rafiq Dandoo
 * Cmpt365 Spring 2016 - Final Project 
 * Adaptive Huffman Encoding
 * 
**/

#include <iostream>
#include <map>
#include <assert.h> 
#include <bitset>
#include <vector>
#include <sstream> 


#include "Huffman.h"

#define MAX_VAL 512



std::string encoder( std::string word );
void createNewNYT( Tree* currNYT, Tree* newNYT, Tree* newChar, char c);
void update( Tree* leaf);
void sendCode(Tree* tree);
Tree* makeHighestOrder(std::map<char, Tree*>* leafNodes, Tree* leaf);

int main(int argc, char** argv){
    
    std::string word;
    std::stringstream ss;
    ss << std::cin.rdbuf();
    encoder( ss.str() ) ;
    std::cout << "\n";
    return 0;
}

std::string encoder( std::string word ){
    
    std::map<char, int> characters;
    std::map<char, Tree*> leafNodes;
    Tree* currNYT;
    
    Tree root( 0, 0, MAX_VAL, 0, 0, 0 );
    
    currNYT = &root;
    for( auto& c : word ){
        auto found = characters.find( c );
        if( found == characters.end() ){ // do stuff for new character
            sendCode( currNYT );
            std::bitset< BIT_SIZE > x(c);
            std::cout << x;  // print here
            std::pair<char,int> p(c,1);
            characters.insert( p );
            Tree* newNYT = new Tree();
            Tree* newChar = new Tree();
            std::pair<char,Tree*> leaf( c, newChar );
            leafNodes.insert( leaf );
            createNewNYT( currNYT, newNYT, newChar, c );
            currNYT = newNYT;
        }else{                          // do stuff for already seen character
            found->second += 1;
            auto node = leafNodes.find( c );
            if( node == leafNodes.end() ){
                assert(0);
            }
            Tree* leaf = node->second;
            sendCode( leaf );
            // makes this the highest order node of it's weight
            Tree* temp = makeHighestOrder( &leafNodes, leaf );
            update( temp );
        }
    }
    return word;
}

void createNewNYT( Tree* currNYT, Tree* newNYT, Tree* newChar, char c){
    currNYT->left = newNYT;
    currNYT->val = 0;
    currNYT->right = newChar;
    
    newNYT->val = -1;
    newNYT->parent = currNYT;
    newNYT->order = currNYT->order - 2;
    
    newChar->val = c;
    newChar->parent = currNYT;
    newChar->order = currNYT->order - 1;
    
    update( newChar );
}

void update( Tree* leaf){
    
    Tree* curr = leaf->parent;
    leaf->weight++;
    
    while( curr->parent ){
        curr->weight++;
        if( curr->order == curr->parent->left->order ){ // check that this isn't the highest order node of it's weight
            
            if( curr->weight > curr->parent->right->weight ){ // check if we need to swap
                int l = curr->order;
                int r = curr->parent->right->order;
                Tree* temp;
                temp = curr->parent->right;
                curr->parent->right = curr;
                curr->parent->left = temp;
                curr->parent->right->order = r;
                curr->parent->left->order = l;
            }
        }
        curr = curr->parent;
    }
    
}

void sendCode(Tree* tree){
    
    Tree* curr = tree;
    std::string code;
    while( curr->parent ){
        if( curr->order == curr->parent->left->order ){
            code = "0" + code;
        }else{
            code = "1" + code;
        }
        curr = curr->parent;
    }
    
    if( code == ""){
        code = "0";
    }
    
    std::cout << code;
}

Tree* makeHighestOrder(std::map<char, Tree*>* leafNodes, Tree* leaf){
    
    Tree* max = leaf;
    for( auto& l : *leafNodes){
        if( l.second->weight == max->weight && l.second->order > max->order 
            && leaf->parent->order != l.second->order ){
            max = l.second;
        }
    }
    if( max->order != leaf->order ){
        // swap here
        //std::cout << "makeHighest swapping " << (char)max->val << " and " << (char)leaf->val << "\n";
        int temp = leaf->val;
        leaf->val = max->val;
        max->val = temp;
        leafNodes->find( (char) max->val )->second = max;
        leafNodes->find( (char) leaf->val)->second = leaf;
        return max;
    }
    return leaf;
}