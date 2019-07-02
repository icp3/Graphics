


/* Author : Mohamed - Last Updated : June 2019 */

#include <iostream>

#include <HUD.h>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

global Camera camera;

global const int SCREEN_WIDTH  = 1920;
global const int SCREEN_HEIGHT = 1080;

global float lastX = SCREEN_WIDTH  / 2; // Half SCREEN_WIDTH
global float lastY = SCREEN_HEIGHT / 2; // Half SCREEN_HEIGHT
global bool firstMouse = true;

bool Paused = false;

int main()
{
    /*---------------- Aquiring Game Memory ----------------*/

    uint GameMemorySize = MEM_PAGE_SIZE * TOTAL_MEM_PAGES;
    void* GameMemory = VirtualAlloc(0, GameMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); //ask 4 sum memory

    Game_Memory MemoryManager;
    GameMemoryInit(&MemoryManager, (Mem_Page*)GameMemory);

    /*---------------- initializing GLEW & GLFW ----------------*/

    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* WindowInstance = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Building & Tower Defense Game", NULL, NULL);
    if(!WindowInstance) { glfwTerminate(); return 1; }

    glfwMakeContextCurrent(WindowInstance);
    glfwSwapInterval(1);

    //Capture the cursor
    glfwSetInputMode(WindowInstance, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Setup Callbacks
    glfwSetCursorPosCallback(WindowInstance, mouse_callback);
    glfwSetFramebufferSizeCallback(WindowInstance, framebuffer_size_callback);
    glfwSetScrollCallback(WindowInstance, scroll_callback);

    //GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    /*-------------------------------------------------- Preparing for game loop --------------------------------------------------*/

    /*---------------- HUD Setup ----------------*/

    float quad_vertices[]{
            // X     Y
            0.000f, 0.10f, // Left  - Top - 0
            0.056f, 0.10f, // Right - Top - 1
            0.056f, 0.00f, // Right - Bottom - 2
            0.000f, 0.00f  // Left  - Bottom - 3
    };

    GLuint quad_elements[]{
            3,0,1,
            1,2,3
    };

    GLuint HUD_VAO;
    glGenVertexArrays(1, &HUD_VAO);
    glBindVertexArray(HUD_VAO);

#define HUD_RENDER_MEM_SIZE sizeof(quad_vertices) + (4096 * HUD_MEM_PAGES)
    GLuint HUD_VBO;
    glGenBuffers(1, &HUD_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, HUD_VBO);
    glBufferData(GL_ARRAY_BUFFER, HUD_RENDER_MEM_SIZE, quad_vertices, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad_vertices), quad_vertices);

    GLuint HUD_EBO;
    glGenBuffers(1, &HUD_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, HUD_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_elements), quad_elements, GL_STATIC_DRAW);

    {
        GLint PosAttrib = 0;
        glVertexAttribPointer(PosAttrib, 2, GL_FLOAT, GL_FALSE, NULL, (void*)0);
        glEnableVertexAttribArray(PosAttrib);

        GLint OffsetAttrib = 1;
        glVertexAttribPointer(OffsetAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Quad_Renderable), (void*)(sizeof(quad_vertices)));
        glVertexAttribDivisor(OffsetAttrib, 1);
        glEnableVertexAttribArray(OffsetAttrib);

        GLint ScaleAttrib = 2;
        glVertexAttribPointer(ScaleAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Quad_Renderable), (void*)(sizeof(quad_vertices) + sizeof(vec2)));
        glVertexAttribDivisor(ScaleAttrib, 1);
        glEnableVertexAttribArray(ScaleAttrib);

        GLint ColorAttrib = 3;
        glVertexAttribPointer(ColorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Quad_Renderable), (void*)(sizeof(quad_vertices) + sizeof(vec4)));
        glVertexAttribDivisor(ColorAttrib, 1);
        glEnableVertexAttribArray(ColorAttrib);
    }

    GLuint HUDVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(HUDVertShader, 1, &HUDVertSource, NULL);
    glCompileShader(HUDVertShader);

    GLuint HUDFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(HUDFragShader, 1, &HUDFragSource, NULL);
    glCompileShader(HUDFragShader);

#if 0
    {
		GLint ShaderStatus;
		glGetShaderiv(HUDVertShader, GL_COMPILE_STATUS, &ShaderStatus);
		out("HUD Vert shader status: " << ShaderStatus); DebugShader(HUDVertShader);
		glGetShaderiv(HUDFragShader, GL_COMPILE_STATUS, &ShaderStatus);
		out("HUD Frag shader status: " << ShaderStatus); DebugShader(HUDFragShader);
	}
#endif

    GLuint HUDShaderProgram = glCreateProgram();
    glAttachShader(HUDShaderProgram, HUDVertShader);
    glAttachShader(HUDShaderProgram, HUDFragShader);
    glBindFragDataLocation(HUDShaderProgram, 0, "FragColor");
    glLinkProgram(HUDShaderProgram);
    glUseProgram(HUDShaderProgram);

    /*--------------- Text Setup ---------------*/

    float text_vertices[]{
            //  X      Y
            0.00f, 0.00f, // Bottom - Left  - 0
            0.05f, 0.00f, // Bottom - Right - 1
            0.05f, 0.10f, // Top    - Right - 2
            0.00f, 0.10f  // Top    - Left  - 3
    };

    int8 text_tex_coords[]{
            // U  V
            0, 0, // Bottom - Left  - 0
            7, 0, // Bottom - Right - 1
            7, 9, // Top    - Right - 2
            0, 9  // Top    - Left  - 3
    };

    GLuint text_elements[]{
            0,1,2,
            2,3,0
    };

    GLuint TEXT_VAO;
    glGenVertexArrays(1, &TEXT_VAO);
    glBindVertexArray(TEXT_VAO);

