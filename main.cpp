#include"parser.h"
#include"fp.h"
int main(int argc,char* argv[])
{
    srand((unsigned)time(NULL));
    clock_t start,end;
    start = clock();

    parser* par;
    par = new parser(argv[2],argv[3]);
    //par->dump();
    c_bs*   bstree;
    bstree = new c_bs(par,argv[1],argv[4]);
    //bstree->dump();
    bstree->build_bstar();
    bstree->sa_first();
    bstree->print_svg();
    end = clock();
    double timing=(double)(end-start)/CLOCKS_PER_SEC;
    bstree->print_rpt(timing);
    return 1;
}
