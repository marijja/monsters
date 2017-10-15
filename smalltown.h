#ifndef JNP4_SMALLTOWN_H
#define JNP4_SMALLTOWN_H

#include <cstddef>
#include <tuple>
#include <iostream>
#include "monster.h"
#include <typeinfo>
#include <vector>
#include <algorithm>


/*
 * Compile-time Fibonacci numbers calculation, not using recursion due to compile-time recursion depth limits
 */
namespace Fibonacci {
    template<typename U, std::size_t... Indices>
    constexpr auto make_array_helper(U* arr, std::index_sequence<Indices...>)
    -> std::array<U, sizeof...(Indices)> {
        return {{arr[Indices]...}};
    }

    template<int N, typename U>
    constexpr auto make_array(U* arr)
    -> std::array<U, N> {
        return make_array_helper(arr, std::make_index_sequence<N>{});
    }

    /*
    * Calculating the length of Fibonacci numbers table long enough that the last element is the last number < t1
    */
    template <typename U, U t1>
    static constexpr size_t length() {
        size_t i = 1;
        U prev = 1, next = 1, res = 0;

        while (prev < t1 && prev <= next) {
            res = prev + next;
            prev = next;
            next = res;
            i++;
        }
        return i;
    }

    /*
    * Calculating Fibonacci numbers table with length l
    */
    template<typename U, std::size_t length>
    constexpr std::array<U, length> fibonacciSequence() {
        U fibonacci[length] = {static_cast<U>(0)};
        fibonacci[0] = static_cast<U>(1);
        fibonacci[1] = static_cast<U>(1);

        for (std::size_t i = 2; i < length; ++i) {
            fibonacci[i] = fibonacci[i - 2] + fibonacci[i - 1];
        }
        return make_array<length, U>(fibonacci);
    }
};

/*
 * Class that describes the environment of monsters and citizens, function tick() determines actions of objects
 */
template <typename M, typename U, U t0, U t1, typename... C>
class SmallTown {
    static_assert(std::is_arithmetic<U>::value, "Smalltown requires arithmetic type");
    static_assert(t0 >= 0 && t1 >= 0, "Smalltown requires positive beginning time");
    static_assert(t0 <= t1, "Smalltown requires positive ending time");

private:
    const std::array<U, Fibonacci::length<U, t1>()> _attackTime
            = Fibonacci::fibonacciSequence<U, Fibonacci::length<U, t1>()>();

    M _monster;
    std::tuple<C...> _citizens;
    size_t _citizensAlive;
    bool _monsterAlive;
    U _currentTime;


    /*
     * Iterate over citizens tuple
     */
    template<std::size_t IT = 0, typename... Tup>
    typename std::enable_if<IT < sizeof...(Tup), void>::type
    makeAttack(std::tuple<Tup...>& t) {
        auto citizen = std::get<IT>(t);
        if(_monsterAlive) {
            attack(_monster, citizen);
            if(citizen.getHealth() <= 0) {
                _citizensAlive--;
            }
            if(_monster.getHealth() <= 0) {
                _monsterAlive = false;
            }
        }
        makeAttack<IT+1, Tup...>(t);
    }

    /*
     * Determine whether monster can attack citizens (if a number is in Fibonacci table)
     */
    bool canAttack(U value) {
        bool found = false;
        for (auto it = _attackTime.begin(); it != _attackTime.end(); it++) {
            if (*it == value) {
                std::cout << "Przeprowadza atak " << value << std::endl;
                found = true;
                break;
            }
        }
        return found;
    }

public:
    SmallTown(M monster, C... citizens) : _monster(monster), _citizens(citizens...) {
        _citizensAlive = sizeof...(citizens);
        _monsterAlive = true;
        _currentTime = t0;
    }

    template <typename T = typename M::valueType>
    std::tuple<std::string, T, size_t> getStatus() {
        return std::make_tuple("COS", _monster.getHealth(), _citizensAlive);
    }

    /*
     * One turn of a game clock
     */
    void tick(U timeStep) {
        std::cout << "Current time " << _currentTime << std::endl;

        if (_monsterAlive && _citizensAlive == 0) std::cout << "MONSTER WON" <<  std::endl;
        else if (!_monsterAlive && _citizensAlive > 0) std::cout << "CITIZENS WON" <<  std::endl;
        else if (!_monsterAlive && _citizensAlive == 0) std::cout << "DRAW" << std::endl;
        else {
            if (canAttack(_currentTime)) {
                makeAttack(_citizens);
            }
        }

        _currentTime += timeStep;
        _currentTime %= t1 + 1;
    }
};

#endif //JNP4_SMALLTOWN_H