#ifndef SHARED_HPP
#define SHARED_HPP

#include <iostream>

template <typename T>
class shared_ptr;

template <typename T>
class weak_ptr;

template <typename T>
class Control_block {
private:
    T* m_ptr;
    int count_weak;
    int count_shared;
    friend class weak_ptr<T>;
public:
    Control_block(T* ptr);
    void add_shared_count();
    void add_weak_count();
    T* get() const;
    int get_shared_count() const;
    int get_weak_count() const;

    void release_shared();
    void release_weak();
};

template <typename T>
class shared_ptr {
private:
    T* m_ptr;
    Control_block<T>* control_block;
    friend class weak_ptr<T>;
public:
    shared_ptr();
    explicit shared_ptr(T* ptr);
    shared_ptr(const shared_ptr<T>& other);
    shared_ptr<T>& operator=(const shared_ptr<T>& other);
    shared_ptr(shared_ptr<T>&& other) noexcept;
    shared_ptr& operator=(shared_ptr&& other) noexcept;
    shared_ptr(const weak_ptr<T>& other);

    T* operator->() const;
    T& operator*() const;
    explicit operator bool() const;
    T* get() const;
    int use_count() const;

    ~shared_ptr();

private:
    void release();
};

template <typename T>
class weak_ptr {
private:
    T* m_ptr;
    Control_block<T>* control_block;
    friend class shared_ptr<T>;
public:
    weak_ptr();
    weak_ptr(T* ptr);
    weak_ptr(const shared_ptr<T>& rhs);
    weak_ptr(const weak_ptr& other);
    weak_ptr(weak_ptr&& other) noexcept;
    weak_ptr& operator=(const weak_ptr& other);
    weak_ptr& operator=(weak_ptr&& other) noexcept;

    shared_ptr<T> lock() const;
    int get_count() const;

    ~weak_ptr();
};

#endif 
