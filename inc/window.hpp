
#pragma once

#ifndef WINDOW_HPP
#define WINDOW_HPP

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif//STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>

#include "vec.hpp"

struct color { float r,g,b,a; };

struct key_event {
    int key;
    int scancode;
    int action;
    int mods;
};

struct state {
    static void key_callback( GLFWwindow* _wnd, int key, int scancode, int action, int mods );
    static void mouse_button_callback( GLFWwindow* _wnd, int button, int action, int mods );
    static void mouse_move_callback( GLFWwindow* _wnd, double xpos, double ypos );
    static void mouse_scroll_callback( GLFWwindow* _wnd, double xoffset, double yoffset );
    static void window_resize_callback( GLFWwindow* _wnd, int width, int height );
};


struct window {
    protected:
        GLFWwindow* id_ = nullptr;
        GLFWmonitor* monitor_ = nullptr;
        vec2i position_;
        vec2i vp_size_;
        vec2i size_;
        bool update_viewprot = true;


    public:
        window( const char* title, const vec2i& size );
        ~window();

        virtual void on_user_create() = 0;
        virtual void on_user_update() = 0;
        virtual void on_user_destroy() = 0;

        inline void start() {
            initialize_event_callbacks();
            on_user_create();
            while ( is_open() ) {

                if( update_viewprot ) {
                    glfwGetFramebufferSize( id_, &vp_size_.x, &vp_size_.y );
                    glViewport( 0, 0, vp_size_.x, vp_size_.y );
                    update_viewprot = false;
                }

                on_user_update();
                swap_buffers();
                poll_events();
            }
            on_user_destroy();
        }

        inline void close() { glfwSetWindowShouldClose( id_, true ); }

        inline bool is_open() const { return !glfwWindowShouldClose( id_ ); }
        inline void poll_events() { glfwPollEvents(); }

        inline void swap_buffers();
        inline void clear( const color& c = { 0.0f, 0.0f, 0.0f, 1.0f } );

        inline void set_title( const char* title );
        inline void resize( const vec2i& vec2i );
        inline void set_position( const vec2i& pos );
        inline void set_icon( const char* path );
        
        inline vec2i get_size() const;
        inline vec2i get_position() const;
        
        inline bool is_fullscreen() const { 
            return glfwGetWindowMonitor( id_ ) != nullptr; 
        }

        inline void set_fullscreen( bool fullscreen );
        inline void toggle_fullscreen() { set_fullscreen( !is_fullscreen() ); }

        inline void set_vsync( bool vsync ) { glfwSwapInterval( vsync ? 1 : 0 ); }

        virtual void handle_key( const key_event& e ) {};
        virtual void handle_mouse_button( const key_event& e ) {};
        virtual void handle_mouse_move( const vec2d& pos ) {};
        virtual void handle_mouse_scroll( const vec2d& offset ) {};

            

    private:
        void init( const char* title, const vec2i& size );
        void terminate();

        void initialize_event_callbacks() {
            glfwSetKeyCallback( id_, state::key_callback );
            glfwSetMouseButtonCallback( id_, state::mouse_button_callback );
            glfwSetCursorPosCallback( id_, state::mouse_move_callback );
            glfwSetWindowSizeCallback( id_, state::window_resize_callback );
            glfwSetScrollCallback( id_, state::mouse_scroll_callback );
        }
};

#endif//WINDOW_HPP
#ifdef WINDOW_IMPL

window::window( const char* title, const vec2i& size ) {
    init( title, size );
}

window::~window() {
    glfwDestroyWindow( id_ );
    glfwTerminate();
}

void window::init( const char* title, const vec2i& size ) {
    if(!glfwInit()) {
        fprintf( stderr, "GLFW failed to initialize\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 4 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    size_ = size;
    id_ = glfwCreateWindow( size.x, size.y, title, nullptr, nullptr );
    if ( id_ == nullptr ) {
        fprintf( stderr, "Failed to create GLFW window\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    glfwMakeContextCurrent( id_ );

    glfwSetWindowUserPointer(id_, this);

    monitor_ = glfwGetPrimaryMonitor();
    size_ = get_size();
    position_ = get_position();
    update_viewprot = true;

    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK ) {
        fprintf( stderr, "Failed to initialize GLEW\n" );
        exit( EXIT_FAILURE );
    }
}

inline void window::set_fullscreen( bool fullscreen ) {
    if(is_fullscreen() == fullscreen) return;


    if ( fullscreen ) {
        glfwGetWindowPos( id_, &position_.x, &position_.y );
        glfwGetWindowSize( id_, &size_.x, &size_.y );

        const GLFWvidmode* mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );

        glfwSetWindowMonitor( id_, monitor_, 0, 0, mode->width, mode->height, mode->refreshRate );
    } else {
        glfwSetWindowMonitor( id_, nullptr, position_.x, position_.y, size_.x, size_.y, 0 );
    }

    update_viewprot = true;
}

inline vec2i window::get_position() const {
    vec2i pos;
    glfwGetWindowPos( id_, &pos.x, &pos.y );
    return pos;
}

inline void window::set_position( const vec2i& pos ) {
    glfwSetWindowPos( id_, pos.x, pos.y );
}

inline void window::set_icon( const char* path ) {
    GLFWimage icon;
    icon.pixels = stbi_load( path, &icon.width, &icon.height, 0, 4 );
    glfwSetWindowIcon( id_, 1, &icon );
    stbi_image_free( icon.pixels );
}

inline vec2i window::get_size() const {
    return size_;
}

inline void window::resize( const vec2i& size ) {
    update_viewprot = true;
}

inline void window::set_title( const char* title ) {
    glfwSetWindowTitle( id_, title );
}

inline void window::swap_buffers() {
    glfwSwapBuffers( id_ );
}

inline void window::clear(const color &c)
{
    glClearColor( c.r, c.g, c.b, c.a );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void state::key_callback( GLFWwindow* _wnd, int key, int scancode, int action, int mods ) {
    window* wnd = static_cast<window*>( glfwGetWindowUserPointer( _wnd ) );
    wnd->handle_key( { key, scancode, action, mods } );
}

void state::mouse_button_callback( GLFWwindow* _wnd, int button, int action, int mods ) {
    window* wnd = static_cast<window*>( glfwGetWindowUserPointer( _wnd ) );
    wnd->handle_mouse_button( { button, 0, action, mods } );
}

void state::mouse_move_callback( GLFWwindow* _wnd, double xpos, double ypos ) {
    window* wnd = static_cast<window*>( glfwGetWindowUserPointer( _wnd ) );
    wnd->handle_mouse_move( { xpos, ypos } );
}

void  state::mouse_scroll_callback( GLFWwindow* _wnd, double xoffset, double yoffset ) {
    window* wnd = static_cast<window*>( glfwGetWindowUserPointer( _wnd ) );
    wnd->handle_mouse_scroll( { xoffset, yoffset } );
}

inline void state::window_resize_callback(GLFWwindow *_wnd, int width, int height) {
    window* wnd = static_cast<window*>( glfwGetWindowUserPointer( _wnd ) );
    wnd->resize( { width, height } );
}

#endif//WINDOW_IMPL