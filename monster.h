#ifndef JNP4_MONSTER_H
#define JNP4_MONSTER_H

#include <type_traits>
#include <iostream>
#include "citizen.h"

namespace monsterNames {
    char *vampire[] = "Vampire";
    char *mummy[] = "Mummy";
    char *zombie[] = "Zombie";
}

template<typename T, char const *name>
class Monster {
    static_assert(std::is_arithmetic<T>::value, "Monster requires arithmetic type");

private:
    T _health, _attackPower;

public:
    using valueType = T;

    Monster(T health, T attackPower) : _health(health), _attackPower(attackPower) {
        assert(health > 0);
        assert(attackPower >= 0);
    }

    T getHealth() {
        return _health;
    }

    T getAttackPower() {
        return _attackPower;
    }

    T getMonsterName() {
        std::string s(name);
        return s;
    }

    void takeDamage(T damage) {
        _health = _health < damage ? 0 : _health - damage;
    }
};

template<typename T>
using Zombie = Monster<T, monsterNames::zombie>;

template<typename T>
using Vampire = Monster<T, monsterNames::vampire>;

template<typename T>
using Mummy = Monster<T, monsterNames::mummy>;

template<typename M, typename T>
void attack(M &monster, Sheriff<T> &victim) {
    std::cout << "Sheriff fight" << std::endl;
    monster.takeDamage(victim.getAttackPower());
    victim.takeDamage(monster.getAttackPower());
}

template<typename M, typename U>
void attack(M &monster, U &victim) {
    std::cout << "Citizen fight" << std::endl;
    victim.takeDamage(monster.getAttackPower());
}


#endif //JNP4_MONSTER_H