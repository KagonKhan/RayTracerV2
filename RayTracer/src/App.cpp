#include "App.hpp"
#include "Input.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imstb_textedit.h>
#include <chrono>
#include <glm/gtc/type_ptr.hpp>
#include <ranges>


void init (Scene &s) {
    // Material &left  = s.materials.emplace_back ();
    // left.Roughness  = 0.0f;
    // left.Refractive = 1.5;

    // Material &bubble  = s.materials.emplace_back ();
    // bubble.Roughness  = 0.0f;
    // bubble.Refractive = 1.0f / 1.5f;

    // Material &blueSphere = s.materials.emplace_back ();
    // blueSphere.Albedo    = {0.2f, 0.3f, 1.0f};
    // blueSphere.Roughness = 0.1f;

    // Material &orangeSphere     = s.materials.emplace_back ();
    // orangeSphere.Albedo        = {0.8f, 0.5f, 0.2f};
    // orangeSphere.Roughness     = 0.1f;
    // orangeSphere.EmissionColor = orangeSphere.Albedo;
    // orangeSphere.EmissionPower = 2.0f;

    // {
    //     Sphere sphere;
    //     sphere.position      = {-1.0, 0.0, -1.0};
    //     sphere.radius        = 0.5f;
    //     sphere.materialIndex = 0;
    //     s.spheres.push_back (sphere);
    // }

    // {
    //     Sphere sphere;
    //     sphere.position      = {-1.0, 0.0, -1.0};
    //     sphere.radius        = 0.4f;
    //     sphere.materialIndex = 1;
    //     s.spheres.push_back (sphere);
    // }

    // {
    //     Sphere sphere;
    //     sphere.position      = {0.0f, -101.0f, 0.0f};
    //     sphere.radius        = 100.0f;
    //     sphere.materialIndex = 2;
    //     s.spheres.push_back (sphere);
    // }


    {
        auto &ground_material  = s.materials.emplace_back ();
        ground_material.Albedo = glm::vec3 (0.5f);


        auto &ground         = s.spheres.emplace_back ();
        ground.position      = glm::vec3 (0, -1000, 0);
        ground.radius        = 1000;
        ground.materialIndex = s.materials.size () - 1;
    }

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto      choose_mat = Random::Float ();
            glm::vec3 center (a + 0.9 * Random::Float (), 0.2, b + 0.9 * Random::Float ());

            if ((center - glm::vec3 (4, 0.2, 0)).length () > 0.9) {
                auto &sphere_material = s.materials.emplace_back ();

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo            = Random::Vec3 () * Random::Vec3 ();
                    sphere_material.Albedo = albedo;
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo               = Random::Vec3 (0.5, 1);
                    auto fuzz                 = Random::Float () / 2.f;
                    sphere_material.Roughness = fuzz;
                    sphere_material.Albedo    = albedo;
                    sphere_material.Metallic  = 0.9f;
                } else {
                    // glass
                    sphere_material.Refractive = 1.5f;
                }

                auto &s1         = s.spheres.emplace_back ();
                s1.position      = center;
                s1.radius        = 0.2;
                s1.materialIndex = s.materials.size () - 1;
            }
        }
    }


    {
        auto &material1      = s.materials.emplace_back ();
        material1.Refractive = 1.5f;

        auto &s1         = s.spheres.emplace_back ();
        s1.position      = glm::vec3 (0, 1, 0);
        s1.radius        = 1;
        s1.materialIndex = s.materials.size () - 1;
    }


    {
        auto &material1  = s.materials.emplace_back ();
        material1.Albedo = glm::vec3 (0.4, 0.2, 0.1);

        auto &s1         = s.spheres.emplace_back ();
        s1.position      = glm::vec3 (-4, 1, 0);
        s1.radius        = 1;
        s1.materialIndex = s.materials.size () - 1;
    }


    {
        auto &material1     = s.materials.emplace_back ();
        material1.Albedo    = glm::vec3 (0.7, 0.6, 0.5);
        material1.Metallic  = 0.9f;
        material1.Roughness = 0.0f;

        auto &s1         = s.spheres.emplace_back ();
        s1.position      = glm::vec3 (4, 1, 0);
        s1.radius        = 1;
        s1.materialIndex = s.materials.size () - 1;
    }
}


void App::start () {
    init (scene);
    Random::Init ();
    Input::Bind (&window);

    auto lastTime = std::chrono::high_resolution_clock::now ();
    while (!glfwWindowShouldClose (window)) {
        auto now      = std::chrono::high_resolution_clock::now ();
        dtime         = std::chrono::duration_cast<std::chrono::microseconds> (now - lastTime).count () / 1000.f;
        lastTime      = now;
        auto lastTime = std::chrono::high_resolution_clock::now ();
        glfwPollEvents ();

        if (glfwGetWindowAttrib (window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep (10);
            continue;
        }
        window.startFrame ();
        prepareBackground ();

        renderApp ();
        window.render ();
    }

    Input::Unbind (&window);
}

void App::prepareBackground () {
    const static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                                 ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport *viewport = ImGui::GetMainViewport ();
    ImGui::SetNextWindowPos (viewport->WorkPos);
    ImGui::SetNextWindowSize (viewport->WorkSize);
    ImGui::SetNextWindowViewport (viewport->ID);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 (0.0f, 0.0f));
    ImGui::Begin ("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar (3);

    // Submit the DockSpace
    ImGuiIO &io = ImGui::GetIO ();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGui::DockSpace (ImGui::GetID ("AppDockspace"), ImVec2 (0.0f, 0.0f), ImGuiDockNodeFlags_NoUndocking);
    }
    ImGui::End ();
}

