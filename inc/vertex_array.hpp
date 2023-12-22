#pragma once
#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "oglcall.hpp"

#ifndef VERTEX_BUFFER_IMPL
#define VERTEX_BUFFER_IMPL
#endif//VERTEX_BUFFER_IMPL
#ifndef VERTEX_BUFFER_LAYOUT_IMPL
#define VERTEX_BUFFER_LAYOUT_IMPL
#endif//VERTEX_BUFFER_LAYOUT_IMPL

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"


struct vertex_array {
    private:
        unsigned int id_;

    public:
        vertex_array();
        ~vertex_array();

        void bind() const;
        void unbind() const;

        void add_buffer( const vertex_buffer& vb, const vertex_buffer_layout& layout );
};


#endif//VERTEX_ARRAY_HPP
#ifdef VERTEX_ARRAY_IMPL

vertex_array::vertex_array() {
    call( glGenVertexArrays( 1, &id_ ) );
}

vertex_array::~vertex_array() {
    call( glDeleteVertexArrays( 1, &id_ ) );
}

void vertex_array::bind() const {
    call( glBindVertexArray( id_ ) );
}

void vertex_array::unbind() const {
    call( glBindVertexArray( 0 ) );
}

void vertex_array::add_buffer( const vertex_buffer& vb, const vertex_buffer_layout& layout ) {
    bind();
    vb.bind();
    const auto& elements = layout.elements();
    size_t offset = 0;
    for( unsigned int i = 0; i < elements.size(); ++i ) {
        const auto& element = elements[i];
        call( glEnableVertexAttribArray( i ) );
        call( glVertexAttribPointer( i, element.count, element.type, element.normalized, layout.stride(), (const void*)offset ) );
        offset += element.count * vertex_buffer_element::size_of_type( element.type );
    }
}

#endif//VERTEX_ARRAY_IMPL