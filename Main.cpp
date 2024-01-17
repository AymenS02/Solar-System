#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"

#include <iostream>
#include <fstream>
#include <string>

static unsigned int ss_id = 0;
void dump_framebuffer_to_ppm(std::string prefix, unsigned int width, unsigned int height) {
    int pixelChannel = 3;
    int totalPixelSize = pixelChannel * width * height * sizeof(GLubyte);
    GLubyte* pixels = new GLubyte[totalPixelSize];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    std::string file_name = prefix + std::to_string(ss_id) + ".ppm";
    std::ofstream fout(file_name);
    fout << "P3\n" << width << " " << height << "\n" << 255 << std::endl;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            size_t cur = pixelChannel * ((height - i - 1) * width + j);
            fout << (int)pixels[cur] << " " << (int)pixels[cur + 1] << " " << (int)pixels[cur + 2] << " ";
        }
        fout << std::endl;
    }
    ss_id++;
    delete[] pixels;
    fout.flush();
    fout.close();
}

//key board control
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window)
{
    //press escape to exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //press p to capture screen
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        std::cout << "Capture Window " << ss_id << std::endl;
        int buffer_width, buffer_height;
        glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
        dump_framebuffer_to_ppm("Assignment0-ss", buffer_width, buffer_height);
    }
}


// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

//sun
float get_sun_rotate_angle_around_itself(float day)
{
    float sunRotationPeriodInDays = 27.0f;
    return (360.0f / sunRotationPeriodInDays) * day;
}
//earth
float get_earth_rotate_angle_around_sun(float day)
{
    float earthRotationPeriodInDays = 365.0f;
    return (360.0f / earthRotationPeriodInDays) * day;
}    
float get_earth_rotate_angle_around_itself(float day)
{
    float earthRotationPeriodInDays = 1.0f;
    return (360.0f / earthRotationPeriodInDays) * day;
}
//moon
float get_moon_rotate_angle_around_earth(float day)
{
    float moonRotationPeriodInDays = 28.0f;
    return (360.0f / moonRotationPeriodInDays) * day;
}
float get_moon_rotate_angle_around_itself(float day)
{
    float moonRotationPeriodInDays = 28.0f;
    return (360.0f / moonRotationPeriodInDays) * day;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("6.3.coordinate_systems.vs", "6.3.coordinate_systems.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        //Positions                 // colors
        //yellow z = -1 
        -1.0f, -1.0f, -1.0f,        1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,        1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,        1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,        1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,        1.0f, 1.0f, 0.0f,

        //fuchsia z = -1 
        -1.0f, -1.0f,  1.0f,        1.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        1.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,        1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,        1.0f, 0.0f, 1.0f,

        //red
        -1.0f,  1.0f,  1.0f,        1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,        1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,        1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,        1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,        1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,        1.0f, 0.0f, 0.0f,

        //green
         1.0f,  1.0f,  1.0f,        0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,        0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,        0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,        0.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,        0.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,        0.0f, 1.0f, 0.0f,

         //aqua
        -1.0f, -1.0f, -1.0f,        0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,        0.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        0.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,        0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,        0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,        0.0f, 1.0f, 1.0f,

        //blue
        -1.0f,  1.0f, -1.0f,        0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, -1.0f,        0.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        0.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,        0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,        0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,        0.0f, 0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    glm::mat4 projection = glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
    ourShader.setMat4("projection", projection);

    float day = 0, inc = 1.0f / 6; // Replace with the desired day value

    // Calculate rotation angles using the functions


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float sunRotation = get_sun_rotate_angle_around_itself(day);
        float earthSunRotation = get_earth_rotate_angle_around_sun(day);
        float earthRotation = get_earth_rotate_angle_around_itself(day);
        float moonEarthRotation = get_moon_rotate_angle_around_earth(day);
        float moonRotation = get_moon_rotate_angle_around_itself(day);
        // input
        // -----
        processInput(window);
        // render
        // ------
        glClearColor(0.3f, 0.4f, 0.5, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 view = glm::mat4(1.0f);
        //earth origin 
        float z1 = (24.0f * (float)(-sin(glm::radians(earthSunRotation))));
        float x1 = (24.0f * (float)cos(glm::radians(earthSunRotation)));
        float z = (12.0f * (float)(-sin(glm::radians(moonEarthRotation))));
        float x = (12.0f * (float)cos(glm::radians(moonEarthRotation)));
        // Keeping the camera static instead of moving in a circle
        view = glm::lookAt(glm::vec3(100.0f, 50.0f, 100.0f), // Camera is at (x,y,z), in World Space
            glm::vec3(x1, 0.0f, z1),  // and looks at the origin
            glm::vec3(0.0f, 1.0f, 0.0f)); // Head is up (set to 0,-1,0 to look upside-down)
        ourShader.setMat4("view", view);
        

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 3; i++)
        {            
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            

            if (i == 0)
            {
                // Cube 0 (i == 0) is at the origin (0,0,0) and has a size of 12x12x12
                glm::mat4 trans0 = glm::translate(model, cubePositions[0]);
                glm::mat4 rot0 = glm::rotate(model, glm::radians(sunRotation), glm::vec3(0.0f, 1.0f, 0.0f));
                glm::mat4 scale0 = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));

                model = rot0 * trans0 * scale0;
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);

            }
            else if (i == 1)
            {
                // Cube 1 (i == 1) is relative to Cube 0 (i == 0) and has a size of 6x6x6
                glm::mat4 scale1 = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
                glm::mat4 trans1 = glm::translate(model, cubePositions[1]); // Position relative to Cube 0
                float angle = 23.4f;
                glm::mat4 prerot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, -1.0f));
                glm::mat4 rot1 = glm::rotate(glm::mat4(1.0f), glm::radians(earthRotation), glm::vec3(0.0f, 1.0f, 0.0f));
                float z = (24.0f * (float)(-sin(glm::radians(earthSunRotation))));
                float x = (24.0f * (float)cos(glm::radians(earthSunRotation)));
                glm::mat4 orb1 = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));
                model = trans1 * prerot * orb1 * rot1 * scale1;
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 2)
            {
                // Cube 2 (i == 2) is relative to Cube 1 (i == 1) and has a size of 3x3x3
                glm::mat4 trans2 = glm::translate(model, cubePositions[2]); // Position relative to Cube 0
                glm::mat4 scale2 = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
                glm::mat4 rot2 = glm::rotate(model, glm::radians(moonRotation), glm::vec3(0.0f, 1.0f, 0.0f));
                float z1 = (24.0f * (float)(-sin(glm::radians(earthSunRotation))));
                float x1 = (24.0f * (float)cos(glm::radians(earthSunRotation)));
                float z = (12.0f * (float)(-sin(glm::radians(moonEarthRotation))));
                float x = (12.0f * (float)cos(glm::radians(moonEarthRotation)));
                glm::mat4 orb2 = glm::translate(model, glm::vec3(x+x1, 0.0f, z+z1));

                model = orb2 * rot2 * trans2 * scale2;
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            // draw solar system on that day


        }
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        day += inc;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}