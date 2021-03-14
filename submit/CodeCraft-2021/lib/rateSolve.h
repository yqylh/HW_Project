#ifndef __rateSolve__
#define __rateSolve__
struct rateSolve{
    int id;
    double rate;
    std::vector<int> virSerIds;
    rateSolve(){
        id = 0;
        rate = 0;
        virSerIds.clear();
    }
    ~rateSolve() {}
    rateSolve(int id, double rate) : id(id), rate(rate) {
        virSerIds.clear();
    }
    inline friend bool operator<(const rateSolve &A, const rateSolve &B){
        return A.rate < B.rate;
    }
};
#endif