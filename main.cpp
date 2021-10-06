#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_opengl3_loader.h"
#include <GLFW/glfw3.h>
#include "memory.h"
#include "arm9.hpp"
int main()
{
    if (!glfwInit()) //if glfw doesn't load, exit
        return 1;

    imgl3wInit();

    //OPENGL 3.0, GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "KiDS", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  
    int pos = 0;
    ARM9 cpu;
    while (!glfwWindowShouldClose(window))
    {
        cpu.step();
        // glfwPollEvents();

        // ImGui_ImplOpenGL3_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();

        // ImGui::Begin("Hello, world!");  
        // ImGui::Text("Hewwo OwO");
        // ImGui::End();

        // ImGui::Render();
        // int display_w, display_h;
        // glfwGetFramebufferSize(window, &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfwSwapBuffers(window);   
    }

    //cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

}