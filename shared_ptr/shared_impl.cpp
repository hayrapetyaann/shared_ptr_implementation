#include "shared.hpp"

template <typename T>
Control_block<T>::Control_block(T* ptr) : m_ptr(ptr), count_weak(0), count_shared(1) {}

template <typename T>
void Control_block<T>::add_shared_count() {
    ++count_shared;
}

template <typename T>
void Control_block<T>::add_weak_count() {
    ++count_weak;
}

template <typename T>
T* Control_block<T>::get() const {
    return m_ptr;
}

template <typename T>
int Control_block<T>::get_shared_count() const {
    return count_shared;
}

template <typename T>
int Control_block<T>::get_weak_count() const {
    return count_weak;
}

template <typename T>
void Control_block<T>::release_shared() {
    if (count_shared) {
        if (!(--count_shared)) {
            delete m_ptr;
            m_ptr = nullptr;
            if (count_weak == 0) {
                delete this;
            }
        }
    }
}

template <typename T>
void Control_block<T>::release_weak() {
    if ((--count_weak == 0) && (count_shared == 0)) {
        delete this;
    }
}

template <typename T>
shared_ptr<T>::shared_ptr() : m_ptr(nullptr), control_block(nullptr) {}

template <typename T>
shared_ptr<T>::shared_ptr(T* ptr) : m_ptr(ptr) {
    control_block = new Control_block<T>(ptr);
}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& other) {
    if (other.control_block) {
        m_ptr = other.m_ptr;
        control_block = other.control_block;
        control_block->add_shared_count();
    }
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& other) {
    if (this != &other) {
        release();
        m_ptr = other.m_ptr;
        control_block = other.control_block;
        if (control_block) {
            control_block->add_shared_count();
        }
    }
    return *this;
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& other) noexcept : m_ptr(other.m_ptr), control_block(other.control_block) {
    other.m_ptr = nullptr;
    other.control_block = nullptr;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& other) noexcept {
    if (this != &other) {
        release();
        m_ptr = other.m_ptr;
        control_block = other.control_block;
        other.m_ptr = nullptr;
        other.control_block = nullptr;
    }
    return *this;
}

template <typename T>
shared_ptr<T>::shared_ptr(const weak_ptr<T>& other) {
    if (other.control_block && other.control_block->get_shared_count() > 0) {
        m_ptr = other.m_ptr;
        control_block = other.control_block;
        control_block->add_shared_count();
    } else {
        m_ptr = nullptr;
        control_block = nullptr;
    }
}

template <typename T>
T* shared_ptr<T>::operator->() const {
    return m_ptr;
}

template <typename T>
T& shared_ptr<T>::operator*() const {
    return *m_ptr;
}

template <typename T>
shared_ptr<T>::operator bool() const {
    return m_ptr != nullptr;
}

template <typename T>
T* shared_ptr<T>::get() const {
    return m_ptr;
}

template <typename T>
int shared_ptr<T>::use_count() const {
    if (control_block) {
        return control_block->get_shared_count();
    }
    return 0;
}

template <typename T>
shared_ptr<T>::~shared_ptr() {
    if (control_block != nullptr) {
        release();
    }
}

template <typename T>
void shared_ptr<T>::release() {
    if (control_block) {
        control_block->release_shared();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr() : m_ptr(nullptr), control_block(nullptr) {}

template <typename T>
weak_ptr<T>::weak_ptr(T* ptr) : m_ptr(ptr), control_block(nullptr) {}

template <typename T>
weak_ptr<T>::weak_ptr(const shared_ptr<T>& rhs) : m_ptr(rhs.m_ptr), control_block(rhs.control_block) {
    if (control_block) {
        control_block->add_weak_count();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr& other) : m_ptr(other.m_ptr), control_block(other.control_block) {
    if (control_block) {
        control_block->add_weak_count();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr(weak_ptr&& other) noexcept : m_ptr(other.m_ptr), control_block(other.control_block) {
    other.m_ptr = nullptr;
    other.control_block = nullptr;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr& other) {
    if (this != &other) {
        if (control_block) {
            control_block->release_weak();
        }
        m_ptr = other.m_ptr;
        control_block = other.control_block;
        if (control_block) {
            control_block->add_weak_count();
        }
    }
    return *this;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr&& other) noexcept {
    if (this != &other) {
        if (control_block) {
            control_block->release_weak();
        }
        m_ptr = other.m_ptr;
        control_block = other.control_block;
        other.m_ptr = nullptr;
        other.control_block = nullptr;
    }
    return *this;
}

template <typename T>
shared_ptr<T> weak_ptr<T>::lock() const {
    return (control_block && control_block->get_shared_count() > 0) ? shared_ptr<T>(*this) : shared_ptr<T>();
}

template <typename T>
int weak_ptr<T>::get_count() const {
    return control_block ? control_block->get_weak_count() : 0;
}

template <typename T>
weak_ptr<T>::~weak_ptr() {
    if (control_block) {
        control_block->release_weak();
    }
}

template class shared_ptr<int>;
template class weak_ptr<int>;
