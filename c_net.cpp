#include"fp.h"
#include<fstream>
int c_net::caculate_HPWL()
{
    int left_x=MAX;
    int right_x=0;
    int top_y=0;
    int bottom_y=MAX;
    c_block* tmp;
    for(unsigned int x=0;x<v_cblock.size();x++)
    {
        tmp=v_cblock[x];
        int tmpx=tmp->get_cor_x()+tmp->get_width()/2;
        int tmpy=tmp->get_cor_y()+tmp->get_lenth()/2;
        if(tmpx<left_x)
            left_x=tmpx;
        if(tmpx>right_x)
            right_x=tmpx;
        if(tmpy>top_y)
            top_y=tmpy;
        if(tmpy<bottom_y)
            bottom_y=tmpy;
    }
    for(unsigned int x=0;x<v_cterminal.size();x++)
    {
        if(v_cterminal[x]->get_cor_x()<left_x)
            left_x=v_cterminal[x]->get_cor_x();
        if(v_cterminal[x]->get_cor_x()>right_x)
            right_x=v_cterminal[x]->get_cor_x();
        if(v_cterminal[x]->get_cor_y()>top_y)
            top_y=v_cterminal[x]->get_cor_y();
        if(v_cterminal[x]->get_cor_y()<bottom_y)
            bottom_y=v_cterminal[x]->get_cor_y();
    }
    return right_x+top_y-left_x-bottom_y;
}
void c_bs::print_contour()
{
    list<int>::iterator itx;
    list<int>::iterator ity;
    for(itx=contour_x->begin(),ity=contour_y->begin();itx!=contour_x->end();++itx,++ity)
    {
        printf("(%d,%d) ",*itx,*ity);
    }
    cout<<endl;
}
void c_block::reg_info()
{
    reg_parent.push_back(parent);
    reg_left.push_back(left);
    reg_right.push_back(right);
}
void c_block::reundo()
{
    set_parent(reg_parent.back());
    set_left(reg_left.back());
    set_right(reg_right.back());
}
void c_bs::print_svg()
{
    ofstream fout;
    fout.open("place.svg");

    double xx=outline_x/10;
    double yy=outline_y/10;
    fout<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>"<<endl;
    fout<<"<svg xmlns=\"http://www.w3.org/2000/svg\">"<<endl;
    fout<<"<rect x=\"0\" y=\"0\" width=\"";
    fout<<xx<<"\" height=\"";
    fout<<yy<<"\" stroke=\"blue\" stroke-width=\"3\" fill-opacity=\"0\"/>\n";
    for(unsigned int x=0;x<v_cblock.size();x++)
    {
        double corx=v_cblock[x]->get_cor_x()/10;
        double cory=v_cblock[x]->get_cor_y()/10;
        double w=v_cblock[x]->get_width()/10;
        double l=v_cblock[x]->get_lenth()/10;
        fout<<"<rect x=\"";
        fout<<corx<<"\" y=\"";
        fout<<cory<<"\" width=\"";
        fout<<w<<"\" height=\"";
        fout<<l<<"\" stroke=\"red\" stroke-width=\"3\"/>\n";
    }
    fout<<"</svg>";
    fout.close();
}
void c_bs::print_rpt(double timing)
{
    ofstream fout2;
    fout2.open(output_file);
    int HPWL=0;
    int area;
    list<int>::iterator itx;
    list<int>::iterator ity;
    int wid=0;
    int len=0;
    for(itx=contour_x->begin(),ity=contour_y->begin();itx!=contour_x->end();++itx,++ity)
    {
        if(*itx==MAX)
            break;
        if(*itx>wid)
            wid=*itx;
        if(*ity>len)
            len=*ity;
    }
    //假設邊界*0.9 在outline裡且cost*0.9小於目前最低cost的話就紀錄
    for(unsigned int x=0;x<v_cnets.size();x++)
    {
        HPWL+=v_cnets[x]->caculate_HPWL();
    }
    area=wid*len;
    double cost_now=alpha*area+(1-alpha)*HPWL;
    fout2<<cost_now<<endl;
    fout2<<HPWL<<endl;
    fout2<<area<<endl;
    fout2<<wid<<" "<<len<<endl;
    fout2<<timing<<endl;
    for(unsigned int x=0;x<v_cblock.size();x++)
    {
        c_block* tmp=v_cblock[x];
        fout2<<tmp->get_name()<<"    "<<tmp->get_cor_x()<<"    "<<tmp->get_cor_y()<<"    "<<tmp->get_cor_x()+tmp->get_width()<<"    "<<tmp->get_cor_y()+tmp->get_lenth()<<endl;
    }

    fout2.close();
}
