
#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "oglcall.hpp"

struct index_buffer {
    private:
        unsigned int id_;
        unsigned int count_;
    public:
        index_buffer() {
            call( glGenBuffers( 1, &id_ ) );
        }
        index_buffer( const unsigned int* data, unsigned int count) {
            call( glGenBuffers( 1, &id_ ) );
            set_data( data, count );
        }
        ~index_buffer() {
            call( glDeleteBuffers( 1, &id_ ) );
        }

        void set_data( const unsigned int* data, unsigned int count ) {
            bind();
            count_ = count;
            call( glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW ) );
        }

        void bind() const { call( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_ ) ); }
        void unbind() const { call( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) ); }

        inline unsigned int count() const { return count_; }
};

#endif//INDEX_BUFFER_HPP
#ifdef INDEX_BUFFER_IMPL

#endif//INDEX_BUFFER_IMPL