void App::renderApp () {
    renderSettingsGUI ();
    renderObjectSettings ();
    renderScene ();
}

void App::renderScene () {
    ImGui::Begin ("Scene");

    auto width  = ImGui::GetContentRegionAvail ().x;
    auto height = ImGui::GetContentRegionAvail ().y;
    if (camera.onUpdate (dtime / 1000.f)) {
        renderer.ResetFrameIndex ();
    }
    camera.onResize ({(int) width, (int) height});
    renderer.onResize ({(int) width, (int) height});


    renderer.render (camera, scene);


    auto image = renderer.getImage ();

    ImGui::Image ((ImTextureID) (intptr_t) image->getID (), {(float) image->getSize ().x, (float) image->getSize ().y},
                  ImVec2 (0, 1), ImVec2 (1, 0));
    ImGui::End ();
}

void App::renderObjectSettings () {
    ImGui::Begin ("Objects");
    if (!scene.spheres.empty ()) {
        editor.render ();
    }

    ImGui::End ();
}

void App::renderSettingsGUI () {
    ImGui::Begin ("Settings");
    ImGui::Text ("Current frame rate %.3f ms", dtime);
    // ImGui::Text ("Render time %.3f ms", render_time);
    // if (ImGui::Button ("RENDER")) {
    //     auto t1 = std::chrono::high_resolution_clock::now ();
    //     renderer.render (camera, scene);
    //     auto t2     = std::chrono::high_resolution_clock::now ();
    //     render_time = std::chrono::duration_cast<std::chrono::microseconds> (t2 - t1).count () / 1000.f;
    // }
    ImGui::Checkbox ("ACCUMULATE", &renderer.settings.Accumulate);
    if (ImGui::Button ("RESET")) {
        renderer.ResetFrameIndex ();
    }
    ImGui::DragInt ("Max bounces", &renderer.settings.bounces, 0.1f, 0, 500);
    ImGui::End ();
}

void App::ObjectEditor::render () {
    renderListOfObjects ();
    if (selectedIndex < 0)
        return;
    renderObjectDetails ();
}

void App::ObjectEditor::renderListOfObjects () {
    auto &spheres = scene.spheres;

    ImGui::PushItemWidth (-1);

    if (ImGui::BeginListBox ("Objects List")) {
        for (int i = 0; i < spheres.size (); ++i) {
            std::string item_name = "Sphere " + std::to_string (i + 1);
            if (ImGui::Selectable (item_name.c_str (), selectedIndex == i)) {
                selectedIndex = i;
            }
        }
        ImGui::EndListBox ();
    }
    ImGui::PopItemWidth ();
    ImGui::Dummy ({0.0f, 20.f});

    if (ImGui::Button ("Add Sphere")) {
        scene.materials.emplace_back ();
        spheres.emplace_back ();
        spheres.back ().materialIndex = scene.materials.size () - 1;
        selectedIndex                 = spheres.size () - 1;
    }
    ImGui::SameLine ();
    if (ImGui::Button ("Remove selected sphere")) {
        if (selectedIndex >= 0 && selectedIndex < spheres.size ()) {
            spheres.erase (spheres.begin () + selectedIndex);
            selectedIndex = -1;
        }
    }
}

void App::ObjectEditor::renderObjectDetails () {
    Sphere &sphere = scene.spheres[selectedIndex];

    ImGui::PushID (selectedIndex);
    {
        ImGui::Separator ();
        ImGui::Dummy ({0.0f, 20.f});
        ImGui::Text ("Object data");
        ImGui::DragFloat3 ("Position", glm::value_ptr (sphere.position), 0.1f);
        ImGui::DragFloat ("Radius", &sphere.radius, 0.1f);
        ImGui::DragInt ("Material", &sphere.materialIndex, 1.0f, 0, (int) scene.materials.size () - 1);
    }
    {
        ImGui::Separator ();
        ImGui::Dummy ({0.0f, 20.f});

        Material &material = scene.materials[sphere.materialIndex];
        ImGui::Text ("Material data");
        ImGui::ColorEdit3 ("Albedo", glm::value_ptr (material.Albedo));
        ImGui::DragFloat ("Roughness", &material.Roughness, 0.005f, 0.0f, 1.0f);
        ImGui::DragFloat ("Metallic", &material.Metallic, 0.005f, 0.0f, 1.0f);
        ImGui::DragFloat ("Refractive", &material.Refractive, 0.005f, 0.0f, 5.0f);
        ImGui::ColorEdit3 ("Emission Color", glm::value_ptr (material.EmissionColor));
        ImGui::DragFloat ("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);
    }
    ImGui::PopID ();
}
