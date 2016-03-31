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


std::string decoder( std::string word );
void createNewNYT( Tree* currNYT, Tree* newNYT, Tree* newChar, char c);
void update( Tree* leaf);
Tree* makeHighestOrder(std::map<char, Tree*>* leafNodes, Tree* leaf);

int main(int argc, char** argv){
    
        
    std::string word;
    std::stringstream ss;
    ss << std::cin.rdbuf();
    word = decoder( ss.str() ) ;
    std::cout << "\n";
    
    double csize = ss.str().size();
    double ucsize = word.size() * sizeof( char ) * 8;
    std::cout << "\nCompression Info:\n";
    std::cout << "Compressed Size: " << csize << " bits\n";
    std::cout << "Uncompressed Size: " << ucsize << " bits\n";
    std::cout << "Compressed Size is " << csize/ucsize *100<< "% of the original\n"; 
    return 0;
}

std::string decoder( std::string word ){
    
    bool readNewChar = true;
    std::map<char, int> characters;
    std::map<char, Tree*> leafNodes;
    Tree* currNode;
    
    Tree root( -1, 0, MAX_VAL, 0, 0, 0 );
    currNode = &root;
    
    
    std::string result;
    for( int i = 0; i < word.size() ; ++i){
        
        if( word[i] == '0' && !readNewChar){
            currNode = currNode->left;
        }else if(word[i] == '1' && !readNewChar){
            currNode = currNode->right;
        }
        
        if( currNode->val == -1 ){
            readNewChar = true;
        }
        
        if( currNode->val != -1 && currNode->val != 0){
            result += (char)currNode->val;
            Tree* temp = makeHighestOrder( &leafNodes, currNode );
            update( temp );
            currNode = &root;
        }
        
        if(readNewChar){
            ++i;
            std::string num( word.substr(i,BIT_SIZE) );
            
            char v = (char) std::bitset<BIT_SIZE>(num).to_ulong();
            i += BIT_SIZE-1;
            readNewChar = false;
            Tree* newNYT = new Tree();
            Tree* newChar = new Tree();
            createNewNYT( currNode, newNYT, newChar, v );
            std::pair<char, Tree*> p(v,newChar);
            leafNodes.insert( p );
            currNode = &root;
            result += (char)newChar->val;
        }
    }
    
    std::cout << result;
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
        int temp = leaf->val;
        leaf->val = max->val;
        max->val = temp;
        leafNodes->find( (char) max->val )->second = max;
        leafNodes->find( (char) leaf->val)->second = leaf;
        return max;
    }
    return leaf;
}
