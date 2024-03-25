#ifndef _FP_H_
#define _FP_H_
#include"parser.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<list>
#include<string>
#include<string.h>
#define MAX 2147483647
class c_block
{
    private:
        string s_mname;
        c_block* parent;
        c_block* left;
        c_block* right;
        int width;
        int lenth;
        int corner_x;
        int corner_y;
        bool update_contour;  //if need to update contour, this bit is one
    public:
        list<int> con_x;      //every block has a contour after placing
        list<int> con_y;
        vector<c_block*> reg_parent;
        vector<c_block*> reg_left;
        vector<c_block*> reg_right;
        //constructor
        c_block(char p[],int x,int y){s_mname.assign(p); width=x; lenth=y; left=NULL; right=NULL; parent=NULL; update_contour=true;}
        ~c_block(){;}
        //accessor
        bool        is_update()             {return update_contour;}
        void        set_update(bool p)      {update_contour=p;}
        void        set_name(string& p)     {s_mname=p;}
        string      get_name()              {return s_mname;}
        void        set_left(c_block* p)    {left=p;}
        c_block*    get_left()              {return left;}
        void        set_right(c_block* p)   {right=p;}
        c_block*    get_right()             {return right;}
        void        set_parent(c_block* p)  {parent=p;}
        c_block*    get_parent()            {return parent;}
        void set_width(int& p){width=p;}
        void set_lenth(int& p){lenth=p;}
        int get_width(){return width;}
        int get_lenth(){return lenth;}
        void set_cor_x(int& p){corner_x=p;}
        int get_cor_x(){return corner_x;}
        void set_cor_y(int& p){corner_y=p;}
        int get_cor_y(){return corner_y;}
        //function
        void reg_info();
        void reundo();
};
class c_terminal
{
    private:
        string s_mname;
        int cor_x;
        int cor_y;
    public:
        //constructor
        c_terminal(char p[],int x,int y){s_mname.assign(p);cor_x=x; cor_y=y;}
        ~c_terminal(){;}
        //accessor
        string get_name(){return s_mname;}
        int    get_cor_x(){return cor_x;}
        int    get_cor_y(){return cor_y;}
};
class c_net
{
    private:
        int  s_mname;
        vector<c_block* > v_cblock;
        vector<c_terminal* > v_cterminal;
    public:
        //constructor
        c_net(int p){s_mname=p;}
        ~c_net(){;}
        //accessor
        void push_block(c_block* p){v_cblock.push_back(p);}
        vector<c_block* > get_vblock(){return v_cblock;}
        void push_terminal(c_terminal* p){v_cterminal.push_back(p);}
        vector<c_terminal* > get_vterminal(){return v_cterminal;}
        int get_name(){return s_mname;}
        //
        int caculate_HPWL();
};
class c_bs
{
    private:
        c_block* root;
        list<int>* contour_x;                   //regist the contour_x now, not necessary, so use point to boost
        list<int>* contour_y;
        vector<c_block*>    v_cblock;
        vector<c_terminal*> v_cterminal;
        vector<c_net*>      v_cnets;
        int     initial_temperature;
        double  now_temperature;                
        double  decrease_slope;                 //0~1     
        double     lowest_cost;
        vector<double> cost;
        double alpha;
        int outline_x;
        int outline_y;
        const char* output_file;
        //function
        bool decide_operation();
        bool swap(c_block*, c_block*);
        bool move(c_block*, c_block*, int);
        void place_right(c_block* , c_block*); //choose place coordinate and update contour
        void place_top(c_block*, c_block*);    //
        void update_afterop();                  //doing dfs() and update contour
        void update_contour(c_block* );        //update contour function. used for place
        void print_contour();
        bool computer_cost(double& , int& , int&);
        //sa->decide_operation->swap/move->update_afterop->place_right/place_top->update_contour->computer_cost
        double get_prob(double);
    public:
        //constructor
        c_bs(parser* ,const char* ,const char*);
        ~c_bs(){;}
        //
        void dump();
        void dump_tree(c_block*);
        void sa_first();
        void sa();
        void build_bstar();
        void print_svg();
        void print_rpt(double);
};






#endif
