#pragma once
#include <typeinfo>

class Serializable
{
public:
    template<typename T, typename K>
    inline bool isType(const K &k) {
        return typeid(T).hash_code() == typeid(k).hash_code();
    }
    Serializable();
    ~Serializable();

};

