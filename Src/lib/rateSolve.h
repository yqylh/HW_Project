#ifndef __rateSolve__
#define __rateSolve__
struct rateSolve{
    int id;
    double rate;
    std::vector<int> virSerId;
    rateSolve(){
        id = 0;
        rate = 0;
        virSerId.clear();
    }
    ~rateSolve() {}
    rateSolve(int id, double rate) : id(id), rate(rate) {
        virSerId.clear();
    }
    inline friend bool operator<(const rateSolve &A, const rateSolve &B){
        return A.rate < B.rate;
    }
};
#endif