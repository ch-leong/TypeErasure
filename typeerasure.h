#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <iostream>

class Player;

template<typename T>
struct IfPlayerNoType
{
    using type = T;
};

template<>
struct IfPlayerNoType<Player>
{};

class Player
{
private:
    // The base class that contains the type erasure.
    struct Model
    {
        virtual void Play() = 0; // Function that required Polymorphic Behavior.
        virtual std::shared_ptr<Model> clone() = 0;
        virtual ~Model() {}
    };
    // The derived class that contains the type erasure
    template<typename U>
    struct Instance : Model
    {
        Instance(U const& t) : m_Obj(t) {}
        Instance(U&& u) : m_Obj(std::move<U>(u)) {}
        void Play() { m_Obj.Play(); } // Function that required Polymorphic Behavior.
        std::shared_ptr<Model> clone() { return std::make_shared<Instance<U>>(m_Obj); }
        ~Instance() {}
        U m_Obj;
    };
public:
    // SFINAE Template non-default constructor.
    template<typename T, typename V = typename IfPlayerNoType<T>::type>
    Player(T const& t) : m_Ptr(std::make_shared<Instance<T>>(t)) {}
    Player() noexcept : m_Ptr(nullptr) {}
    Player(Player&& rhs) noexcept : m_Ptr(rhs.m_Ptr) { rhs.m_Ptr = nullptr; }
    Player(Player const& rhs)
        : m_Ptr(nullptr)
    {
        if (rhs.m_Ptr)
            m_Ptr = rhs.m_Ptr->clone();
    }
    Player& operator=(Player&& rhs) noexcept
    {
        std::swap(rhs.m_Ptr, m_Ptr);
        return *this;
    }
    Player& operator=(Player const& rhs)
    {
        if (rhs.m_Ptr)
            m_Ptr = rhs.m_Ptr->clone();
        else
            m_Ptr = nullptr;
        return *this;
    }
    // Function that required Polymorphic Behavior.
    void Play() { if (!m_Ptr) throw std::exception(); m_Ptr->Play(); }
    ~Player() {}
private:
    std::shared_ptr<Model> m_Ptr;
};
