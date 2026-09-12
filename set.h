//
// Created by kirillr on 12.09.2026.
//

#ifndef CONVEX_HULL_SET_H
#define CONVEX_HULL_SET_H

template <typename T>
class set {
    protected:
    size_t _size;
    struct node {
        bool color; T value; node* l, *r, *p;
    };
    node* head;
};

#endif //CONVEX_HULL_SET_H