#define TEXT_RENDER_MEM_SIZE sizeof(text_vertices) + sizeof(text_tex_coords) + (TEXT_MEM_PAGES * 4096)

    GLuint Text_VBO;
    glGenBuffers(1, &Text_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Text_VBO);
    glBufferData(GL_ARRAY_BUFFER, TEXT_RENDER_MEM_SIZE, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(text_vertices), text_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(text_vertices), sizeof(text_tex_coords), text_tex_coords);

    GLuint Text_EBO;
    glGenBuffers(1, &Text_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Text_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(text_elements), text_elements, GL_STATIC_DRAW);

    {
        GLint TextPosAttrib = 0;
        glVertexAttribPointer(TextPosAttrib, 2, GL_FLOAT, GL_FALSE, NULL, (void*)(0));
        glEnableVertexAttribArray(TextPosAttrib);

        GLint TextTexAttrib = 1;
        glVertexAttribPointer(TextTexAttrib, 2, GL_UNSIGNED_BYTE, GL_TRUE, NULL, (void*)(sizeof(text_vertices)));
        glEnableVertexAttribArray(TextTexAttrib);

        GLint TextPosOffsetAttrib = 2;
        glVertexAttribPointer(TextPosOffsetAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Text_Renderable), (void*)((sizeof(text_vertices)) + sizeof(text_tex_coords)));
        glVertexAttribDivisor(TextPosOffsetAttrib, 1);
        glEnableVertexAttribArray(TextPosOffsetAttrib);

        GLint TextTexOffsetAttrib = 3;
        glVertexAttribPointer(TextTexOffsetAttrib, 2, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Text_Renderable), (void*)((sizeof(text_vertices)) + sizeof(text_tex_coords) + sizeof(vec2)));
        glVertexAttribDivisor(TextTexOffsetAttrib, 1);
        glEnableVertexAttribArray(TextTexOffsetAttrib);
    }

    GLuint TextVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(TextVertShader, 1, &TextVertSource, NULL);
    glCompileShader(TextVertShader);

    GLuint TextFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(TextFragShader, 1, &TextFragSource, NULL);
    glCompileShader(TextFragShader);

#if 0 // shrink both macros into one?
    {
		GLint ShaderStatus;
		glGetShaderiv(TextVertShader, GL_COMPILE_STATUS, &ShaderStatus);
		out("Text Vert shader status: " << ShaderStatus);
		glGetShaderiv(TextFragShader, GL_COMPILE_STATUS, &ShaderStatus);
		out("Text Frag shader status: " << ShaderStatus);
	} //DebugShader(TextVertShader);
#endif

    GLuint TextShaderProgram = glCreateProgram();
    glAttachShader(TextShaderProgram, TextVertShader);
    glAttachShader(TextShaderProgram, TextFragShader);
    glBindFragDataLocation(TextShaderProgram, 0, "FragColor");
    glLinkProgram(TextShaderProgram);
    glUseProgram(TextShaderProgram);

    GLint FontTexSampler = glGetUniformLocation(TextShaderProgram, "FontTextures");
    glUniform1i(FontTexSampler, 0);

    /*---------------- 3D Stuff ----------------*/
#define FOV ToRadians(45.0f)
#define RENDER_DISTANCE 64.0f

    // Creating  Model, View, and Projection Matrices
    mat4 proj = glm::perspective(FOV, 1920.0f / 1080.0f, 0.1f, RENDER_DISTANCE); //Last parameter is render distance in blocks
    mat4 model = mat4(1.0f);
    mat4 view = glm::lookAt(
            vec3(2.5f, 2.5f, 2.0f),
            vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 0.0f, 1.0f)
    );

    /*--------------- World Setup ---------------*/

    float cube_vertices[]{
            // X     Y      Z     U     V
            0.0f,  1.0f,  1.0f, 0.0f, 1.0f, // Left  - Top - Near - 0
            0.0f,  1.0f,  0.0f, .25f, 1.0f, // Left  - Top - Far  - 1
            1.0f,  1.0f,  0.0f, 0.0f, 1.0f, // Right - Top - Far  - 2
            1.0f,  1.0f,  1.0f, .25f, 1.0f, // Right - Top - Near - 3

            0.0f,  0.0f,  1.0f, 0.0f, .75f, // Left  - Bottom - Near - 4
            0.0f,  0.0f,  0.0f, .25f, .75f, // Left  - Bottom - Far  - 5
            1.0f,  0.0f,  0.0f, 0.0f, .75f, // Right - Bottom - Far  - 6
            1.0f,  0.0f,  1.0f, .25f, .75f, // Right - Bottom - Near - 7

            //Duplicates for textures
            0.0f,  1.0f,  1.0f, .25f, .75f, // Left  - Top - Near - 8
            1.0f,  1.0f,  1.0f, 0.0f, .75f, // Right - Top - Near - 9

            1.0f,  0.0f,  0.0f, .25f, 1.0f, // Right - Bottom - Far  - 10
            0.0f,  0.0f,  0.0f, 0.0f, 1.0f, // Left  - Bottom - Far  - 11
    };

    //Element Buffer (Counter-Clockwise)
    GLuint cube_elements[]{
            4, 7, 3, //Near 1
            3, 0, 4, //Near 2

            5, 6, 2, //Far 1
            2, 1, 5, //Far 2

            8, 9, 2, //Top 1
            2, 1, 8, //Top 2

            4, 7, 10, //Bottom 1
            10, 11, 4, //Bottom 2

            4, 0, 1, //Left 1
            1, 5, 4, //Left 2

            7, 6, 2, //Right 1
            2, 3, 7  //Right 2
    };

    GLuint WORLD_VAO;
    glGenVertexArrays(1, &WORLD_VAO);
    glBindVertexArray(WORLD_VAO);

