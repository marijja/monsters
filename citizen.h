#ifndef SMALLTOWN_CITIZEN_H
#define SMALLTOWN_CITIZEN_H

#include <type_traits>
#include <cassert>
#include <iostream>

template <typename T, int AD, int AU, bool canAttack>
class Citizen {
    static_assert(std::is_arithmetic<T>::value, "Citizen requires arithmetic type");
private:
    T _health;
    T _age;
    T _attackPower;
public:
    template<bool B = !canAttack, typename std::enable_if<B, int>::type = 0>
    Citizen(T health, T age) : _health(health), _age(age) {
        std::cout << "Adult or teenager constructor" << std::endl;
        assert(age >= AD && age <= AU);
        assert(health > 0);
    }

    template<bool B = canAttack, typename std::enable_if<B, int>::type = 0>
    Citizen(T health, T age, T attackPower) : _health(health), _age(age), _attackPower(attackPower) {
        std::cout << "Sheriff constructor" << std::endl;
        assert(age >= AD && age <= AU);
        assert(attackPower >= 0);
        assert(health > 0);
    }

    T getHealth() const { return _health; }

    T getAge() const { return _age; }

    /*
     * Returns attack power points if a citizen can attack, otherwise throws compilation error
     */
    template<bool B = canAttack, typename std::enable_if<B, int>::type = 0>
    T getAttackPower() const { return _attackPower; }

    /*
     * Change of health points as a result of attack by monster
     */
    void takeDamage(T damage) {
        std::cout << "f_takeDamage" << std::endl;
        _health -= (damage >= _health ? _health : damage);
    }
};

template <typename T>
using Adult = Citizen<T, 18, 100, false>;

template <typename T>
using Teenager = Citizen<T, 11, 17, false>;

template <typename T>
using Sheriff = Citizen<T, 18, 100, true>;

#endif //SMALLTOWN_CITIZEN_H