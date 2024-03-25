#include"parser.h"
#include<string.h>
#include<fstream>
block_s::block_s(char p_name[],int p_x,int p_y,bool ty)
{
    strcpy(m_name,p_name);
    m_nx=p_x;
    m_ny=p_y;
    b_t =ty;
}
parser::parser(const char* p_block,const char* p_nets)
{
    ifstream fin;
    ifstream fin2;
    fin.open(p_block);
    char buffer[256];
    fin.getline(buffer,256);
    sscanf(buffer,"Outline: %d %d",&max_x,&max_y);
    fin.getline(buffer,256);
    sscanf(buffer,"NumBlocks:%d",&n_blocks);
    fin.getline(buffer,256);
    sscanf(buffer,"%*s%d",&n_terminals);
    //printf("x:%d y=%d #block=%d #terminals=%d\n",max_x,max_y,n_blocks,n_terminals);
    while(1)
    {
        fin.getline(buffer,256);
        if(buffer[0]!='\r'&&buffer[0]!='\0')
            break;
    }
    for(int x=0;x<n_blocks;x++)
    {
        char block_name[256];
        int  block_x;
        int  block_y;
        sscanf(buffer,"%s %d %d",block_name,&block_x,&block_y);
        block_s* temp;
        temp=new block_s(block_name,block_x,block_y,0);
        m_sblock.push_back(temp);
        if(n_blocks!=x+1)
        {
            fin.getline(buffer,256);
        }
    }
    while(1)
    {
        fin.getline(buffer,256);
        if(buffer[0]!='\r'&&buffer[0]!='\0')
            break;
    }
    for(int x=0;x<n_terminals;x++)
    {
        char block_name[256];
        int  block_x;
        int  block_y;
        sscanf(buffer,"%s terminal %d %d",block_name,&block_x,&block_y);
        block_s* temp;
        temp = new block_s(block_name,block_x,block_y,1);
        m_sblock.push_back(temp);
        if(n_blocks!=x+1)
        {
            fin.getline(buffer,256);
        }
    }
    fin.close();
    fin2.open(p_nets);
    fin2.getline(buffer,256);
    sscanf(buffer,"NumNets: %d",&n_nets);
    for(int x=0;x<n_nets;x++)
    {
        fin2.getline(buffer,256);
        int node;
        sscanf(buffer,"NetDegree: %d",&node);
        net_s* r;
        r= new net_s(x);
        for(int y=0;y<node;y++)
        {
            fin2.getline(buffer,256);
            //cout<<buffer<<endl;
            char* tmp;
            tmp=new char();
            char bu[256];
            sscanf(buffer,"%[^\r]",bu);
            strcpy(tmp,bu);
            r->vblock.push_back(tmp);
        }
        m_snet.push_back(r);
    }
    fin2.close();
}



void parser::dump()
{
    block_s* r;
    for(unsigned int x=0;x<m_sblock.size();x++)
    {
        r=m_sblock[x];
        printf("%s %d %d %d\n",r->m_name,r->m_nx,r->m_ny,r->b_t);
    }
    for(unsigned int x=0;x<m_snet.size();x++)
    {
        for(unsigned int y=0;y< m_snet[x]->vblock.size();y++)
        {
            printf("%d %s\n",m_snet[x]->name,m_snet[x]->vblock[y]);
        }
    }
}

