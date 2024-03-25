#ifndef _PARSER_H_
#define _PARSER_H_
#include<iostream>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<iomanip>
using namespace std;
class block_s
{
    public:
        char m_name[256];
        bool b_t;           //is this block or terminal, 0 is block 1 is terminal
        int  m_nx;          //if this is a block x,y mean lenth
        int  m_ny;          //else mean terminal  x,y coordinate
        //constructor
        block_s(char[] ,int, int,bool);
        ~block_s(){;}
};
class net_s
{
    public:
        int name;
        vector<char* > vblock;
        net_s(int p){name=p;}
        ~net_s(){;}
};

class parser
{
    public:
        vector<block_s*> m_sblock;
        vector<net_s*>   m_snet;
        int max_x;
        int max_y;
        int n_blocks;
        int n_terminals;
        int n_nets;
        
        //constructor
        parser(const char* , const char*);
        ~parser(){;}
        void dump();
};



#endif
