#include "game.h"

Game::Game() {
    window = new Window(1920, 1080, "GLRayTracing");
}
int Game::setup() {

    ShaderInclude common = ShaderInclude("assets/shaders/common/common.glsl", "/common.glsl");

    //include directories
    std::vector<char*> common_vec = {(char *)"/common.glsl"};
    point_shader = new Shader("assets/shaders/vertex/point.glsl", "assets/shaders/fragment/point.glsl", common_vec);
    line_shader = new Shader("assets/shaders/vertex/line.glsl", "assets/shaders/fragment/line.glsl", common_vec);
    center_shader = new Shader("assets/shaders/vertex/center.glsl", "assets/shaders/fragment/point.glsl", common_vec);
    shape_match_shader = new Shader("assets/shaders/vertex/shape_matching.glsl", "assets/shaders/fragment/line.glsl", common_vec);
    triangle_shader = new Shader("assets/shaders/vertex/triangle.glsl", "assets/shaders/fragment/triangle.glsl", common_vec);
    
    polygon_shader = new Shader("assets/shaders/vertex/polygon.glsl", "assets/shaders/fragment/polygon.glsl", common_vec);

    init_indices_boundaries = new ComputeShader("assets/shaders/compute/find_indice_boundaries.glsl", common_vec);
    computePhysics = new ComputeShader("assets/shaders/compute/physics.glsl", common_vec);
    init_object_buffer = new ComputeShader("assets/shaders/compute/find_object_boundaries.glsl", common_vec);
    computeSprings = new ComputeShader("assets/shaders/compute/spring_physics.glsl", common_vec);
    findCenters = new ComputeShader("assets/shaders/compute/find_centers.glsl", common_vec);
    resetCenter = new ComputeShader("assets/shaders/compute/reset_center.glsl", common_vec);
    findOffsets = new ComputeShader("assets/shaders/compute/find_offsets.glsl", common_vec);
    shapeMatching = new ComputeShader("assets/shaders/compute/shape_matching.glsl", common_vec);
    find_angles = new ComputeShader("assets/shaders/compute/find_angles.glsl", common_vec);
    averageCenters = new ComputeShader("assets/shaders/compute/average_centers.glsl", common_vec);
    averageAngles = new ComputeShader("assets/shaders/compute/average_angles.glsl", common_vec);
    gravity = new ComputeShader("assets/shaders/compute/gravity.glsl", common_vec);
    integrate = new ComputeShader("assets/shaders/compute/integrate.glsl", common_vec);
    init_object_properties = new ComputeShader("assets/shaders/compute/init_object_properties.glsl", common_vec);

    findBounds = new ComputeShader("assets/shaders/compute/find_bounds.glsl", common_vec);
    calculate_bounds = new ComputeShader("assets/shaders/compute/calculate_bounding_box.glsl", common_vec);

    Square square1 = Square(glm::vec3(0., -10400., 0.), 10000.0, object_id++, 10000000000.0, 0.1,0.05);
    square1.insert(&vertex_data, &object_data, &indices, &triangle_indices, &spring_indices, &spring_data);
    for (int x = 0; x < 12; x++) {
        for (int y = 0; y < 12; y++) {
            Circle circle = Circle(glm::vec3((x - 8) * 100, (y + 2) * 125, 0.), 40.0, object_id++, 1.0, 0.1, 0.01, 10);
            circle.insert(&vertex_data, &object_data, &indices, &triangle_indices, &spring_indices, &spring_data);
        }
    }
    /*
    these(high resolution polygons) cause "some"(a lot) of instability when spawned 
    into the scene. If anyone wants to take stab at fixing the problem it probably 
    has to do with an edge case of the point in polygon problem which is taken care 
    of in assets/shaders/compute/physics.glsl in the first section of the main function
    */
    
    //Circle circle = Circle(glm::vec3(0., 200., 0.), 100.0, object_id++, 1.0, 0.05,0.01,20);
    //circle.insert(&vertex_data, &object_data, &indices, &triangle_indices, &spring_indices, &spring_data);
    //ResolutionRectangle r = ResolutionRectangle(glm::vec3(0., 1000.0, 0.), 20.0, 40, 40, object_id++, 1.0, 0.05, 0.05);
    //r.insert(&vertex_data, &object_data, &indices, &triangle_indices, &spring_indices, &spring_data);
    

    //lots of OpenGL boilerplate
    glCreateBuffers(1, &vertexDataBuffer);

    glNamedBufferStorage(vertexDataBuffer,
        sizeof(VertexData) * vertex_data.size(),
        (const void*)vertex_data.data(),
        GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &objectDataBuffer);

    glNamedBufferStorage(objectDataBuffer, 
        sizeof(ObjectData) * object_data.size(),
        (const void*)object_data.data(),
        GL_DYNAMIC_STORAGE_BIT
    );

    glCreateBuffers(1, &springsBuffer);

    glNamedBufferStorage(springsBuffer,
        sizeof(Spring) * spring_data.size(),
        (const void*)spring_data.data(),
        GL_DYNAMIC_STORAGE_BIT
    );

    glCreateBuffers(1, &indicesBuffer);

    glNamedBufferStorage(indicesBuffer,
        sizeof(unsigned int) * indices.size(),
        (const void*)indices.data(),
        GL_DYNAMIC_STORAGE_BIT
    );

    glGenVertexArrays(1, &vertexVAO);
    glBindVertexArray(vertexVAO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &springVAO);
    glBindVertexArray(springVAO);
    unsigned int springEBO;
    glGenBuffers(1, &springEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, springEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * spring_indices.size(), spring_indices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &triangleVAO);
    glBindVertexArray(triangleVAO);
    unsigned int triangleEBO;
    glGenBuffers(1, &triangleEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangle_indices.size(), triangle_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexDataBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, objectDataBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, springsBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, indicesBuffer);

    init_object_buffer->use();
    glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    init_indices_boundaries->use();
    glDispatchCompute((unsigned int)ceil(indices.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    resetCenter->use();
    glDispatchCompute((unsigned int)ceil(object_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    findCenters->use();
    glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    averageCenters->use();
    glDispatchCompute((unsigned int)ceil(object_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    findOffsets->use();
    glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    init_object_properties->use();
    glDispatchCompute((unsigned int)ceil(object_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    Input::init(window->getWindow());

    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    camera = new Camera2D(glm::vec3(0.0f, 500.0f, 1.0f), glm::vec3(0.0,0.0,0.0), 1.5f, window);
    
    return 0;
}
void Game::draw() {
    camera->update(deltaTime);

    currentTime = (float)glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = (float)glfwGetTime();

    //clear buffers
    if (render_mode == 1) {
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    }
    else {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //camera uniforms
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();

    //points
    if (render_mode == 2 || render_mode == 3) {
        point_shader->use();
        point_shader->setMatrix("view", view);
        point_shader->setMatrix("projection", projection);
        point_shader->setFloat("zoom", camera->zoom);
        
        point_shader->set3DVector("color", glm::vec3(1.0, 1.0, 1.0));

        glBindVertexArray(vertexVAO);
        glDrawArrays(GL_POINTS, 0, vertex_data.size());
    }

    //center of mass visualization
    if (render_mode == 3 || render_mode == 4) {
        center_shader->use();
        center_shader->setMatrix("view", view);
        center_shader->setMatrix("projection", projection);
        center_shader->setFloat("zoom", camera->zoom);
        glBindVertexArray(vertexVAO);
        glDrawArrays(GL_POINTS, 0, object_data.size());
    }

    //edges
    glLineWidth(3.0 / camera->zoom);
    if (render_mode == 1 || render_mode == 2) {
        glBindVertexArray(vertexVAO);
        line_shader->use();
        line_shader->setMatrix("view", view);
        line_shader->setMatrix("projection", projection);
        line_shader->setFloat("zoom", camera->zoom);
        if (render_mode == 1) {
            line_shader->set3DVector("color", glm::vec3(0.0, 0.0, 0.0));
        }
        else if (render_mode == 2) {
            line_shader->set3DVector("color", glm::vec3(1.0, 1.0, 1.0));
        }
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    
    //filled polygons
    if (render_mode == 1) {
        glBindVertexArray(triangleVAO);
        triangle_shader->use();
        triangle_shader->setMatrix("view", view);
        triangle_shader->setMatrix("projection", projection);
        triangle_shader->setFloat("zoom", camera->zoom);
        triangle_shader->set3DVector("color", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, triangle_indices.size(), GL_UNSIGNED_INT, 0);
    }

    //render springs
    if (render_mode == 2) {
        glBindVertexArray(springVAO);
        line_shader->use();
        line_shader->setMatrix("view", view);
        line_shader->setMatrix("projection", projection);
        line_shader->setFloat("zoom", camera->zoom);
        line_shader->set3DVector("color", glm::vec3(1.0, 1.0, 0.0));
        glDrawElements(GL_LINES, spring_indices.size(), GL_UNSIGNED_INT, 0);
    }

    //displays the shape matching frame and what the shape should look like in virtual space
    if (render_mode == 4) {
        glBindVertexArray(vertexVAO);
        shape_match_shader->use();
        shape_match_shader->use();
        shape_match_shader->setMatrix("view", view);
        shape_match_shader->setMatrix("projection", projection);
        shape_match_shader->setFloat("zoom", camera->zoom);
        shape_match_shader->set3DVector("color", glm::vec3(1.0, 1.0, 0.0));
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    }

    //bounding box mode
    if (render_mode == 5) {
        if (ticked) {
            calculate_bounds->use();
            glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            findBounds->use();
            glDispatchCompute((unsigned int)ceil(object_data.size() / (float)workgroup_size), 1, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        }

        polygon_shader->use();
        polygon_shader->setMatrix("view", view);
        polygon_shader->setMatrix("projection", projection);
        polygon_shader->setFloat("zoom", camera->zoom);
        polygon_shader->set3DVector("color", glm::vec3(1.0, 1.0, 1.0));
        glBindVertexArray(vertexVAO);
        glDrawArrays(GL_TRIANGLES, 0, object_data.size() * 6);
    }
}
void Game::update_state() {
    GLFWwindow* gWindow = window->getWindow();
    if (glfwGetKey(gWindow, GLFW_KEY_1)) {
        render_mode = 1;
    }
    if (glfwGetKey(gWindow, GLFW_KEY_2)) {
        render_mode = 2;
    }
    if (glfwGetKey(gWindow, GLFW_KEY_3)) {
        render_mode = 3;
    }
    if (glfwGetKey(gWindow, GLFW_KEY_4)) {
        render_mode = 4;
    }
    if (glfwGetKey(gWindow, GLFW_KEY_5)) {
        render_mode = 5;
    }
    if (glfwGetKey(window->getWindow(), GLFW_KEY_C)) {
        timestep = slowed_timestep;
    }
    else {
        timestep = normal_timestep;
    }
}
void Game::tick(float timestep) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexDataBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, objectDataBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, springsBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, indicesBuffer);

    resetCenter->use();
    glDispatchCompute((unsigned int)ceil(object_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    findCenters->use();
    glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    averageCenters->use();
    glDispatchCompute((unsigned int)ceil(object_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    find_angles->use();
    glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    averageAngles->use();
    glDispatchCompute((unsigned int)ceil(object_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    gravity->use();
    glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    computeSprings->use();
    glDispatchCompute((unsigned int)ceil(spring_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    shapeMatching->use();
    shapeMatching->setFloat("match_factor", 0.1f);
    shapeMatching->setFloat("damp_factor", 0.01f);
    glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    integrate->use();
    glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    computePhysics->use();
    glDispatchCompute((unsigned int)ceil(vertex_data.size() / (float)workgroup_size), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

}
int Game::run() {
    
    int init = window->initialize();
    if (init == -1) {
        return -1;
    }
    //initialize
    setup();
    GLFWwindow* glfwWindow = window->getWindow();

    double t = 0.0;
    //timestep = 0.1;
    double currentTime = glfwGetTime();
    double accumulator = 0.0;

    while (!glfwWindowShouldClose(glfwWindow))
    {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        if (frameTime > 0.25) {
            frameTime = 0.25;
        }
        currentTime = newTime;
        accumulator += frameTime;

        window->processInput();
        update_state();

        //logic
        ticked = false;
        while (accumulator >= timestep) {
            ticked = true;
            tick((float)timestep);
            t += timestep;
            accumulator -= timestep;
        }
        draw();
        Input::yScrollOffset = 0;
        Input::xScrollOffset = 0;
        //check and call events and swap the buffers
        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}