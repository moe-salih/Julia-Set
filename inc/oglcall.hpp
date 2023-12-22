

#ifndef OGLCALL_HPP
#define OGLCALL_HPP

#define GLEW_STATIC
#include <GL/glew.h>

#include <cstdio>


#define call( x ) ogl_clear_error(); x; ogl_list_error( #x, __LINE__ );


inline void ogl_clear_error() {
    while ( glGetError() != GL_NO_ERROR ) ;
}

inline void ogl_list_error( const char* function, int line ) {
    int error;
    while ( (error = glGetError()) != GL_NO_ERROR ) {
        fprintf(stderr, "Error: [CODE: 0x%x] %s %d @%s \n", error, __FILE__, line, function);
    }
}

#endif//OGLCALL_HPP
