#pragma once
#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include "oglcall.hpp"

struct vertex_buffer {
    private:
        unsigned int id_;

    public:
        vertex_buffer();
        ~vertex_buffer();

        void set_data( const void* data, unsigned int size );

        void bind() const;
        void unbind() const;
};

#endif//VERTEX_BUFFER_HPP
#ifdef VERTEX_BUFFER_IMPL

vertex_buffer::vertex_buffer() {
    call( glGenBuffers( 1, &id_ ) );
}

void vertex_buffer::set_data( const void* data, unsigned int size ) {
    bind();
    call( glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW ) );
}

vertex_buffer::~vertex_buffer() {
    call( glDeleteBuffers( 1, &id_ ) );
}

void vertex_buffer::bind() const {
    call( glBindBuffer( GL_ARRAY_BUFFER, id_ ) );
}

void vertex_buffer::unbind() const {
    call( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
}

#endif//VERTEX_BUFFER_IMPL