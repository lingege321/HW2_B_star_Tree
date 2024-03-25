#include"fp.h"
#include<stack>
#include<math.h>
void c_bs::build_bstar()
{
    double notuse=0;
    //choose the biggest block and hope to put it in the corner
    int biggest_length=0;
    int root_block=0;
    int nousew,nousel;
    for(int x=0;x<v_cblock.size();x++)
    {
        if(v_cblock[x]->get_width()+v_cblock[x]->get_lenth()>biggest_length)
        {
            root_block=x;
            biggest_length=v_cblock[x]->get_width()+v_cblock[x]->get_lenth();
        }
    }
    if(root_block!=0)
    {
        c_block* swap_tmp=v_cblock[0];
        v_cblock[0]=v_cblock[root_block];
        v_cblock[root_block]=swap_tmp;
    }
    //change finish
    root->set_left(v_cblock[0]);
    v_cblock[0]->set_parent(root);
    v_cblock[0]->set_update(true);
    for(int x=1;x<v_cblock.size();x++)
    {
        if(x%2==1)
        {
           v_cblock[(x-1)/2]->set_left(v_cblock[x]); 
           v_cblock[x]->set_parent(v_cblock[(x-1)/2]);
        }
        else
        {
           v_cblock[(x-2)/2]->set_right(v_cblock[x]); 
           v_cblock[x]->set_parent(v_cblock[(x-1)/2]);
        }
    }
    
    //dump_tree(root->get_left());
    //cout<<endl;
    update_afterop();
    computer_cost(notuse,nousew,nousel);
    //cout<<lowest_cost<<endl;
   
    initial_temperature=lowest_cost;
    now_temperature=initial_temperature;
    for(unsigned int x=0;x<v_cblock.size();x++)
        v_cblock[x]->reg_info();
    cost.push_back(lowest_cost); 
    //cout<<"now_tem: "<<now_temperature<<endl;
    
    /*
    v_cblock[0]->set_left(v_cblock[3]);
    v_cblock[3]->set_parent(v_cblock[0]);
    v_cblock[0]->set_right(v_cblock[2]);
    v_cblock[2]->set_parent(v_cblock[0]);
    v_cblock[2]->set_left(v_cblock[4]);
    v_cblock[4]->set_parent(v_cblock[2]);
    v_cblock[4]->set_left(NULL);
    v_cblock[4]->set_right(NULL);
    v_cblock[2]->set_right(NULL);
    v_cblock[3]->set_left(NULL);
    v_cblock[3]->set_right(NULL);

    dump_tree(root->get_left());
    cout<<endl;
    update_afterop();
    cout<<"\nfinal ";
    print_contour();
    */
}
void c_bs::update_afterop()
{
    //use dfs
    stack<c_block*> st_block;  
    bool upd=false;             //initial upd=false
    st_block.push(root);
    while(!st_block.empty())
    {
        c_block* tmp;
        tmp=st_block.top();
        st_block.pop();
        upd=upd|(tmp->is_update());
        if(tmp->get_right()!=NULL)
            st_block.push(tmp->get_right());
        if(tmp->get_left()!=NULL)
            st_block.push(tmp->get_left());
        if(upd)
        {
            //dump_tree(root->get_left());
            //cout<<endl;
            //cout<<tmp->get_name()<<endl;
            c_block* tmp_parent=tmp->get_parent();
            if(tmp==tmp_parent->get_left())
                place_right(tmp_parent,tmp);
            else
                place_top(tmp_parent,tmp);
        }
        contour_x=&(tmp->con_x);
        contour_y=&(tmp->con_y);
    }
}
bool c_bs::decide_operation()
{
    //I hope it do move operation at high temperator and do swap at low temperator
    int now_t=rand()%10000;
    if(now_t<5000)//at high temperature decide_factor is easier to be less than tmperature.  So do move
    {
        int abc=v_cblock.size();
        int b_from=rand()%abc;    
        int b_to=rand()%abc;    
        int b_where=rand()%3;
        if(move(v_cblock[b_from],v_cblock[b_to],b_where))
        {
            //cout<<"//-----------------------------------------------------------------------//move "<<b_from<<" "<<b_to<<" "<<b_where<<endl;
            update_afterop();
            return true;
        }
    }
    else
    {
        int b_from=rand()%v_cblock.size();    
        int b_to=rand()%v_cblock.size();    
        if(swap(v_cblock[b_from],v_cblock[b_to]))
        {
            //cout<<"//-----------------------------------------------------------------------//swap"<<b_from<<" "<<b_to<<endl;
            update_afterop();
            return true;
        }
    }
    return false;
}
bool c_bs::computer_cost(double& dif_cost, int& wid, int& len)
{
    int HPWL=0;
    int area;
    list<int>::iterator itx;
    list<int>::iterator ity;
    wid=0;
    len=0;
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
    //cout<<"area: "<<area<<" HPWL"<<HPWL<<endl;
    double cost_now=alpha*area+(1-alpha)*HPWL;
    //cout<<"cost1 "<<cost_now<<endl;
    double temp;
    bool overlape=false;
    if(wid>outline_x)
    {
        temp=(wid-outline_x)*outline_x*100;
        if(cost_now+temp<cost_now)
            overlape=true;
        else
            cost_now+=temp;
    }
    //cout<<"cost2 "<<cost_now<<endl;
    if(overlape==false && len>outline_y)
    {
        temp=(len-outline_y)*outline_y*100;
        if(cost_now+temp<cost_now)
            overlape=true;
        else
            cost_now+=temp;
    }
    //cout<<"cost3 "<<cost_now<<endl;
    //cout<<"low cost: "<<lowest_cost<<endl;
    if(overlape==false)
    {
        double dif_cost=cost_now-lowest_cost;
        double prob=get_prob(dif_cost);
        int n_prob=prob*10000;
        int hit=rand()%10000;
        if(hit<n_prob)      //can document this answer
        {
            for(unsigned int x=0;x<v_cblock.size();x++)
                v_cblock[x]->reg_info();
            lowest_cost=cost_now;
            cost.push_back(lowest_cost);
            return true;
        }
        for(unsigned int x=0;x<v_cblock.size();x++)
        {
            v_cblock[x]->reundo();
            if(v_cblock[x]->get_parent()==root)
            {
                root->set_left(v_cblock[x]);
                v_cblock[x]->set_update(true);
            }
        }
        return false;
    }
    return false;
}
void c_bs::sa_first()
{
    double dif_cost=0;
    double prob=1.0;
    int int_prob=prob*10000;
    int hit=0;
    int wid=MAX,len=MAX;
    list<int>::iterator itx;
    list<int>::iterator ity;
    int iter=1;
    int k=1000;
    while(wid>outline_x||len>outline_y)
    {
       // printf("wid:%d len%d\n",wid,len);
        print_svg();
        if(decide_operation())
        {
            if(computer_cost(dif_cost,wid,len))
            {
                now_temperature=initial_temperature;
                now_temperature/=(iter++);
                now_temperature/=k;
            }
        }
        if(iter==k)
            k=1;
        if(iter==5000)
        {
            k=1000;
            iter=1;
        }
        //if(now_temperature<1)
        //    now_temperature=initial_temperature/2;
        /*
        prob=get_prob(dif_cost,now_temperature);
        if(prob>1)
            prob=1.0;
        now_temperature*=decrease_slope;
        decrease_slope*=increase_tem;
        if(decrease_slope>=0.95)
            decrease_slope=0.85;
        int_prob=prob*10000;
        hit=rand()%10000;
        */
        //printf("outlinex:%d outliney:%d\n",outline_x,outline_y);
        //printf("wid:%d len%d\n",wid,len);
        //printf("iter:%d int_prob:%d now_temperature:%lf \n",iter,int_prob,now_temperature);
    }
}
void c_bs::sa()
{
    double increase_tem=1.01;
    double dif_cost=0;
    double prob=get_prob(dif_cost);
    if(prob>1)
        prob=1.0;
    prob*=10000;
    int hit;
    hit=rand()%10000;
    while(hit<prob && now_temperature>1)
    {
        decide_operation();
        if(computer_cost(dif_cost,hit,hit))
            break;
        /*
        prob=get_prob(dif_cost,now_temperature);
        if(prob>1)
            prob=1.0;
        cout<<"/--------------------------------------------"<<endl;
        cout<<"dif: "<<dif_cost<<" "<<now_temperature<<endl;
        now_temperature*=decrease_slope;
        cout<<"dif: "<<dif_cost<<" "<<now_temperature<<endl;
        decrease_slope*=increase_tem;
        if(decrease_slope>=0.95)
            decrease_slope=0.85;
        prob*=10000;
        hit=rand()%10000;
        cout<<"hit: "<<hit<<"prob: "<<prob<<endl;*/
    }
}
double c_bs::get_prob(double dif_cost)
{
    if(dif_cost>0)
        return exp(-dif_cost/now_temperature);
    else
        return 1;
}
