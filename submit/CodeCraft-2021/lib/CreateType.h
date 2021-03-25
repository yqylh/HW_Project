#ifndef __CreateType__
#define __CreateType__
struct CreateType{
    int _serverId;
    int _type;
    int order;
    CreateType() {
        _serverId = 0;
        _type = 0;
        order = 0;
    }
    CreateType(int _serverId, int _type, int order) : _serverId(_serverId), _type(_type), order(order) {}
    inline bool friend operator< (const CreateType &A, const CreateType &B) {
        return A.order < B.order;
    }
};
#endif