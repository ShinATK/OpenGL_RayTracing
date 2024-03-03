#include "./components/GLWindow.h"
#include "./components/GUI.h"
#include "./scene/Scene.h"

#include "./tools/type_convert.h"

const char* OBJECT_NAME = "Ray Tracing";

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

void HideCursor(GLFWwindow* window, GLboolean bHide);

// Screen Size
GLuint SCR_WIDTH  = 1080;
GLuint SCR_HEIGHT = 720;

// 
GLboolean Hold[1024];
GLboolean bFirstMouse = GL_TRUE;
GLboolean bCatchMouse = GL_TRUE;
GLfloat lastX         = SCR_WIDTH / 2;
GLfloat lastY         = SCR_HEIGHT / 2;
GLfloat xpos          = lastX;
GLfloat ypos          = lastY;

//
GLWindow* window;
GUI* gui;

Scene* scene;

// GUI's
static bool configurePosition = false;


int main()
{
    // Create window
    window = new GLWindow(SCR_WIDTH, SCR_HEIGHT, OBJECT_NAME);

    // 初始化窗口
    if(!window->Init())
        return -1;

    // 设置回调函数
    glfwSetKeyCallback(window->GetWindow(), KeyCallback);
    glfwSetCursorPosCallback(window->GetWindow(), CursorPosCallback);
    glfwSetMouseButtonCallback(window->GetWindow(), MouseClickCallback);

    // 初始化场景
    scene = new Scene(window->GetWindow(), SCR_WIDTH, SCR_HEIGHT);
    scene->Init();

    // 初始化 GUI
    gui = new GUI(window->GetWindow());
    
    GLfloat delta_time      = 0.0f;
    GLfloat last_frame_time = 0.0f;

    // 主循环，处理窗口事件
    while (!window->ShouldClose())
    {
/* Window Setting */
        HideCursor(window->GetWindow(), bCatchMouse);
        glClearColor(gui->clearColor.x * gui->clearColor.w, gui->clearColor.y * gui->clearColor.w, gui->clearColor.z * gui->clearColor.w, gui->clearColor.w);
        glEnable(GL_DEPTH_TEST); 
        //glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO:封装到 GLWindow中，用引用返回
        GLfloat current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;
/* ============== */

        // Ready GUI
        gui->Init(bCatchMouse);
        gui->MainWidget(OBJECT_NAME);

        // Input
        scene->ProcessInput(delta_time);


/* Configure chosen object */ 
        
        if (scene->GetChosenObject()) {
            ImGui::Begin(scene->GetChosenObject()->GetName());
            {
                // Set position
                static float pos[3];
                int pos_limits[2] = {-10, 10};
                ToFloat3<glm::vec3, float>(scene->GetChosenObject()->GetPosition(), pos);
                gui->SetVec3("Set Position", pos, pos_limits);
                scene->GetChosenObject()->SetPosition(ToVec3<float, glm::vec3>(pos));

                // Set Size
                static float size[3];
                int size_limits[2] = {0, 5};
                ToFloat3<glm::vec3, float>(scene->GetChosenObject()->GetSize(), size);
                gui->SetVec3("Set Size", size, size_limits);
                scene->GetChosenObject()->SetSize(ToVec3<float, glm::vec3>(size));

                // Set rotation
                //static float rotation[4];
                //// int rotate_limits[]
                //int angle_limits[2] = { -180, 180 };
                //ToFloat4<glm::vec4, float>(scene->GetChosenObject()->GetRotation(), rotation);
                //// TODO: 修改为选择依据世界空间的 x y z 哪个轴转
                //static bool 
                //gui->Text("Rotation Axis");
                //gui->SetBool("Axis-x", , false); gui->SetBool("Axis-y", , true); gui->SetBool("Axis-z", , true);
                //gui->SetFloat("Set Rotate Angle", rotation[3], angle_limits);
            }
            ImGui::End();
        }

        // Update
        scene->Update();

/* Render */
        scene->Render(delta_time);
        gui->Render();                              // Render GUI widget
        

/* Swap buffer and Poll events */ 
        window->Update();
    }

    delete scene;
    delete gui;
    delete window;

    return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Press ESC to close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    // Show or hide cursor
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        if (!Hold[key])
        {
            bCatchMouse = !bCatchMouse;
            Hold[key] = GL_TRUE;
        }
    }
    else if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
    {
        Hold[key] = GL_FALSE;
    }
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Camera view movement
    scene->SetCameraView(xpos, ypos, lastX, lastY, bCatchMouse, bFirstMouse);

}

void MouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (!bCatchMouse && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double mouseX, mouseY; glfwGetCursorPos(window, &mouseX, &mouseY);
        // float depth; glReadPixels(mouseX, mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        scene->CheckClicked(mouseX, mouseY);
    }
    if (!bCatchMouse && button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        scene->ClearChosenObject();
    }
}

void HideCursor(GLFWwindow* window, GLboolean bHide)
{
    if (bHide)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

