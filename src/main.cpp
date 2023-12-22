
#include "windows.h"
#include "main.hpp"

struct app : public window {
    private:

        const float verticies[12] = {
            -1.0f, -1.0f, 0.0f, // 0
             1.0f, -1.0f, 0.0f, // 1
             1.0f,  1.0f, 0.0f, // 2
            -1.0f,  1.0f, 0.0f  // 3
        };
        
        const unsigned int indices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        vertex_array vao_;
        vertex_buffer vbo_;
        vertex_buffer_layout layout_;
        index_buffer ibo_;
        shader shader_;
        renderer renderer_;
        
        color l = { 32.0f/255.0f, 32.0f/255.0f, 32.0f/255.0f, 1.0f };
        color h = { 0.2f, 0.5f, 0.7f, 1.0f };
        int iterations = 10;
        int power = 2;
        vec2f c = {0, 0};
        bool paused = false;

        vec4f world = { -2.0f, -2.0f, 2.0f, 2.0f };
        float zoom = 1.0f;

    public:

        app( const char* title, const vec2i& size ) : window( title, size ) {
            set_vsync( true );
            set_icon( "./res/icons/icon.png" );

            // get gpu info
            printf( "GPU: %s\n", glGetString( GL_RENDERER ) );
        }
        ~app() {}

        void on_user_create() override {
            vbo_.set_data( verticies, 4*3*sizeof(float) );

            layout_.push<float>( 3 );
            vao_.add_buffer( vbo_, layout_ );
            
            ibo_.set_data( indices, 6 );

            shader_.set_source("./res/shaders/vert.glsl", "./res/shaders/frag.glsl");
        }

        void on_user_update() override {
            clear(l);

            shader_.bind();
            shader_.set_uniform("world", world);
            shader_.set_uniform("low", vec4f{ l.r, l.g, l.b, l.a } );
            shader_.set_uniform("high", vec4f{ h.r, h.g, h.b, h.a } );
            shader_.set_uniform("iterations", iterations);
            shader_.set_uniform("power", power);
            if(!paused) shader_.set_uniform("mouse", c );
            shader_.set_uniform("resolution", vec2f{ float(vp_size_.x), float(vp_size_.y) } );

            renderer_.draw( vao_, ibo_, shader_ );

        }

        void on_user_destroy() override {

        }

        void handle_key( const key_event& e ) override {
            if(e.key == GLFW_KEY_ESCAPE && e.action == GLFW_PRESS) {
                close();
            }

            if(e.key == GLFW_KEY_UP && (e.action == GLFW_REPEAT || e.action == GLFW_PRESS)) {
                iterations += 1;
            } else if(e.key == GLFW_KEY_DOWN && (e.action == GLFW_REPEAT || e.action == GLFW_PRESS)) {
                (iterations > 1 ) ? iterations -= 1 : iterations = 1;
            }

            if(e.key == GLFW_KEY_RIGHT && (e.action == GLFW_REPEAT || e.action == GLFW_PRESS)) {
                power += 1.0;
            } else if(e.key == GLFW_KEY_LEFT && (e.action == GLFW_REPEAT || e.action == GLFW_PRESS)) {
                power -= 1.0;
            }
        
            if(e.key == GLFW_KEY_SPACE && e.action == GLFW_PRESS) {
                paused = !paused;
                if(!paused) {
                    world = { -2.0f, -2.0f, 2.0f, 2.0f };
                } 
            }

            // enable fullscreen
            if(e.key == GLFW_KEY_F11 && e.action == GLFW_PRESS) {
                toggle_fullscreen();
            }

            // navigate world
            switch(e.action) {
                case GLFW_REPEAT:
                case GLFW_PRESS:
                    if(!paused) return;

                    switch(e.key) {
                        case GLFW_KEY_W:
                            world.y += 0.1f * (world.w - world.y);
                            world.w += 0.1f * (world.w - world.y);
                            break;
                        case GLFW_KEY_S:
                            world.y -= 0.1f * (world.w - world.y);
                            world.w -= 0.1f * (world.w - world.y);
                            break;
                        case GLFW_KEY_A:
                            world.x -= 0.1f * (world.z - world.x);
                            world.z -= 0.1f * (world.z - world.x);
                            break;
                        case GLFW_KEY_D:
                            world.x += 0.1f * (world.z - world.x);
                            world.z += 0.1f * (world.z - world.x);
                            break;

                        case GLFW_KEY_E:
                            // zoom in:
                            world.x += 0.1f * (world.z - world.x);
                            world.y += 0.1f * (world.w - world.y);
                            world.z -= 0.1f * (world.z - world.x);
                            world.w -= 0.1f * (world.w - world.y);
                            break;
                        case GLFW_KEY_Q:
                            // zoom out:
                            world.x -= 0.1f * (world.z - world.x);
                            world.y -= 0.1f * (world.w - world.y);
                            world.z += 0.1f * (world.z - world.x);
                            world.w += 0.1f * (world.w - world.y);
                            break;
                    }
                    break;
                break;
            }   
        }

        void handle_mouse_move( const vec2d& pos ) override {
            vec2i size = get_size();
            c = { 2 * (float(pos.x) / vp_size_.x) - 1.0f, 2 * (float(pos.y) / vp_size_.y) - 1.0f};
            // shader_.set_uniform("c", vec2d{ c.x, c.y } );
        }
};

// WinMain 
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

    

    app wnd( "Julia Set", { 1024 ,512 } );
    wnd.start();


    return 0;
}