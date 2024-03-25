#include"fp.h"

void c_bs::update_contour(c_block* p_place)
{
    list<int>::iterator itx;
    list<int>::iterator ity;
    for(itx=p_place->con_x.begin(),ity=p_place->con_y.begin();itx!=p_place->con_x.end();++itx,++ity)//insert the new contour and delete the unnessery contour
    {
        if(p_place->get_cor_x()>=*itx)
            continue;
        else
        {
            p_place->con_x.insert(itx,p_place->get_cor_x());
            p_place->con_y.insert(ity,p_place->get_cor_y());
            p_place->con_x.insert(itx,p_place->get_cor_x());
            p_place->con_y.insert(ity,p_place->get_cor_y()+p_place->get_lenth());
            p_place->con_x.insert(itx,p_place->get_cor_x()+p_place->get_width());
            p_place->con_y.insert(ity,p_place->get_cor_y()+p_place->get_lenth());
            p_place->con_x.insert(itx,p_place->get_cor_x()+p_place->get_width());
            p_place->con_y.insert(ity,p_place->get_cor_y());
            list<int>::iterator un_itx=itx;
            list<int>::iterator un_ity=ity;
            un_itx++;
            un_ity++;
            if(un_itx==p_place->con_x.end())
                break;
            if(*itx<p_place->get_cor_x()+p_place->get_width())
            {
                while(p_place->get_cor_x()+p_place->get_width()>*un_itx)
                {
                    un_itx++;
                    un_ity++;
                }
                p_place->con_x.erase(itx,un_itx);
                p_place->con_y.erase(ity,un_ity);
            }
            break;
        }
    }
    for(itx=p_place->con_x.begin(),ity=p_place->con_y.begin();itx!=p_place->con_x.end();++itx,++ity)//delete redundent contour
    {
        list<int>::iterator tmp_itx=itx;
        list<int>::iterator tmp_ity=ity;
        tmp_itx++;
        tmp_ity++;
        if(tmp_itx==p_place->con_x.end())
            break;
        list<int>::iterator tmp_itx_start=tmp_itx;
        list<int>::iterator tmp_ity_start=tmp_ity;
        if(*itx==*tmp_itx)
        {
            while(*(tmp_itx)==*itx)
            {
                if(tmp_itx==p_place->con_x.end())
                    break;
                tmp_itx++;
                tmp_ity++;
            }
            tmp_itx--;
            tmp_ity--;
            if(tmp_itx_start==tmp_itx)
                continue;
            else
            {
                p_place->con_x.erase(tmp_itx_start,tmp_itx);
                p_place->con_y.erase(tmp_ity_start,tmp_ity);
            }
        }
    }
    for(itx=p_place->con_x.begin(),ity=p_place->con_y.begin();itx!=p_place->con_x.end();++itx,++ity)//delete redundent contour
    {
        list<int>::iterator tmp_itx=itx;
        list<int>::iterator tmp_ity=ity;
        tmp_itx++;
        tmp_ity++;
        if(tmp_itx==p_place->con_x.end())
            break;
        list<int>::iterator tmp_itx_start=tmp_itx;
        list<int>::iterator tmp_ity_start=tmp_ity;
        if(*ity==*tmp_ity)
        {
            while(*tmp_ity==*ity)
            {
                if(tmp_itx==p_place->con_x.end())
                    break;
                tmp_itx++;
                tmp_ity++;
            }
            tmp_itx--;
            tmp_ity--;
            if(tmp_ity_start==tmp_ity)
                continue;
            else
            {
                p_place->con_x.erase(tmp_itx_start,tmp_itx);
                p_place->con_y.erase(tmp_ity_start,tmp_ity);
            }
        }
    }
    ity=p_place->con_y.end();
    ity--;
    ity--;
    if(*ity!=0)
        *ity=0;

    contour_x=&(p_place->con_x);
    contour_y=&(p_place->con_y);
    //cout<<"after place:"<<p_place->get_name()<<" "; 
    p_place->set_update(false);
    //print_contour();
}
void c_bs::place_right(c_block* from, c_block* to) //place "to" to from's right
{
    int start_x=from->get_cor_x()+from->get_width();
    int end_x=start_x+to->get_width();
    int start_y=0;
    to->con_x= *contour_x;
    to->con_y= *contour_y;
    list<int>::iterator itx;
    list<int>::iterator ity;
    for(itx=to->con_x.begin(),ity=to->con_y.begin();itx!=to->con_x.end();++itx,++ity)//決定to起始的x與y
    {
        if(start_x>*itx)
            continue;
        if(end_x<=*itx)
        {
            list<int>::iterator range_itx=itx;
            range_itx--;
            if(*range_itx<=start_x){
                start_y=*ity;
            }
            break;    
        }
        else if(start_x==*itx)
            start_y=*ity;
        else
        {
            if(start_y<*ity)
                start_y=*ity;
        }
    }
    to->set_cor_x(start_x);
    to->set_cor_y(start_y);
    update_contour(to);
}
void c_bs::place_top(c_block* from, c_block* to) //place "to" to from's top
{
    int start_x=from->get_cor_x();
    int end_x=start_x+to->get_width();
    int start_y=from->get_cor_y()+from->get_lenth();
    to->con_x= *contour_x;
    to->con_y= *contour_y;
    list<int>::iterator itx;
    list<int>::iterator ity;
    for(itx=to->con_x.begin(),ity=to->con_y.begin();itx!=to->con_x.end();++itx,++ity)//決定to起始的x和y
    {
        if(start_x>*itx)
            continue;
        if(end_x<=*itx)
        {
            list<int>::iterator range_itx=itx;
            range_itx--;
            if(*range_itx<=start_x){
                start_y=*ity;
            }
            break;    
        }
        else if(start_x==*itx)
            start_y=*ity;
        else
        {
            if(start_y<*ity)
                start_y=*ity;
        }
    }
    to->set_cor_x(start_x);
    to->set_cor_y(start_y);
    update_contour(to);
}
