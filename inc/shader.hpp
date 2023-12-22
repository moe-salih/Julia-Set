#pragma once

#ifndef SHADER_HPP
#define SHADER_HPP

#include "vec.hpp"
#include "oglcall.hpp"

#include <cstdio>
#include <cstdlib>
#include <malloc.h>

#include <unordered_map>
#include <string>

struct shader {
    private:
        unsigned int id_;
        std::unordered_map<std::string, int> uniform_cache_;
    public:
        shader();
        shader( const char* vert_file, const char* frag_file);
        ~shader();

        void set_source( const char* vert_file, const char* frag_file );    

        void bind() const;
        void unbind() const;


        // implement set unifrom using template
        template<typename T> void set_uniform( const char* name, T value ) {
            assert( false );
        }

        template<> void set_uniform( const char* name, int value);
        template<> void set_uniform( const char* name, float value);
        template<> void set_uniform( const char* name, double value);
        template<> void set_uniform( const char* name, vec2i value );
        template<> void set_uniform( const char* name, vec2f value );
        template<> void set_uniform( const char* name, vec2d value );
        template<> void set_uniform( const char* name, vec3i value );
        template<> void set_uniform( const char* name, vec3f value );
        template<> void set_uniform( const char* name, vec3d value );
        template<> void set_uniform( const char* name, vec4i value );
        template<> void set_uniform( const char* name, vec4f value );
        template<> void set_uniform( const char* name, vec4d value );

    private:
        int uniform_location( const std::string& name );

        unsigned int compile_shader( unsigned int type, const char* source ) const;
        unsigned int create_shader( const char* vertex_shader, const char* fragment_shader ) const;
        std::string load_shader( const char* file ) const;

};

#endif//SHADER_HPP
#ifdef SHADER_IMPL

shader::shader() {}
shader::shader( const char* vertex_shader, const char* fragment_shader ) {
    set_source( vertex_shader, fragment_shader );
}

void shader::set_source( const char* vertex_shader, const char* fragment_shader ) {
    id_ = create_shader( vertex_shader, fragment_shader );
}

inline shader::~shader() {
    call( glDeleteProgram( id_ ) );
}

inline void shader::bind() const {
    call( glUseProgram( id_ ) );
}

inline void shader::unbind() const {
    call( glUseProgram( 0 ) );
}

inline int shader::uniform_location(const std::string& name) {
    if( uniform_cache_.find( name ) != uniform_cache_.end() ) {
        return uniform_cache_[name];
    }

    int location = glGetUniformLocation( id_, name.c_str() );

    if( location == -1 ) {
        printf( "Warning: uniform '%s' doesn't exist!\n", name.c_str() );
    }

    uniform_cache_[name] = location;

    return 0;
}

unsigned int shader::compile_shader(unsigned int type, const char *source) const
{
    unsigned int id = glCreateShader( type );
    glShaderSource( id, 1, &source, nullptr );
    glCompileShader( id );

    int result;
    glGetShaderiv( id, GL_COMPILE_STATUS, &result );
    if( result == GL_FALSE ) {
        int length;
        glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );
        char* message = (char*)alloca( length * sizeof( char ) );
        glGetShaderInfoLog( id, length, &length, message );
        printf( "Failed to compile %s shader:\n%s\n", ( type == GL_VERTEX_SHADER ? "vertex" : "fragment" ), message );
        glDeleteShader( id );
        return 0;
    }
    return id;
}

unsigned int shader::create_shader( const char* vert_file, const char* frag_file) const {
    unsigned int program = glCreateProgram();
    
    // load the source code 
    std::string vertex_shader_source = load_shader( vert_file );
    std::string fragment_shader_source = load_shader( frag_file );

    // compile the source code
    unsigned int vs = compile_shader( GL_VERTEX_SHADER, vertex_shader_source.c_str() );
    unsigned int fs = compile_shader( GL_FRAGMENT_SHADER, fragment_shader_source.c_str() );    


    glAttachShader( program, vs );
    glAttachShader( program, fs );
    glLinkProgram( program );
    glValidateProgram( program );

    glDeleteShader( vs );
    glDeleteShader( fs );

    // check for errors
    int result;
    glGetProgramiv( program, GL_LINK_STATUS, &result );
    if( result == GL_FALSE ) {
        int length;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &length );
        char* message = (char*)alloca( length * sizeof( char ) );
        glGetProgramInfoLog( program, length, &length, message );
        printf( "Failed to link shader program:\n%s\n", message );
        glDeleteProgram( program );
        return 0;
    }

    return program;
}

std::string shader::load_shader( const char* file ) const {
    FILE* fp = fopen( file, "r" );
    if( !fp ) {
        printf( "Failed to open shader file: %s\n", file );
        return "";
    }

    fseek( fp, 0, SEEK_END );
    size_t size = ftell( fp );
    fseek( fp, 0, SEEK_SET );

    std::string buffer( size, '\0' );
    
    size_t u = 0, k = 0;
    k = fread( &buffer[0], 1, size, fp );
    while(k < size) {
        u = fread( &buffer[k], 1, size - k, fp );
        if( u == 0 ) break;
        k += u;
    }

    fclose( fp );
    return buffer;
}


// set uniform template specializations
template<> void shader::set_uniform( const char* name, int value ) {
    call( glUniform1i( uniform_location( name ), value ) );
}

template<> void shader::set_uniform( const char* name, float value ) {
    call( glUniform1f( uniform_location( name ), value ) );
}

template<> void shader::set_uniform( const char* name, double value ) {
    call( glUniform1d( uniform_location( name ), value ) );
}

template<> void shader::set_uniform( const char* name, vec2i value ) {
    call( glUniform2i( uniform_location( name ), value.x, value.y ) );
}

template<> void shader::set_uniform( const char* name, vec2f value ) {
    call( glUniform2f( uniform_location( name ), value.x, value.y ) );
}

template<> void shader::set_uniform( const char* name, vec2d value ) {
    call( glUniform2d( uniform_location( name ), value.x, value.y ) );
}

template<> void shader::set_uniform( const char* name, vec3i value ) {
    call( glUniform3i( uniform_location( name ), value.x, value.y, value.z ) );
}

template<> void shader::set_uniform( const char* name, vec3f value ) {
    call( glUniform3f( uniform_location( name ), value.x, value.y, value.z ) );
}

template<> void shader::set_uniform( const char* name, vec3d value ) {
    call( glUniform3d( uniform_location( name ), value.x, value.y, value.z ) );
}

template<> void shader::set_uniform( const char* name, vec4i value ) {
    call( glUniform4i( uniform_location( name ), value.x, value.y, value.z, value.w ) );
}

template<> void shader::set_uniform( const char* name, vec4f value ) {
    call( glUniform4f( uniform_location( name ), value.x, value.y, value.z, value.w ) );
}

template<> void shader::set_uniform( const char* name, vec4d value ) {
    call( glUniform4d( uniform_location( name ), value.x, value.y, value.z, value.w ) );
}



#endif//SHADER_IMPL