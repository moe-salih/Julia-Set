#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

#ifndef VERTEX_ARRAY_IMPL
#define VERTEX_ARRAY_IMPL
#endif//VERTEX_ARRAY_IMPL
#include "vertex_array.hpp"

#ifndef INDEX_BUFFER_IMPL
#define INDEX_BUFFER_IMPL
#endif//INDEX_BUFFER_IMPL
#include "index_buffer.hpp"

#ifndef SHADER_IMPL
#define SHADER_IMPL
#endif//SHADER_IMPL
#include "shader.hpp"

struct renderer {
    public:
        void clear() const;
        void draw( const vertex_array& , const index_buffer&, const shader& ) const;
};


#endif//RENDERER_HPP
#ifdef RENDERER_IMPL

void renderer::clear() const {
    call( glClear( GL_COLOR_BUFFER_BIT ) );
}

void renderer::draw( const vertex_array& va, const index_buffer& ib, const shader& sh ) const {
    va.bind();
    ib.bind();
    // sh.bind();
    call( glDrawElements( GL_TRIANGLES, ib.count(), GL_UNSIGNED_INT, nullptr ) );
}

#endif//RENDERER_IMPL