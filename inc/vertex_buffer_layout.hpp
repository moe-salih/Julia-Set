#pragma once
#ifndef VERTEX_BUFFER_LAYOUT_HPP
#define VERTEX_BUFFER_LAYOUT_HPP

#include "oglcall.hpp"
#include <vector>
#include <cassert>

struct vertex_buffer_element{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int size_of_type(unsigned int type) {
        switch(type) {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
};


struct vertex_buffer_layout {
    private:
        std::vector<vertex_buffer_element> elements_;
        unsigned int stride_;
    public:
        vertex_buffer_layout();
        ~vertex_buffer_layout();

        template<typename T> void push(unsigned int count);

        template<> void push<float>(unsigned int count);
        template<> void push<unsigned int>(unsigned int count);
        template<> void push<unsigned char>(unsigned int count);
        
        inline const std::vector<vertex_buffer_element>& elements() const { return elements_; }
        inline const unsigned int stride() const { return stride_; }        
};

#endif//VERTEX_BUFFER_LAYOUT_HPP
#ifdef VERTEX_BUFFER_LAYOUT_IMPL

vertex_buffer_layout::vertex_buffer_layout() : stride_(0) { }
vertex_buffer_layout::~vertex_buffer_layout() { }

template<typename T> void vertex_buffer_layout::push(unsigned int count) {
    assert(false);
}

template<> void vertex_buffer_layout::push<float>(unsigned int count) {
    elements_.push_back({ GL_FLOAT, count, GL_FALSE });
    stride_ += count * vertex_buffer_element::size_of_type(GL_FLOAT);
}

template<> void vertex_buffer_layout::push<unsigned int>(unsigned int count) {
    elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    stride_ += count * vertex_buffer_element::size_of_type(GL_UNSIGNED_INT);
}

template<> void vertex_buffer_layout::push<unsigned char>(unsigned int count) {
    elements_.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    stride_ += count * vertex_buffer_element::size_of_type(GL_UNSIGNED_BYTE);
}

#endif//VERTEX_BUFFER_LAYOUT_IMPL