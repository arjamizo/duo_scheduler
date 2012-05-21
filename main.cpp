#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <iterator>

using namespace std;
typedef vector<bool> X;
typedef vector<X> X2;
typedef vector<X2> X3;

string Xstr(X x) {
    stringstream ss;
    for(size_t i=0; i<x.size(); ++i) {
        ss<<setw(3)<<(x[i]?"T":"-")<<" ";
    }
    return ss.str();
}

string X2str(X2 x2) {
    stringstream ss;
    for(size_t i=0; i<x2.size(); ++i) {
        ss<<Xstr(x2[i])<<endl;
    }
    return ss.str();
}


class DuoScheduler {
    public:
    X3 x;
    long cmax;
    static const int m=2;
    vector<int> jobs;
    void addJob(int j) {
        jobs.push_back(j);
    }
    int getC() {
        int mean=((float)std::accumulate(jobs.begin(), jobs.end(), 0.0) / m)+0.5;
        //cout<<"in c\n";
        int max=*std::max_element(jobs.begin(), jobs.end());
        int c=std::max(mean, max); /*because we count from 0 and need values to be till max(mean, max)*/
        std::cout<<"c="<<c<<endl;
        return c;
    }
    DuoScheduler(vector<int> js):jobs(js) {
        init();
    }
    operator std::string() {
        stringstream ss;
        for(int j=0; j<x.size(); ++j) {
            ss<<j<<" job table, cost="<<((j>0)?jobs[j-1]:-1)<<"\n";
            ss<<X2str(x[j]);
            ss<<endl;
        }
        return ss.str();
    }
    void init() {
        //std::cout<<"construct\n";
        int size=getC();
        //std::cout<<size<<endl;
        X x=X(size+1, false);
        //std::cout<<"x done\n";
        X2 x2=X2(size+1, x);
        //std::cout<<"x2 done\n";
        this->x=X3(jobs.size()+1, x2); /*+1 because zero job frame j=0*/
        cmax=size;
    }
    void solve() {
        init();
        x[0][0][0]=true; // [0] jobs are being done at moment [0][0]
        cout<<"solve start\n";
        for(int j=1; j<x.size(); ++j) {
            cout<<"job="<<j<<endl;
            cout<<(string)*this;
            for(int t1=0; t1<x[j].size(); ++t1)
                for(int t2=0; t2<=x[j][t1].size(); ++t2) {
                    bool exists_qm=false; //exists question mark
                    for(int i=0; i<2; ++i) {
                        //printf("j=%d t1=%d t2=%d\n",j,t1,t2);
                        int _t1=t1-((i==0)?jobs[j-1]:0);
                        int _t2=t2-((i==1)?jobs[j-1]:0);
                        if(_t1>=0 && _t1<=cmax && _t2>=0 && _t2<=cmax) {//domain for multidimentional x vector
                            //printf("j=%d _t1=%d _t2=%d ", j, _t1, _t2);
                            //printf("[]=%d\n", (int)x[j-1][_t1][_t2]);
                            if(x[j-1][_t1][_t2])
                                x[j][t1][t2]=true;
                        }
                    }
                }
        }
    }
    vector<vector<int > > backtrack() {
        //X3 x=X3(1, X2(cmax+1,X(cmax+1,false)));
        //x[0][3][4]=true;

        //finding the time in multidimentional vector where last task ended.
        vector<vector<int > > proc=vector<vector<int> >(m, vector<int>());
        int T1=0;
        int T2=0;
        for(int t1=cmax; t1>=0; --t1)
        for(int t2=cmax; t2>=0; --t2)
        {
            //printf("%d,%d = %d\n", t1,t2, (int)x.back().at(t1)[t2]);
            if(x.back().at(t1)[t2])
            {
                T1=t1;
                T2=t2;
                std::cout<<"fount at ("<<T1<<","<<T2<<")\n";
            }

        }
        int j=jobs.size();
        int t1=T1;
        int t2=T2;
        while(j>0)
        {

            cout<<"job="<<j;
            printf(" t1=%d t2=%d\n", t1, t2);
            j--;
            if(t2-jobs[j]>=0) {
                printf("x[%d][%d][%d%d]=%d\n", j, t1, t2,-jobs[j], (int)x[j][t1][t2-jobs[j]]);
                t2-=jobs[j];
                proc[1].push_back(jobs[j]);
            } else if(t1-jobs[j]>=0) {
                printf("x[%d][%d%d][%d]=%d\n", j, t1, -jobs[j], t2, (int)x[j][t1-jobs[j]][t2]);
                t1-=jobs[j];
                proc[0].push_back(jobs[j]);
            }
        }
        return proc;
    }
};

int main()
{
    int j[]={1,2,3,4,5,6};
    vector<int> jobs(j,j+6);
    delete []j;
    DuoScheduler duo(jobs);
    duo.solve();
    cout<<(string)duo;
    vector<vector<int> >proc=duo.backtrack();
    cout<<"M1\n";
    std::copy(proc[0].begin(), proc[0].end(), std::ostream_iterator<int> (std::cout, " "));
    std::cout<<endl<<"M2\n";
    std::copy(proc[1].begin(), proc[1].end(), std::ostream_iterator<int> (std::cout, " "));
    return 0;
}
