#include "project/exercise.hpp"
#include <algorithm>
#include <utility>
#include <stdexcept>

// Default constructor initializing an empty buffer.
DynamicBuffer::DynamicBuffer() : data_(nullptr), size_(0) {
}

// Allocation and initialization with zero-filled elements.
DynamicBuffer::DynamicBuffer(size_t capacity) : data_(nullptr), size_(capacity) {
    if (size_ > 0) {
        data_ = new int[size_](); // Value-initializes (zeros) array elements
    }
}

// Deep-copy constructor.
DynamicBuffer::DynamicBuffer(const DynamicBuffer& other) : data_(nullptr), size_(other.size_) {
    if (size_ > 0) {
        data_ = new int[size_];
        std::copy(other.data_, other.data_ + size_, data_);
    }
}

// Move constructor.
DynamicBuffer::DynamicBuffer(DynamicBuffer&& other) noexcept 
    : data_(other.data_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = 0;
}

// Destructor with proper cleanup.
DynamicBuffer::~DynamicBuffer() {
    release();
}

// Copy assignment with self-assignment protection using copy-and-swap idiom.
DynamicBuffer& DynamicBuffer::operator=(const DynamicBuffer& other) {
    if (this != &other) {
        DynamicBuffer temp(other);
        swap(temp);
    }
    return *this;
}

// Move assignment.
DynamicBuffer& DynamicBuffer::operator=(DynamicBuffer&& other) noexcept {
    if (this != &other) {
        release();
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

// Return the current managed size.
size_t DynamicBuffer::size() const noexcept {
    return size_;
}

// Return true if the buffer is empty.
bool DynamicBuffer::empty() const noexcept {
    return size_ == 0;
}

// Implement resize with resource ownership and exception safety.
void DynamicBuffer::resize(size_t newSize) {
    if (newSize == size_) {
        return;
    }

    if (newSize == 0) {
        release();
        return;
    }

    int* newData = new int[newSize]();
    size_t copyCount = std::min(size_, newSize);
    if (copyCount > 0) {
        std::copy(data_, data_ + copyCount, newData);
    }

    delete[] data_;
    data_ = newData;
    size_ = newSize;
}

// Fill all elements with the given value.
void DynamicBuffer::fill(int value) {
    if (size_ > 0) {
        std::fill(data_, data_ + size_, value);
    }
}

// Validate index and assign the value.
void DynamicBuffer::setAt(size_t index, int value) {
    if (index >= size_) {
        throw std::out_of_range("Index out of bounds");
    }
    data_[index] = value;
}

// Return element at index with bounds checking.
int DynamicBuffer::at(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of bounds");
    }
    return data_[index];
}

// Return a reference without bounds checking.
int& DynamicBuffer::operator[](size_t index) {
    return data_[index];
}

// Return const reference without bounds checking.
const int& DynamicBuffer::operator[](size_t index) const {
    return data_[index];
}

// Compare size and elements.
bool DynamicBuffer::operator==(const DynamicBuffer& other) const {
    if (size_ != other.size_) {
        return false;
    }
    return size_ == 0 || std::equal(data_, data_ + size_, other.data_);
}

// Implement inequality comparison.
bool DynamicBuffer::operator!=(const DynamicBuffer& other) const {
    return !(*this == other);
}

// Return true when the buffer owns valid memory.
DynamicBuffer::operator bool() const noexcept {
    return data_ != nullptr;
}

// Delete allocated memory and reset state.
void DynamicBuffer::release() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
}

// Deep-copy the other object's contents.
void DynamicBuffer::copyFrom(const DynamicBuffer& other) {
    if (this == &other) {
        return;
    }
    
    DynamicBuffer copy(other);
    swap(copy);
}

// Swap the resources of two buffers.
void DynamicBuffer::swap(DynamicBuffer& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
}