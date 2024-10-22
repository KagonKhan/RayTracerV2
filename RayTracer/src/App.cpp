#include "App.hpp"
#include "Input.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imstb_textedit.h>
#include <chrono>
#include <glm/gtc/type_ptr.hpp>
#include <ranges>


void init (Scene &s) {
    Material &pinkSphere = s.materials.emplace_back ();
    pinkSphere.Albedo    = {1.0f, 0.0f, 1.0f};
    pinkSphere.Roughness = 0.0f;

    Material &blueSphere = s.materials.emplace_back ();
    blueSphere.Albedo    = {0.2f, 0.3f, 1.0f};
    blueSphere.Roughness = 0.1f;

    Material &orangeSphere     = s.materials.emplace_back ();
    orangeSphere.Albedo        = {0.8f, 0.5f, 0.2f};
    orangeSphere.Roughness     = 0.1f;
    orangeSphere.EmissionColor = orangeSphere.Albedo;
    orangeSphere.EmissionPower = 2.0f;

    {
        Sphere sphere;
        sphere.position      = {0.0f, 0.0f, 0.0f};
        sphere.radius        = 1.0f;
        sphere.materialIndex = 0;
        s.spheres.push_back (sphere);
    }

    {
        Sphere sphere;
        sphere.position      = {2.0f, 0.0f, 0.0f};
        sphere.radius        = 1.0f;
        sphere.materialIndex = 2;
        s.spheres.push_back (sphere);
    }

    {
        Sphere sphere;
        sphere.position      = {0.0f, -101.0f, 0.0f};
        sphere.radius        = 100.0f;
        sphere.materialIndex = 1;
        s.spheres.push_back (sphere);
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
    const static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
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

#ifdef NDEBUG

    renderer.render (camera, scene);

#endif

    auto image = renderer.getImage ();

    ImGui::Image ((ImTextureID) (intptr_t) image->getID (), {(float) image->getSize ().x, (float) image->getSize ().y}, ImVec2 (0, 1), ImVec2 (1, 0));
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
    ImGui::DragInt ("Max bounces", &renderer.settings.bounces, 0.1f, 0, 20);
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
        spheres.emplace_back ();
        scene.materials.emplace_back ();
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
    auto &spheres = scene.spheres;


    ImGui::Separator ();
    ImGui::Dummy ({0.0f, 20.f});
    ImGui::PushID (selectedIndex);

    Sphere &sphere = spheres[selectedIndex];
    ImGui::Text ("Object data");
    ImGui::DragFloat3 ("Position", glm::value_ptr (sphere.position), 0.1f);
    ImGui::DragFloat ("Radius", &sphere.radius, 0.1f);
    ImGui::DragInt ("Material", &sphere.materialIndex, 1.0f, 0, (int) scene.materials.size () - 1);

    ImGui::Separator ();
    ImGui::Dummy ({0.0f, 20.f});


    Material &material = scene.materials[sphere.materialIndex];
    ImGui::Text ("Material data");
    ImGui::ColorEdit3 ("Albedo", glm::value_ptr (material.Albedo));
    ImGui::DragFloat ("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat ("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
    ImGui::ColorEdit3 ("Emission Color", glm::value_ptr (material.EmissionColor));
    ImGui::DragFloat ("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);

    ImGui::Separator ();

    ImGui::PopID ();
}
