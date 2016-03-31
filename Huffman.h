/**
 * Rafiq Dandoo
 * Cmpt365 Spring 2016 - Final Project 
 * Adaptive Huffman Encoding
 * 
**/

#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX_VAL 512
#define BIT_SIZE 8

class Tree {
public:
  int val;
  int weight;
  int order;
  
  Tree* left;
  Tree* right;
  Tree* parent;
  
  Tree(int value, int w, int o, Tree* l, Tree* r, Tree* p):
    val(value), weight(w), order(o), left(l), right(r), parent(p)
    {}
  
  Tree():
    val(0), weight(0), order(0), left(0), right(0), parent(0)
    {}

};


#endif