#define CUBE_RENDER_MEM_SIZE sizeof(cube_vertices) + (sizeof(Chunk_Renderable) * 4096 * NUM_CHUNKS)

    GLuint World_VBO;
    glGenBuffers(1, &World_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, World_VBO);
    glBufferData(GL_ARRAY_BUFFER, CUBE_RENDER_MEM_SIZE, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_vertices), cube_vertices);

    GLuint World_EBO;
    glGenBuffers(1, &World_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, World_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

    {
        GLint PosAttrib = 0;
        glVertexAttribPointer(PosAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(PosAttrib);
        GLint WorldTexAttrib = 1;
        glEnableVertexAttribArray(WorldTexAttrib);
        glVertexAttribPointer(WorldTexAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

        GLint PosOffsetAttrib = 2;
        glVertexAttribPointer(PosOffsetAttrib, 3, GL_FLOAT, GL_FALSE, (sizeof(Chunk_Renderable)), (void*)(sizeof(cube_vertices) + sizeof(uint)));
        glVertexAttribDivisor(PosOffsetAttrib, 1);
        glEnableVertexAttribArray(PosOffsetAttrib);
        GLint TexOffsetAttrib = 3;
        glVertexAttribPointer(TexOffsetAttrib, 2, GL_FLOAT, GL_FALSE, (sizeof(Chunk_Renderable)), (void*)(sizeof(cube_vertices) + sizeof(uint) + sizeof(vec3)));
        glVertexAttribDivisor(TexOffsetAttrib, 1);
        glEnableVertexAttribArray(TexOffsetAttrib);
    }

    GLuint WorldVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(WorldVertShader, 1, &WorldVertSource, NULL);
    glCompileShader(WorldVertShader);

    GLuint WorldFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(WorldFragShader, 1, &WorldFragSource, NULL);
    glCompileShader(WorldFragShader);

#if 0
    {
		GLint ShaderStatus;
		glGetShaderiv(WorldVertShader, GL_COMPILE_STATUS, &ShaderStatus);
		out("World vert shader status: " << ShaderStatus);
		glGetShaderiv(WorldFragShader, GL_COMPILE_STATUS, &ShaderStatus);
		out("World Frag shader status: " << ShaderStatus);
	}
#endif

    GLuint WorldShaderProgram = glCreateProgram();
    glAttachShader(WorldShaderProgram, WorldVertShader);
    glAttachShader(WorldShaderProgram, WorldFragShader);
    glBindFragDataLocation(WorldShaderProgram, 0, "FragColor");
    glLinkProgram(WorldShaderProgram);
    glUseProgram(WorldShaderProgram);

    GLint WorldProjMatrix = glGetUniformLocation(WorldShaderProgram, "Proj");
    GLint WorldViewMatrix = glGetUniformLocation(WorldShaderProgram, "View");
    GLint WorldTexSampler = glGetUniformLocation(WorldShaderProgram, "WorldTextures");

    glUniformMatrix4fv(WorldProjMatrix, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(WorldViewMatrix, 1, GL_FALSE, glm::value_ptr(view));
    glUniform1i(WorldTexSampler, 1);

    /*------------ Projectile Setup ------------*/

    float projectile_vertices[]{
            // X     Y      Z
            0.0f,  .25f,  .25f, // Left  - Top - Near - 0
            0.0f,  .25f,  0.0f, // Left  - Top - Far  - 1
            .25f,  .25f,  0.0f, // Right - Top - Far  - 2
            .25f,  .25f,  .25f, // Right - Top - Near - 3

            0.0f,  0.0f,  .25f, // Left  - Bottom - Near - 4
            0.0f,  0.0f,  0.0f, // Left  - Bottom - Far  - 5
            .25f,  0.0f,  0.0f, // Right - Bottom - Far  - 6
            .25f,  0.0f,  .25f  // Right - Bottom - Near - 7
    };

    GLuint projectile_elements[]{
            4, 7, 3, //Near 1
            3, 0, 4, //Near 2

            5, 6, 2, //Far 1
            2, 1, 5, //Far 2

            0, 3, 2, //Top 1
            2, 1, 0, //Top 2

            4, 7, 6, //Bottom 1
            6, 5, 4, //Bottom 2

            4, 0, 1, //Left 1
            1, 5, 4, //Left 2

            7, 6, 2, //Right 1
            2, 3, 7  //Right 2
    };

    GLuint PROJECTILE_VAO;
    glGenVertexArrays(1, &PROJECTILE_VAO);
    glBindVertexArray(PROJECTILE_VAO);

#define PHYSX_RENDER_MEM_SIZE sizeof(projectile_vertices) + (sizeof(vec3) * NUM_PROJECTILES)
#define MAX_PROJECTILES 256
#define NUM_PROJECTILES 2

    GLuint Projectile_VBO;
    glGenBuffers(1, &Projectile_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Projectile_VBO);
    glBufferData(GL_ARRAY_BUFFER, PHYSX_RENDER_MEM_SIZE, NULL, GL_DYNAMIC_DRAW);//fix vec3->projectile
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(projectile_vertices), projectile_vertices);

    GLuint Projectile_EBO;
    glGenBuffers(1, &Projectile_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Projectile_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(projectile_elements), projectile_elements, GL_DYNAMIC_DRAW);

    {
        GLint PosAttrib = 0;
        glVertexAttribPointer(PosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
        glEnableVertexAttribArray(PosAttrib);

        GLint OffsetAttrib = 1;
        glVertexAttribPointer(OffsetAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)(sizeof(projectile_vertices)));
        glVertexAttribDivisor(OffsetAttrib, 1);
        glEnableVertexAttribArray(OffsetAttrib);
    }

    GLuint ProjectileVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ProjectileVertShader, 1, &ProjectiletVertSource, NULL);
    glCompileShader(ProjectileVertShader);

    GLuint ProjectileFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(ProjectileFragShader, 1, &ProjectileFragSource, NULL);
    glCompileShader(ProjectileFragShader);

#if 0
    {
		GLint ShaderStatus;
		glGetShaderiv(ProjectileVertShader, GL_COMPILE_STATUS, &ShaderStatus);
		out("Projectile vert shader status: " << ShaderStatus);
		glGetShaderiv(ProjectileFragShader, GL_COMPILE_STATUS, &ShaderStatus);
		out("Projectile Frag shader status: " << ShaderStatus);
	}
#endif

    GLuint ProjectileShaderProgram = glCreateProgram();
    glAttachShader(ProjectileShaderProgram, ProjectileVertShader);
    glAttachShader(ProjectileShaderProgram, ProjectileFragShader);
    glBindFragDataLocation(ProjectileShaderProgram, 0, "FragColor");
    glLinkProgram(ProjectileShaderProgram);
    glUseProgram(ProjectileShaderProgram);

    GLint ProjectileProjMatrix = glGetUniformLocation(ProjectileShaderProgram, "Proj");
    GLint ProjectileViewMatrix = glGetUniformLocation(ProjectileShaderProgram, "View");

    glUniformMatrix4fv(ProjectileProjMatrix, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(ProjectileViewMatrix, 1, GL_FALSE, glm::value_ptr(view));

    /*---------------- Enemy Setup ----------------*/

    float enemy_vertices[]{
            // X     Y      Z     U     V
            0.0f,  2.0f,  1.0f, 0.0f, 1.0f, // Left  - Top - Near - 0
            0.0f,  2.0f,  0.0f, 1.0f, 1.0f, // Left  - Top - Far  - 1
            1.0f,  2.0f,  0.0f, 0.0f, 1.0f, // Right - Top - Far  - 2
            1.0f,  2.0f,  1.0f, 1.0f, 1.0f, // Right - Top - Near - 3

            0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // Left  - Bottom - Near - 4
            0.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Left  - Bottom - Far  - 5
            1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // Right - Bottom - Far  - 6
            1.0f,  0.0f,  1.0f, 1.0f, 0.0f, // Right - Bottom - Near - 7

            //Duplicates for textures
            0.0f,  2.0f,  1.0f, 1.0f, 0.0f, // Left  - Top - Near - 8
            1.0f,  2.0f,  1.0f, 0.0f, 0.0f, // Right - Top - Near - 9

            1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // Right - Bottom - Far  - 10
            0.0f,  0.0f,  0.0f, 0.0f, 1.0f, // Left  - Bottom - Far  - 11
    };

    GLuint enemy_elements[]{
            4, 7, 3, //Near 1
            3, 0, 4, //Near 2

            5, 6, 2, //Far 1
            2, 1, 5, //Far 2

            8, 9, 2, //Top 1
            2, 1, 8, //Top 2

            4, 7, 10, //Bottom 1
            0, 11, 4, //Bottom 2

            4, 0, 1, //Left 1
            1, 5, 4, //Left 2

            7, 6, 2, //Right 1
            2, 3, 7  //Right 2
    };

    GLuint ENEMY_VAO;
    glGenVertexArrays(1, &ENEMY_VAO);
    glBindVertexArray(ENEMY_VAO);

#define MAX_ENEMIES 16
#define ENEMY_RENDER_MEM_SIZE sizeof(enemy_vertices) + (sizeof(Enemy_Renderable) * MAX_ENEMIES)

    GLuint Enemy_VBO;
    glGenBuffers(1, &Enemy_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Enemy_VBO);
    glBufferData(GL_ARRAY_BUFFER, ENEMY_RENDER_MEM_SIZE, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(enemy_vertices), enemy_vertices);

    GLuint Enemy_EBO;
    glGenBuffers(1, &Enemy_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Enemy_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(enemy_elements), enemy_elements, GL_STATIC_DRAW);

    {
        GLuint EnemyPosAttrib = 0;
        glVertexAttribPointer(EnemyPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec2), 0);
        glEnableVertexAttribArray(EnemyPosAttrib);
        GLuint EnemyTexAttrib = 1;
        glVertexAttribPointer(EnemyTexAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec2), (void*)(sizeof(vec3)));
        glEnableVertexAttribArray(EnemyTexAttrib);

        GLuint EnemyOffsetAttrib = 2;
        glVertexAttribPointer(EnemyOffsetAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Enemy_Renderable), (void*)(sizeof(enemy_vertices)));
        glVertexAttribDivisor(EnemyOffsetAttrib, 1);
        glEnableVertexAttribArray(EnemyOffsetAttrib);
    }

    GLuint EnemyVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(EnemyVertShader, 1, &EnemyVertSource, NULL);
    glCompileShader(EnemyVertShader);

    GLuint EnemyFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(EnemyFragShader, 1, &EnemyFragSource, NULL);
    glCompileShader(EnemyFragShader);

    GLuint EnemyShaderProgram = glCreateProgram();
    glAttachShader(EnemyShaderProgram, EnemyVertShader);
    glAttachShader(EnemyShaderProgram, EnemyFragShader);
    glBindFragDataLocation(EnemyShaderProgram, 0, "FragColor");
    glLinkProgram(EnemyShaderProgram);
    glUseProgram(EnemyShaderProgram);

    GLint EnemyProj = glGetUniformLocation(EnemyShaderProgram, "Proj");
    GLint EnemyView = glGetUniformLocation(EnemyShaderProgram, "View");
    GLint EnemyTexSampler = glGetUniformLocation(EnemyShaderProgram, "EnemyTextures");

    glUniformMatrix4fv(EnemyProj, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(EnemyView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform1i(EnemyTexSampler, 2);

    /*--------------- Water Setup ---------------*/

    float water_vertices[]{
            // X     Y      Z
            0.0f,  1.0f,  0.2f, // Left  - Near - 0
            0.2f,  1.0f,  0.2f, // Right - Near - 1
            0.2f,  1.0f,  0.0f, // Right - Far  - 2
            0.0f,  1.0f,  0.0f, // Left  - Far  - 3

            0.0f,  1.0f,  0.4f, // Left  - Far  - 4
            0.2f,  1.0f,  0.4f, // Right - Far  - 5
            //0.2f,  1.0f,  0.2f, // Right - Near - 1
            //0.0f,  1.0f,  0.2f, // Left  - Near - 0

            0.0f,  1.0f,  0.6f, // Left  - Far  - 6
            0.2f,  1.0f,  0.6f, // Right - Far  - 7
            //0.2f,  1.0f,  0.4f, // Right - Near - 5
            //0.0f,  1.0f,  0.4f, // Left  - Near - 4

            0.0f,  1.0f,  0.8f, // Left  - Far  - 8
            0.2f,  1.0f,  0.8f, // Right - Far  - 9
            //0.2f,  1.0f,  0.6f, // Right - Near - 7
            //0.0f,  1.0f,  0.6f, // Left  - Near - 6

            0.0f,  1.0f,  1.0f, // Left  - Far  - 10
            0.2f,  1.0f,  1.0f, // Right - Far  - 11
            //0.2f,  1.0f,  0.8f, // Right - Near - 9
            //0.0f,  1.0f,  0.8f, // Left  - Near - 8

            //Column 2 --------------------------------
            //0.2f,  1.0f,  0.2f, // Left  - Near - 1
            0.4f,  1.0f,  0.2f, // Right - Near - 12
            0.4f,  1.0f,  0.0f, // Right - Far  - 13
            //0.2f,  1.0f,  0.0f, // Left  - Far  - 2

            //0.2f,  1.0f,  0.4f, // Left  - Far  - 5
            0.4f,  1.0f,  0.4f, // Right - Far  - 14
            //0.4f,  1.0f,  0.2f, // Right - Near - 12
            //0.2f,  1.0f,  0.2f, // Left  - Near - 1

            //0.2f,  1.0f,  0.6f, // Left  - Far  - 7
            0.4f,  1.0f,  0.6f, // Right - Far  - 15
            //0.4f,  1.0f,  0.4f, // Right - Near - 14
            //0.2f,  1.0f,  0.4f, // Left  - Near - 5

            //0.2f,  1.0f,  0.8f, // Left  - Far  - 9
            0.4f,  1.0f,  0.8f, // Right - Far  - 16
            //0.4f,  1.0f,  0.6f, // Right - Near - 15
            //0.2f,  1.0f,  0.6f, // Left  - Near - 7

            //0.2f,  1.0f,  1.0f, // Left  - Far  - 11
            0.4f,  1.0f,  1.0f, // Right - Far  - 17
            //0.4f,  1.0f,  0.8f, // Right - Near - 16
            //0.2f,  1.0f,  0.8f, // Left  - Near - 9

            //Column 3 --------------------------------
            //0.4f,  1.0f,  0.2f, // Left  - Near - 12
            0.6f,  1.0f,  0.2f, // Right - Near - 18
            0.6f,  1.0f,  0.0f, // Right - Far  - 19
            //0.4f,  1.0f,  0.0f, // Left  - Far  - 13

            //0.4f,  1.0f,  0.4f, // Left  - Far  - 14
            0.6f,  1.0f,  0.4f, // Right - Far  - 20
            //0.6f,  1.0f,  0.2f, // Right - Near - 18
            //0.4f,  1.0f,  0.2f, // Left  - Near - 12

            //0.4f,  1.0f,  0.6f, // Left  - Far  - 15
            0.6f,  1.0f,  0.6f, // Right - Far  - 21
            //0.6f,  1.0f,  0.4f, // Right - Near - 20
            //0.4f,  1.0f,  0.4f, // Left  - Near - 14

            //0.4f,  1.0f,  0.8f, // Left  - Far  - 16
            0.6f,  1.0f,  0.8f, // Right - Far  - 22
            //0.6f,  1.0f,  0.6f, // Right - Near - 21
            //0.4f,  1.0f,  0.6f, // Left  - Near - 15

            //0.4f,  1.0f,  1.0f, // Left  - Far  - 17
            0.6f,  1.0f,  1.0f, // Right - Far  - 23
            //0.6f,  1.0f,  0.8f, // Right - Near - 22
            //0.4f,  1.0f,  0.8f, // Left  - Near - 16

            //Column 4 --------------------------------
            //0.6f,  1.0f,  0.2f, // Left  - Near - 18
            0.8f,  1.0f,  0.2f, // Right - Near - 24
            0.8f,  1.0f,  0.0f, // Right - Far  - 25
            //0.6f,  1.0f,  0.0f, // Left  - Far  - 19

            //0.6f,  1.0f,  0.4f, // Left  - Far  - 20
            0.8f,  1.0f,  0.4f, // Right - Far  - 26
            //0.8f,  1.0f,  0.2f, // Right - Near - 24
            //0.6f,  1.0f,  0.2f, // Left  - Near - 18

            //0.6f,  1.0f,  0.6f, // Left  - Far  - 21
            0.8f,  1.0f,  0.6f, // Right - Far  - 27
            //0.8f,  1.0f,  0.4f, // Right - Near - 26
            //0.6f,  1.0f,  0.4f, // Left  - Near - 20

            //0.6f,  1.0f,  0.8f, // Left  - Far  - 22
            0.8f,  1.0f,  0.8f, // Right - Far  - 28
            //0.8f,  1.0f,  0.6f, // Right - Near - 27
            //0.6f,  1.0f,  0.6f, // Left  - Near - 21

            //0.6f,  1.0f,  1.0f, // Left  - Far  - 23
            0.8f,  1.0f,  1.0f, // Right - Far  - 29
            //0.8f,  1.0f,  0.8f, // Right - Near - 28
            //0.6f,  1.0f,  0.8f, // Left  - Near - 22

            //Column 5 --------------------------------
            //0.8f,  1.0f,  0.2f, // Left  - Near - 24
            1.0f,  1.0f,  0.2f, // Right - Near - 30
            1.0f,  1.0f,  0.0f, // Right - Far  - 31
            //0.8f,  1.0f,  0.0f, // Left  - Far  - 25

            //0.8f,  1.0f,  0.4f, // Left  - Far  - 26
            1.0f,  1.0f,  0.4f, // Right - Far  - 32
            //1.0f,  1.0f,  0.2f, // Right - Near - 30
            //0.8f,  1.0f,  0.2f, // Left  - Near - 24

            //0.8f,  1.0f,  0.6f, // Left  - Far  - 27
            1.0f,  1.0f,  0.6f, // Right - Far  - 33
            //1.0f,  1.0f,  0.4f, // Right - Near - 32
            //0.8f,  1.0f,  0.4f, // Left  - Near - 26

            //0.8f,  1.0f,  0.8f, // Left  - Far  - 28
            1.0f,  1.0f,  0.8f, // Right - Far  - 34
            //1.0f,  1.0f,  0.6f, // Right - Near - 33
            //0.8f,  1.0f,  0.6f, // Left  - Near - 27

            //0.8f,  1.0f,  1.0f, // Left  - Far  - 29
            1.0f,  1.0f,  1.0f, // Right - Far  - 35
            //1.0f,  1.0f,  0.8f, // Right - Near - 34
            //0.8f,  1.0f,  0.8f, // Left  - Near - 28
    };

    //Element Buffer (Counter-Clockwise)
    GLuint water_elements[]{
            /* --- column 1 --- */
            0, 1, 2,
            2, 3, 0,

            4, 5, 1,
            1, 0, 4,

            6, 7, 5,
            5, 4, 6,

            8, 9, 7,
            7, 6, 8,

            10,11,9,
            9, 8,10,

            /* --- column 2 --- */
            1,12,13,
            13,2, 1,

            5,14,12,
            12, 1, 5,

            7,15,14,
            14, 5,7,

            9,16,15,
            15, 7, 9,

            11,17,16,
            16, 9,11,

            /* --- column 3 --- */
            12,18,19,
            19,13,12,

            14,20,18,
            18,12,14,

            15,21,20,
            20,14,15,

            16,22,21,
            21,15,16,

            17,23,22,
            22,16,17,

            /* --- column 4 --- */
            18,24,25,
            25,19,18,

            20,26,24,
            24,18,20,

            21,27,26,
            26,20,21,

            22,28,27,
            27,21,22,

            23,29,28,
            28,22,23,

            /* --- column 5 --- */
            24,30,31,
            31,25,24,

            26,32,30,
            30,24,26,

            27,33,32,
            32,26,27,

            28,34,33,
            33,27,28,

            29,35,34,
            34,28,29,
    };

    GLuint WATER_VAO;
    glGenVertexArrays(1, &WATER_VAO);
    glBindVertexArray(WATER_VAO);

#define NUM_WATER 8
#define WATER_RENDER_MEM_SIZE sizeof(water_vertices) + ( sizeof(vec3) * NUM_WATER)

    GLuint Water_VBO;
    glGenBuffers(1, &Water_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Water_VBO);
    glBufferData(GL_ARRAY_BUFFER, WATER_RENDER_MEM_SIZE, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(water_vertices), water_vertices);

    GLuint Water_EBO;
    glGenBuffers(1, &Water_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Water_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(water_elements), water_elements, GL_DYNAMIC_DRAW);

    {
        GLint PosAttrib = 0;
        glVertexAttribPointer(PosAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(PosAttrib);

        GLint OffsetAttrib = 1;
        glVertexAttribPointer(OffsetAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(water_vertices)));
        glVertexAttribDivisor(OffsetAttrib, 1);
        glEnableVertexAttribArray(OffsetAttrib);
    }

    GLuint WaterVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(WaterVertShader, 1, &WaterVertSource, NULL);
    glCompileShader(WaterVertShader);

    GLuint WaterFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(WaterFragShader, 1, &WaterFragSource, NULL);
    glCompileShader(WaterFragShader);

#if 1
    {
        GLint ShaderStatus;
        glGetShaderiv(WaterVertShader, GL_COMPILE_STATUS, &ShaderStatus);
        out("Water vert shader status: " << ShaderStatus);
        glGetShaderiv(WaterFragShader, GL_COMPILE_STATUS, &ShaderStatus);
        out("Water Frag shader status: " << ShaderStatus);
    } DebugShader(WaterVertShader);
#endif

    GLuint WaterShaderProgram = glCreateProgram();
    glAttachShader(WaterShaderProgram, WaterVertShader);
    glAttachShader(WaterShaderProgram, WaterFragShader);
    glBindFragDataLocation(WaterShaderProgram, 0, "FragColor");
    glLinkProgram(WaterShaderProgram);
    glUseProgram(WaterShaderProgram);

    GLint WaterProjMatrix = glGetUniformLocation(WaterShaderProgram, "Proj");
    GLint WaterViewMatrix = glGetUniformLocation(WaterShaderProgram, "View");
    GLint WaterTimer = glGetUniformLocation(WaterShaderProgram, "Timer");

    glUniformMatrix4fv(WaterProjMatrix, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(WaterViewMatrix, 1, GL_FALSE, glm::value_ptr(view));

    /* ------------ Textures ------------ */

    GLuint FontBitmap;
    GLuint WorldTextureAtlas;
    GLuint EnemyTexture;

    {
        int Width, Height, NumChannels;
        uint8* Image;
        stbi_set_flip_vertically_on_load(true);

        Image = stbi_load("textures/Font.bmp", &Width, &Height, &NumChannels, 0);

        glGenTextures(1, &FontBitmap);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FontBitmap);
        glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Image = stbi_load("textures/Texture_Atlas.bmp", &Width, &Height, &NumChannels, 0);

        glGenTextures(1, &WorldTextureAtlas);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, WorldTextureAtlas);
        glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Image = stbi_load("textures/Noggin.png", &Width, &Height, &NumChannels, 0);

        glGenTextures(1, &EnemyTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, EnemyTexture);
        glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(Image);
    }

    /*--------------- HUD & Text ---------------*/
    //Hide text elements by moving them off-screen "1234567890123456"

    uint TotalChars = 0;
    Text_Renderable* TextMemory = (Text_Renderable*)MemoryManager.TextPointers[0];

    uint TotalQuads = 0;
    Quad_Renderable* HUDMemory = (Quad_Renderable*)MemoryManager.HUDPointers[0];
    ;

    Quad HealthBar = {};
    QuadInit(&HealthBar, vec2(-.785, -.625), vec2(27.25, 1), vec3(1, 0, 0), &TotalQuads);
    QuadToRenderable(&HealthBar, HUDMemory);

    Text_String HealthText = {};
    TextStringInit(&HealthText, vec2(-.785, -.625), 10, "Health 100");
    TextToRenderable(&HealthText, TextMemory, &TotalChars);

    Quad MousePointer = {};
    QuadInit(&MousePointer, vec2(0, 0), vec2(.5, .5), vec3(0, 1, 1), &TotalQuads);
    QuadToRenderable(&MousePointer, HUDMemory);

    InventoryHUD Inventory;
    PlayerHUDInit(&Inventory, &TotalQuads);
    PlayerHUDToRenderable(&Inventory, HUDMemory);

    Crosshair crosshair;
    CrosshairInit(&crosshair, vec3(.6, .2, .2), &TotalQuads);
    CrosshairToRenderable(&crosshair, HUDMemory);

    //Button test;
    //ButtonInit(&test, "test", 4, vec2(0, 0), vec3(.5, .1, .1), &TotalQuads);
    //ButtonToRenderable(&test, &TotalChars, HUDMemory, TextMemory);

    /*--------------- Testing Grounds ---------------*/

    Chunk TestChunk;
    ChunkInit(&TestChunk, 0, 0, GameMemory);
    Chunk_Renderable* ChunkTemp = new Chunk_Renderable[4096];

    int BlockType = DIRT_ID; // For BlockType Selection

    //Testing out projectiles
    vec3 Projectiles[MAX_PROJECTILES];
    Projectiles[0] = vec3(0,0,0);
    Projectiles[1] = vec3(7, 15, 7);

    Projectile Bullet;
    ProjectileInit(&Bullet);

    Enemy_Renderable enemy_positions[MAX_ENEMIES];
    for (uint i = 0; i < MAX_ENEMIES; ++i)
    {
        enemy_positions[i].Position.y = (float)13;
        enemy_positions[i].Position.x = (float)(__rdtsc() % 15);
        enemy_positions[i].Position.z = (float)(__rdtsc() % 13);
    }

    bool FirstPress = true; // need one for every key
    bool FirstPress2 = true; // need one for every key
    /*--------------- Misc, Relocate ---------------*/

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //camera.Position.x + camera.Front.x, camera.Position.y + camera.Front.y, camera.Position.z + camera.Front.z

    //Initialize Objects (just the camera for now)
    CameraInit(&camera);

    /*--------------- Timing Logic ---------------*/

    //Get CPU clock frequency for Timing
    int64 PerformanceFrequency;
    {
        long long int win32_PerformanceFrequency;
        QueryPerformanceFrequency(&win32_PerformanceFrequency);
        PerformanceFrequency = (int64)win32_PerformanceFrequency.QuadPart;
    }

#define TARGET_FRAMERATE (float)120;
    float TargetFrameTime = 1 / TARGET_FRAMERATE;
    float DeltaTime = TargetFrameTime;

    long long int StartCounter;
    long long int EndCounter;

#define DESIRED_SCHEDULER_GRANULARITY 1 //milliseconds
    HRESULT SchedulerResult = timeBeginPeriod(DESIRED_SCHEDULER_GRANULARITY);

    QueryPerformanceCounter(&StartCounter);

    float timer[1] = {};

    /*-------------------------------------------------------------- Main Game Loop --------------------------------------------------------------*/
    while (!glfwWindowShouldClose(WindowInstance))
    {
        /*------------------------------- Input -------------------------------*/
        double MouseXPos, MouseYPos;
        glfwGetCursorPos(WindowInstance, &MouseXPos, &MouseYPos);
        MouseXPos = ((uint)MouseXPos % SCREEN_WIDTH)  / (double)SCREEN_WIDTH;
        MouseYPos = ((uint)MouseYPos % SCREEN_HEIGHT) / (double)SCREEN_HEIGHT;

        MouseYPos = 1 - MouseYPos;
        MouseXPos *= 2;	MouseXPos += -1; //out(MouseXPos);
        MouseYPos *= 2;	MouseYPos += -1; //out(MouseYPos);

        if (Paused)
        {
            QuadUpdatePos(MousePointer.ID, vec2(MouseXPos, MouseYPos), HUDMemory);
        } else QuadUpdatePos(MousePointer.ID, vec2(1000, 1000), HUDMemory);

        //Pausing
        if (glfwGetKey(WindowInstance, GLFW_KEY_P) == GLFW_PRESS) Paused = true;
        if (glfwGetKey(WindowInstance, GLFW_KEY_O) == GLFW_PRESS) Paused = false;

        //Movement
        (glfwGetKey(WindowInstance, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? camera.MovementSpeed = 9.0f : camera.MovementSpeed = 3.0f;

        if (glfwGetKey(WindowInstance, GLFW_KEY_W) == GLFW_PRESS) CameraProcessKeyboard(&camera, CAM_FORWARD  , DeltaTime);
        if (glfwGetKey(WindowInstance, GLFW_KEY_S) == GLFW_PRESS) CameraProcessKeyboard(&camera, CAM_BACKWARD , DeltaTime);
        if (glfwGetKey(WindowInstance, GLFW_KEY_A) == GLFW_PRESS) CameraProcessKeyboard(&camera, CAM_LEFT	  , DeltaTime);
        if (glfwGetKey(WindowInstance, GLFW_KEY_D) == GLFW_PRESS) CameraProcessKeyboard(&camera, CAM_RIGHT    , DeltaTime);

        //Block type selection
        if (glfwGetKey(WindowInstance, GLFW_KEY_1) == GLFW_PRESS) BlockType = STONE_ID;
        if (glfwGetKey(WindowInstance, GLFW_KEY_2) == GLFW_PRESS) BlockType = DIRT_ID;
        if (glfwGetKey(WindowInstance, GLFW_KEY_3) == GLFW_PRESS) BlockType = GRASS_ID;
        if (glfwGetKey(WindowInstance, GLFW_KEY_4) == GLFW_PRESS) BlockType = SAND_ID;
        if (glfwGetKey(WindowInstance, GLFW_KEY_5) == GLFW_PRESS) BlockType = WATER_ID;
        if (glfwGetKey(WindowInstance, GLFW_KEY_6) == GLFW_PRESS) BlockType = WOOD_ID;
        if (glfwGetKey(WindowInstance, GLFW_KEY_7) == GLFW_PRESS) BlockType = CLAY_BRICK_ID;
        if (glfwGetKey(WindowInstance, GLFW_KEY_8) == GLFW_PRESS) BlockType = STONE_BRICK_ID;
        if (glfwGetKey(WindowInstance, GLFW_KEY_9) == GLFW_PRESS) BlockType = COAL_ORE_ID;

        if (glfwGetKey(WindowInstance, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(WindowInstance, GL_TRUE);

        if (glfwGetKey(WindowInstance, GLFW_KEY_E) == GLFW_PRESS) //Placing a block
        {
            if (FirstPress)
            {
                vec3 Increment = vec3(.2, .2, .2) * glm::normalize(camera.Front);
                vec3 TestPoint = camera.Position + Increment;

                float IncrementLength = glm::length(Increment);
                float TestDistance = IncrementLength;

                bool32 Collision = false;
                do
                {
                    int Test_x = (int)TestPoint.x;
                    int Test_z = (int)TestPoint.z;
                    int Test_y = (int)TestPoint.y;

                    //TODO Remove these
                    if (Test_x > 16) Test_x = 16;
                    if (Test_y > 16) Test_y = 16;
                    if (Test_z > 16) Test_z = 16;

                    if (Test_x < 0)  Test_x = 0;
                    if (Test_y < 0)  Test_y = 0;
                    if (Test_z < 0)  Test_z = 0;

                    if (TestChunk.Blocks[(Test_x + (16 * Test_z)) + (256 * Test_y)])
                    {
                        Collision = true;
                        Test_x = (int)(TestPoint.x - Increment.x);
                        Test_z = (int)(TestPoint.z - Increment.z);
                        Test_y = (int)(TestPoint.y - Increment.y);

                        TestChunk.Blocks[(Test_x + (16 * Test_z)) + (256 * Test_y)] = BlockType;
                    }

                    TestDistance += IncrementLength;
                    TestPoint += Increment;

                    if (TestDistance > 3.6) Collision = true;

                } while (!Collision);

                out("first time");
                FirstPress = false;

            }
            else
            {
                out("continuous");
            }
        }
        else FirstPress = true;

        if (glfwGetKey(WindowInstance, GLFW_KEY_F) == GLFW_PRESS) //Digging a block
        {
            if (FirstPress2)
            {
                vec3 Increment = vec3(.2, .2, .2) * glm::normalize(camera.Front);
                vec3 TestPoint = camera.Position + Increment;

                float IncrementLength = glm::length(Increment);
                float TestDistance = IncrementLength;

                bool32 Collision = false;
                do
                {
                    int Test_x = (int)TestPoint.x;
                    int Test_z = (int)TestPoint.z;
                    int Test_y = (int)TestPoint.y;

                    //TODO Remove these
                    if (Test_x > 16) Test_x = 16;
                    if (Test_y > 16) Test_y = 16;
                    if (Test_z > 16) Test_z = 16;

                    if (Test_x < 0)  Test_x = 0;
                    if (Test_y < 0)  Test_y = 0;
                    if (Test_z < 0)  Test_z = 0;

                    if (TestChunk.Blocks[(Test_x + (16 * Test_z)) + (256 * Test_y)])
                    {
                        Collision = true;
                        TestChunk.Blocks[(Test_x + (16 * Test_z)) + (256 * Test_y)] = AIR_ID;
                        Projectiles[0] = TestPoint;
                    }

                    TestDistance += IncrementLength;
                    TestPoint += Increment;

                    if (TestDistance > 3.6)
                    {
                        Collision = true;
                        Projectiles[0] = TestPoint;
                    }

                } while (!Collision);

                out("first time");
                FirstPress2 = false;
            }
            else
            {
                out("continuous");
            }
        }
        else FirstPress2 = true;

        /*------------------------------- Enemy AI -------------------------------*/

        for (uint i = 0; i < MAX_ENEMIES; ++i)
        {
            enemy_positions[i].Position += -1.f * camera.Front * DeltaTime;
            enemy_positions[i].Position.y = (float)13;
        }

        for (int i = 0; i < MAX_ENEMIES; ++i)
        {
            Projectiles[1] = camera.Position + (glm::normalize(camera.Front) * 3.f);
            bool test = InCube(Projectiles[1], enemy_positions[i].Position, vec3(1, 2, 1));
            if (test)
            {
                enemy_positions[i].Position = vec3(100, 100, 100);
                out("killed gnome " << i);
            }
        }
        Projectiles[1] = vec3(0, 0, 0);

        /*------------------------------- Physics -------------------------------*/

        ApplyForce(&Bullet, vec3(0, GRAVITY * DeltaTime, 0));
        SimulateProjectile(&Bullet, DeltaTime);

        vec3 CollisionTest = glm::normalize(Bullet.Velocity);
        CollisionTest *= .1f;
        CollisionTest += Bullet.Position;

        if (TestChunk.Blocks[((int)CollisionTest.x + (16 * (int)CollisionTest.z)) + (256 * (int)CollisionTest.y)])
        {
            Bullet.Velocity *= -1.0f;
        }
        Projectiles[0] = Bullet.Position;

        /*------------------------------- HUD -------------------------------*/

        for (uint i = 0; i < INVENTORY_SIZE; ++i)
        {
            if (MouseInQuad(&Inventory.Items[i], MouseXPos, MouseYPos))
            {
                QuadUpdateColor(Inventory.Items[i].ID, vec3(.6, .6, .6), HUDMemory);
            }
            else
            {
                QuadUpdateColor(Inventory.Items[i].ID, vec3(.3, .3, .3), HUDMemory);
            }
        }

        /*------------------------------- Rendering -------------------------------*/

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Update Matrices
        if (!Paused)
        {
            view = CameraGetViewMatrix(&camera);
        }

        /* ----- Draw Text ----- */
        glBindVertexArray(TEXT_VAO);

        glUseProgram(TextShaderProgram);

        glBindBuffer(GL_ARRAY_BUFFER, Text_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(text_vertices) + sizeof(text_tex_coords), TotalChars * sizeof(Text_Renderable), TextMemory);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, TotalChars);

        /* ----- Draw the HUD ----- */
        glBindVertexArray(HUD_VAO);

        glUseProgram(HUDShaderProgram);

        glBindBuffer(GL_ARRAY_BUFFER, HUD_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(quad_vertices), TotalQuads * sizeof(Quad_Renderable), HUDMemory);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, TotalQuads);

        /* ----- Draw Bullets ----- */
        glBindVertexArray(PROJECTILE_VAO);

        glUseProgram(ProjectileShaderProgram);
        glUniformMatrix4fv(ProjectileViewMatrix, 1, GL_FALSE, glm::value_ptr(view));

        glBindBuffer(GL_ARRAY_BUFFER, Projectile_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(projectile_vertices), sizeof(vec3) * NUM_PROJECTILES, Projectiles);

        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, NUM_PROJECTILES);

        /* ----- Draw Enemies ----- */
        glBindVertexArray(ENEMY_VAO);

        glUseProgram(EnemyShaderProgram);
        glUniformMatrix4fv(EnemyView, 1, GL_FALSE, glm::value_ptr(view));

        glBindBuffer(GL_ARRAY_BUFFER, Enemy_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(enemy_vertices), sizeof(Enemy_Renderable) * MAX_ENEMIES, enemy_positions);

        //glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, MAX_ENEMIES);

        /* ----- Draw a (the) chunk ----- */
        glBindVertexArray(WORLD_VAO);

        glUseProgram(WorldShaderProgram);
        glUniformMatrix4fv(WorldViewMatrix, 1, GL_FALSE, glm::value_ptr(view));

        ChunkUpdate(&TestChunk, ChunkTemp);
        glBindBuffer(GL_ARRAY_BUFFER, World_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_vertices), sizeof(Chunk_Renderable) * TestChunk.NumBlocks, ChunkTemp);

        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, TestChunk.NumBlocks);

        /* ----- Draw Water ----- */
        glBindVertexArray(WATER_VAO);

        glUseProgram(WaterShaderProgram);
        glUniformMatrix4fv(WaterViewMatrix, 1, GL_FALSE, glm::value_ptr(view));

        vec3 WaterPosition[6];
        WaterPosition[0] = vec3(2, 12, 2);
        WaterPosition[1] = vec3(3, 12, 2);
        WaterPosition[2] = vec3(4, 12, 2);
        WaterPosition[3] = vec3(3, 12, 3);
        WaterPosition[4] = vec3(2, 12, 3);
        WaterPosition[5] = vec3(2, 12, 4);

        timer[0] += DeltaTime;
        if (timer[0] > 2) timer[0] = 0;
        glUniform1f(WaterTimer, timer[0]);

        glBindBuffer(GL_ARRAY_BUFFER, Water_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(water_vertices), sizeof(vec3) * 6, WaterPosition);

        glDrawElementsInstanced(GL_TRIANGLES, 150, GL_UNSIGNED_INT, 0, 6);

        /*------------------------------- Finish Frame -------------------------------*/

        // Update the window
        glfwSwapBuffers(WindowInstance);
        glfwPollEvents();

        //Frame Time
        QueryPerformanceCounter(&EndCounter);
        int64 CyclesElapsed = (int64)(EndCounter.QuadPart - StartCounter.QuadPart);
        int64 MillisecondsElapsed = (CyclesElapsed * 1000) / PerformanceFrequency;

        if ((uint)(1000 * TargetFrameTime) > (uint)MillisecondsElapsed)
        {
            uint SleepMS = (uint)(TargetFrameTime * 1000) - (uint)MillisecondsElapsed;
            Sleep(SleepMS);

            QueryPerformanceCounter(&EndCounter);
            CyclesElapsed = (int64)(EndCounter.QuadPart - StartCounter.QuadPart);
            MillisecondsElapsed = (CyclesElapsed * 1000) / PerformanceFrequency;
        }

        StartCounter.QuadPart = EndCounter.QuadPart;
    }

    /*------------------------------- End Game Loop -------------------------------*/

    /*------- Cleanup & Exit -------*/

    //glDeleteTextures(1, &WorldTextureAtlas);

    //World - macro? #define glCleanupShader()?
    glDeleteProgram(WorldShaderProgram);
    glDeleteShader(WorldVertShader);
    glDeleteShader(WorldFragShader);
    glDeleteVertexArrays(1, &WORLD_VAO);
    glDeleteBuffers(1, &World_VBO);
    glDeleteBuffers(1, &World_EBO);
    //HUD
    glDeleteProgram(HUDShaderProgram);
    glDeleteShader(HUDVertShader);
    glDeleteShader(HUDFragShader);
    glDeleteVertexArrays(1, &HUD_VAO);
    glDeleteBuffers(1, &HUD_VBO);
    glDeleteBuffers(1, &HUD_EBO);

    glfwTerminate();

    VirtualFree(GameMemory, 0, MEM_RELEASE); //Give back the memory

    out("Cleanup Succesfull!");

    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = (float) xpos;
        lastY = (float) ypos;
        firstMouse = false;
    }

    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(lastY - ypos); // reversed since y-coordinates go from bottom to top

    lastX = (float) xpos;
    lastY = (float) ypos;

    if(!Paused) CameraProcessMouseMovement(&camera, xoffset, yoffset);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ;//
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ;
}