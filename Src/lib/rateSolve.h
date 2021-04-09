#ifndef __rateSolve__
#define __rateSolve__
struct rateSolve{
    int id;
    double rate;
    int lorR;
    std::vector<int> virSerIds;
    rateSolve(){
        id = 0;
        rate = 0;
        lorR = -1;
        virSerIds.clear();
    }
    ~rateSolve() {}
    rateSolve(int id, double rate) : id(id), rate(rate) {
        lorR = -1;
        virSerIds.clear();
    }
    rateSolve(int id, double rate, bool lorR) : id(id), rate(rate) , lorR(lorR) {
        virSerIds.clear();
    }
    inline friend bool operator<(const rateSolve &A, const rateSolve &B){
        return A.rate < B.rate;
    }
    void operator=(const rateSolve &B)  {
        id = B.id;
        rate = B.rate;
        lorR = B.lorR;
        virSerIds.clear();
        for (auto & i : B.virSerIds) {
            virSerIds.push_back(i);
        }
    }
};
#endif