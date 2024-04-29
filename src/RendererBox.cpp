#include "./components/GLWindow.h"
#include "./components/GUI.h"
#include "./scene/Scene.h"
#include "./scene/tools.h"

const char* OBJECT_NAME = "Renderer Box";

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

    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_CULL_FACE);

    // 主循环，处理窗口事件
    while (!window->ShouldClose())
    {
/* Window Setting */
        HideCursor(window->GetWindow(), bCatchMouse);
        glClearColor(gui->clearColor.x * gui->clearColor.w, gui->clearColor.y * gui->clearColor.w, gui->clearColor.z * gui->clearColor.w, gui->clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO:封装到 GLWindow中，用引用返回
        GLfloat current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;
/* ============== */

        // Ready GUI
        gui->Init(bCatchMouse);
        gui->MainWidget(OBJECT_NAME, false);
        {   
            // Configure Scene Skybox
            static bool bShowSkybox = scene->GetShowSkybox();
            gui->SetCheckBox("Show Skybox", &bShowSkybox);
            scene->SetShowSkybox(bShowSkybox);
            
            // Configure Camera movement
            static bool bLockOnFloor = scene->GetCamera()->GetbLockOnFloor();
            gui->SetCheckBox("Camera Lock On Floor", &bLockOnFloor);
            scene->GetCamera()->SetbLockOnFloor(bLockOnFloor);

        }
        ImGui::End();

        // Input
        scene->ProcessInput(delta_time);
        
        // Configure Chosen Object
        if (scene->GetChosenObject()) {
            ImGui::Begin(scene->GetChosenObject()->GetName());
            {
                // Set Screen Space bounding box
                static bool show_2DBBox; show_2DBBox = scene->GetChosenObject()->GetShow2DBBox();
                gui->SetCheckBox("Show 2D BBox", &show_2DBBox);
                scene->GetChosenObject()->SetShow2DBox(show_2DBBox);

                // Set Light Method
                static int light_method; light_method = scene->GetChosenObject()->GetLightMethod(); // phong by default
                ImGui::Text("Light Method"); ImGui::SameLine();
                gui->SetRadioButton("Phong", &light_method, 0); ImGui::SameLine();
                gui->SetRadioButton("Blinn-Phong", &light_method, 1);
                scene->GetChosenObject()->SetLightMethod(light_method);

                // Set position
                static bool pos_checkBox = false;
                gui->SetCheckBox("Object position configure", &pos_checkBox); ImGui::SameLine();
                if (ImGui::Button("Rest Position")) { scene->GetChosenObject()->SetPosition(glm::vec3(0.0f)); }
                if (pos_checkBox)
                {
                    static float    pos[3];
                    int             pos_limits[2] = { -10, 10 };
                    ToFloat3<glm::vec3, float>(scene->GetChosenObject()->GetPosition(), pos);
                    gui->SetVec3("Set Position", pos, pos_limits);
                    scene->GetChosenObject()->SetPosition(ToVec3<float, glm::vec3>(pos));
                }

                // Set Size
                static bool size_checkBox = false;
                gui->SetCheckBox("Object size configure", &size_checkBox); ImGui::SameLine();
                if (ImGui::Button("Rest Size")) { scene->GetChosenObject()->SetSize(glm::vec3(1.0f)); }
                if (size_checkBox)
                {
                    static float    size[3];
                    int             size_limits[2] = { 0, 50 };
                    ToFloat3<glm::vec3, float>(scene->GetChosenObject()->GetSize(), size);
                    gui->SetVec3("Set Size", size, size_limits);
                    scene->GetChosenObject()->SetSize(ToVec3<float, glm::vec3>(size));
                }

                // Set Rotation
                static bool rotate_checkBox = false;
                gui->SetCheckBox("Object rotate configure", &rotate_checkBox); ImGui::SameLine();
                if (ImGui::Button("Rest Rotation")) {
                    scene->GetChosenObject()->SetYaw(0);
                    scene->GetChosenObject()->SetPitch(0);
                    scene->GetChosenObject()->SetRoll(0);
                }
                if (rotate_checkBox)
                {
                    static int axis = -1;
                    static float angle[3];
                    angle[0] = scene->GetChosenObject()->GetYaw();
                    angle[1] = scene->GetChosenObject()->GetPitch();
                    angle[2] = scene->GetChosenObject()->GetRoll();
                    if (gui->SetRadioButton("Axis X", &axis, 0) == 0) {
                        ImGui::SameLine();
                        gui->SetFloat("Set Yaw", angle[0], -180, 180);
                        scene->GetChosenObject()->SetYaw(angle[0]);
                    }
                    if (gui->SetRadioButton("Axis Y", &axis, 1) == 1) {
                        ImGui::SameLine();
                        gui->SetFloat("Set Pitch", angle[1], -180, 180);
                        scene->GetChosenObject()->SetPitch(angle[1]);
                    }
                    if (gui->SetRadioButton("Axis Z", &axis, 2) == 2) {
                        ImGui::SameLine();
                        gui->SetFloat("Set Roll", angle[2], -180, 180);
                        scene->GetChosenObject()->SetRoll(angle[2]);
                    }
                }

                // Set Texture
                static bool texture_checkBox; texture_checkBox = scene->GetChosenObject()->GetUseTexture();
                gui->SetCheckBox("Object texture configrue", &texture_checkBox);
                scene->GetChosenObject()->SetUseTexture(texture_checkBox);

                // Set material
                static bool material_checkBox = false;
                gui->SetCheckBox("Object material configure", &material_checkBox);
                static int limits[2] = { 0, 1 };
                if (material_checkBox) {
                    static float ambient[3];
                    ToFloat3<glm::vec3, float>(scene->GetChosenObject()->GetAmbient(), ambient);
                    gui->SetVec3("Set Ambient", ambient, limits);
                    scene->GetChosenObject()->SetAmbient(ToVec3<float, glm::vec3>(ambient));

                    static float diffuse[3];
                    ToFloat3<glm::vec3, float>(scene->GetChosenObject()->GetDiffuse(), diffuse);
                    gui->SetVec3("Set Diffuse", diffuse, limits);
                    scene->GetChosenObject()->SetDiffuse(ToVec3<float, glm::vec3>(diffuse));

                    static float specular[3];
                    ToFloat3<glm::vec3, float>(scene->GetChosenObject()->GetSpecular(), specular);
                    gui->SetVec3("Set Specular", specular, limits);
                    scene->GetChosenObject()->SetSpecular(ToVec3<float, glm::vec3>(specular));

                    static float shininess;
                    shininess = scene->GetChosenObject()->GetShininess();
                    gui->SetFloat("SetShininess", shininess, 0.0f, 100.0f);
                    scene->GetChosenObject()->SetShininess(shininess);
                }
            }
            ImGui::End();
        }

        // Update
        scene->Update();

/* Render */
        scene->Render();
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
        if(scene->GetChosenObject() == nullptr) scene->CheckClicked(mouseX, mouseY);
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
