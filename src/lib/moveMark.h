#ifndef __moveMark__
#define __moveMark__

struct moveMark
{
    int virSerId;
    int serId;
    int where;

    moveMark()
    {
        virSerId = 0;
        serId = 0;
        where = 0;
    }

    ~moveMark()
    {}

    moveMark(int virSerId, int serId, int where) : virSerId(virSerId), serId(serId), where(where)
    {}
};

#endif