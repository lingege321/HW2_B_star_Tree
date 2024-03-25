#include"fp.h"
#include"parser.h"
c_bs::c_bs(parser* p , const char* alp, const char* out)
{
    outline_x=p->max_x;
    outline_y=p->max_y;
    c_block* b;
    c_terminal* t;
    for(unsigned int x=0;x<p->m_sblock.size();x++)
    {
        if(!p->m_sblock[x]->b_t) //block
        {
            b=new c_block(p->m_sblock[x]->m_name,p->m_sblock[x]->m_nx,p->m_sblock[x]->m_ny);
            v_cblock.push_back(b);
        }
        else
        {
            t=new c_terminal(p->m_sblock[x]->m_name,p->m_sblock[x]->m_nx,p->m_sblock[x]->m_ny);
            v_cterminal.push_back(t);
        }
    }
    c_net* c;
    for(unsigned int x=0;x<p->m_snet.size();x++)
    {
        c=new c_net(p->m_snet[x]->name);
        int zero=1;
        for(unsigned int y=0;y<p->m_snet[x]->vblock.size();y++)
        {   
            for(unsigned int z=0;z<v_cblock.size();z++)
            {
                zero=strcmp(v_cblock[z]->get_name().c_str(),p->m_snet[x]->vblock[y]);
                if(!zero)
                {
                    c->push_block(v_cblock[z]);
                }
            } 
            for(unsigned int z=0;z<v_cterminal.size();z++)
            {
                zero=strcmp(v_cterminal[z]->get_name().c_str(),p->m_snet[x]->vblock[y]);
                if(!zero)
                {
                    c->push_terminal(v_cterminal[z]);
                }
            }
        }
        v_cnets.push_back(c);
    }
    contour_x=new list<int>(1,0);
    contour_y=new list<int>(1,0);
    contour_x->push_back(MAX);
    contour_y->push_back(0);
    char rootname[]="root";
    root=new c_block(rootname,0,0);
    root->con_x.push_back(0);
    root->con_y.push_back(0);
    root->con_x.push_back(MAX);
    root->con_y.push_back(0);
    int zero=0;
    root->set_cor_x(zero);
    root->set_cor_y(zero);
    root->set_update(false);
    alpha=strtod(alp,NULL);
    lowest_cost=MAX;
    output_file=out;
    decrease_slope=0.5;
}
void c_bs::dump()
{
       for(unsigned int x=0;x<v_cblock.size();x++)
       {
           printf("Name: %5s Width: %4d Length: %4d\n",v_cblock[x]->get_name().c_str(),v_cblock[x]->get_width(),v_cblock[x]->get_lenth());
       } 
       for(unsigned int x=0;x<v_cterminal.size();x++)
       {
           printf("Name: %5s     X: %4d      Y: %4d\n",v_cterminal[x]->get_name().c_str(),v_cterminal[x]->get_cor_x(),v_cterminal[x]->get_cor_y());
       } 
       for(unsigned int x=0;x<v_cnets.size();x++)
       {
           vector<c_block*> b;
           vector<c_terminal*> t;
           b=v_cnets[x]->get_vblock();
           t=v_cnets[x]->get_vterminal();
           printf("Name: %d block:%d terminal:%d total:%d\n",v_cnets[x]->get_name(),b.size(),t.size(),b.size()+t.size());
       }
}
bool c_bs::swap(c_block* p,c_block* q) //swap p and q
{
    //dump_tree(root->get_left());
    if(p==q)
        return false;
    p->set_update(true);
    q->set_update(true);
    if(p->get_parent()==q->get_parent())
    {
        c_block* tmp_=p->get_parent()->get_right();
        p->get_parent()->set_right(p->get_parent()->get_left());
        p->get_parent()->set_left(tmp_);
        
    }
    else if(p->get_parent()==q)
    {
        //cout<<"b"<<endl;
        if(q->get_parent()->get_right()==q)
            q->get_parent()->set_right(p);
        else
            q->get_parent()->set_left(p);
        p->set_parent(q->get_parent());
        q->set_parent(p);
        c_block* p_left=p->get_left();
        c_block* p_right=p->get_right();
        if(q->get_left()==p)
        {
            p->set_left(q);
            p->set_right(q->get_right());
            if(p->get_right()!=NULL)
                p->get_right()->set_parent(p);
            q->set_left(p_left);
            q->set_right(p_right);
            if(p_left!=NULL)
                p_left->set_parent(q);
            if(p_right!=NULL)
                p_right->set_parent(q);
        }
        else
        {
            p->set_right(q);
            p->set_left(q->get_left());
            if(p->get_left()!=NULL)
                p->get_left()->set_parent(p);
            q->set_left(p_left);
            q->set_right(p_right);
            if(p_left!=NULL)
                p_left->set_parent(q);
            if(p_right!=NULL)
                p_right->set_parent(q);
        }
    }
    else if(q->get_parent()==p)
    {
        //cout<<"a"<<endl;
        if(p->get_parent()->get_right()==p)
            p->get_parent()->set_right(q);
        else
            p->get_parent()->set_left(q);
        q->set_parent(p->get_parent());
        p->set_parent(q);
        c_block* q_left=q->get_left();
        c_block* q_right=q->get_right();
        if(p->get_left()==q)
        {
            q->set_left(p);
            q->set_right(p->get_right());
            if(q->get_right()!=NULL)
                q->get_right()->set_parent(q);
            p->set_left(q_left);
            p->set_right(q_right);
            if(q_left!=NULL)
                q_left->set_parent(p);
            if(q_right!=NULL)
                q_right->set_parent(p);
        }
        else
        {
            q->set_right(p);
            q->set_left(p->get_left());
            if(q->get_left()!=NULL)
                q->get_left()->set_parent(q);
            p->set_left(q_left);
            p->set_right(q_right);
            if(q_left!=NULL)
                q_left->set_parent(p);
            if(q_right!=NULL)
                q_right->set_parent(p);
        }
    }
    else
    {
        c_block* p_parent=p->get_parent();
        c_block* p_left=p->get_left();
        c_block* p_right=p->get_right();
        if(p_parent->get_left()==p)
            p_parent->set_left(q);
        else
            p_parent->set_right(q);
        p->set_parent(q->get_parent());
        if(q->get_parent()->get_left()==q)
            q->get_parent()->set_left(p);
        else
            q->get_parent()->set_right(p);
        q->set_parent(p_parent);
        p->set_left(q->get_left());
        p->set_right(q->get_right());
        if(p->get_left()!=NULL)
            p->get_left()->set_parent(p);
        if(p->get_right()!=NULL)
            p->get_right()->set_parent(p);
        q->set_left(p_left);
        q->set_right(p_right);
        if(q->get_left()!=NULL)
            q->get_left()->set_parent(q);
        if(q->get_right()!=NULL)
            q->get_right()->set_parent(q);
    }
    /*
    for(unsigned int x=0;x<v_cblock.size();x++)
    {
        printf("(%s,%s)",v_cblock[x]->get_name().c_str(),v_cblock[x]->get_parent()->get_name().c_str());
    }
    */
    return true;
}
bool c_bs::move(c_block* p, c_block* q, int choose)
{
    //dump_tree(root->get_left());
    if(p==q)
        return false;
    if(p->get_left()!=NULL && p->get_right()!=NULL)
        return false;
    p->set_update(true);
    //choose (0,1,2)=(left,parent,right) if choose==0 put p to q's left child
    //p must have only one child or zero child
    if(p->get_left()!=NULL)
    {
        p->get_left()->set_update(true);
        if(p->get_parent()->get_left()==p)
            p->get_parent()->set_left(p->get_left());
        else
            p->get_parent()->set_right(p->get_left());
        p->get_left()->set_parent(p->get_parent());
        p->set_left(NULL);
    }
    else if(p->get_right()!=NULL)
    {
        p->get_right()->set_update(true);
        if(p->get_parent()->get_left()==p)
            p->get_parent()->set_left(p->get_right());
        else
            p->get_parent()->set_right(p->get_right());
        p->get_right()->set_parent(p->get_parent());
        p->set_right(NULL);
    }
    else
    {
        //two child are both NULL
        p->get_parent()->set_update(true);
        if(p->get_parent()->get_left()==p)
            p->get_parent()->set_left(NULL);
        else
            p->get_parent()->set_right(NULL);
    }
    if(choose==0)
    {
        p->set_parent(q);
        p->set_left(q->get_left());
        q->set_left(p);
        if(p->get_left()!=NULL)
            p->get_left()->set_parent(p);
    }
    else if(choose==1)
    {
        if(q->get_parent()==root)
        {
            //compare p's width and length. If length is longer than width, we choose q is p's left child
            root->set_left(p);
            p->set_parent(root);
            if(p->get_width()>p->get_lenth())
                p->set_right(q);
            else
                p->set_left(q);
            q->set_parent(p);
        }
        else
        {
            if(q->get_parent()->get_left()==q)
            {
                q->get_parent()->set_left(p);
                p->set_parent(q->get_parent());
                p->set_left(q);
                q->set_parent(p);
            }
            else
            {
                q->get_parent()->set_right(p);
                p->set_parent(q->get_parent());
                p->set_right(q);
                q->set_parent(p);
            }
        }
    }
    else
    {
        p->set_parent(q);
        p->set_right(q->get_right());
        q->set_right(p);
        if(p->get_right()!=NULL)
            p->get_right()->set_parent(p);
    }
    return true;
}
void c_bs::dump_tree(c_block* p)
{
    if(p!=NULL)
    {
        printf("%s ",p->get_name().c_str());
        dump_tree(p->get_left());
        dump_tree(p->get_right());
    }
}
