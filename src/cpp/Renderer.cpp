#include "App.h"
Renderer::Renderer(unsigned int shaderProgramID){
    computeProgramID = shaderProgramID;
    vaoID = 0;
    glCreateVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    vertexBufferID = 0;
    glCreateBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    const float vertices[] = {-1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glCreateBuffers(1, &elementBufferID);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
    const unsigned short elements[6] = {0, 1, 2, 0, 2, 3};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glCreateBuffers(1, &uvBufferID);
    glEnableVertexAttribArray(1);
    const float texCoords[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    int work_grp_cnt[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
    std::cout<<"max global (total) work group size x: "<<work_grp_cnt[0]<<", "<<work_grp_cnt[1]<<", "<<work_grp_cnt[2]<<"\n";

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_cnt[2]);
    std::cout<<"max local (total) work group size x: "<<work_grp_cnt[0]<<", "<<work_grp_cnt[1]<<", "<<work_grp_cnt[2]<<"\n";
    
    int work_grp_inv;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    std::cout<<"max local work group invocations: "<<work_grp_inv<<"\n";
    
    int max_uniform_count;
    glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &max_uniform_count);
    std::cout<<"max uniforms in compute shader: "<<work_grp_inv<<"\n";
    int output = 0;
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &output);
    std::cout<<"max buffer bindings: "<<output<<"\n";
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &output);
    std::cout<<"max block size: "<<output<<"\n";
    glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &output);
    std::cout<<"max vertex blocks: "<<output<<"\n";
    glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &output);
    std::cout<<"max frame blocks: "<<output<<"\n";
    glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &output);
    std::cout<<"max compute blocks: "<<output<<"\n";
    glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &output);
    std::cout<<"max total blocks: "<<output<<"\n";

    glBindVertexArray(NULL);
}
void Renderer::DrawScence(unsigned int shaderProgramID){
    glClearColor(0.2f, 0.2f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgramID);

    glBindVertexArray(vaoID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    glDisableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, NULL);
    glBindVertexArray(NULL);
    glUseProgram(NULL);
}

void Renderer::UseComputeShader(){
    const float speed = 4.0f;
    glUseProgram(computeProgramID);
    if(cylinder_positions.size() > 0){
        cylinder_lengths.back() += delta_time / 10.0f * speed;
        sphere_positions.back() += delta_time / 10.0f * speed * cylinder_axes.back();
        if(!grow){
            sphere_positions.front() += delta_time / 10.0f * speed * cylinder_axes.front();
            cylinder_positions.front() += delta_time / 10.0f * speed * cylinder_axes.front();
            cylinder_lengths.front() -= delta_time / 10.0f * speed;
            if(cylinder_lengths.front() <= 0.0){
                std::rotate(sphere_positions.begin(), sphere_positions.begin() + 1, sphere_positions.end());
                sphere_positions.pop_back();
                std::rotate(sphere_colors.begin(), sphere_colors.begin() + 1, sphere_colors.end());
                sphere_colors.pop_back();
                std::rotate(sphere_radius2.begin(), sphere_radius2.begin() + 1, sphere_radius2.end());
                sphere_radius2.pop_back();
                std::rotate(sphere_inv_radius.begin(), sphere_inv_radius.begin() + 1, sphere_inv_radius.end());
                sphere_inv_radius.pop_back();

                std::rotate(cylinder_positions.begin(), cylinder_positions.begin() + 1, cylinder_positions.end());
                cylinder_positions.pop_back();
                std::rotate(cylinder_axes.begin(), cylinder_axes.begin() + 1, cylinder_axes.end());
                cylinder_axes.pop_back();
                std::rotate(cylinder_colors.begin(), cylinder_colors.begin() + 1, cylinder_colors.end());
                cylinder_colors.pop_back();
                std::rotate(cylinder_lengths.begin(), cylinder_lengths.begin() + 1, cylinder_lengths.end());
                cylinder_lengths.pop_back();
                std::rotate(cylinder_radius.begin(), cylinder_radius.begin() + 1, cylinder_radius.end());
                cylinder_radius.pop_back();
            }
        }
    }
    lightPosition = quatRot(lightPosition, glm::vec3(0.0f, 1.0f, 0.0f), delta_time / 1000.f, glm::vec3(1100.0f, 2800.0f, 1100.0f));
    
    glUniform3fv(locations.lightPosition, 1, glm::value_ptr(lightPosition));
    glUniform1f(locations.time, delta_time);
    glUniform1i(locations.numSpheres, sphere_positions.size());
    glUniform1i(locations.numCylinders, cylinder_positions.size());

    for(int j=0; j < sphere_positions.size(); j++){
        glUniform3fv(locations.spherePositions[j], 1, glm::value_ptr(sphere_positions[j]));
        glUniform3fv(locations.sphereColors[j], 1, glm::value_ptr(sphere_colors[j]));
        glUniform1f(locations.sphereRadius2[j], sphere_radius2[j]);
        glUniform1f(locations.sphereInvRadius[j], sphere_inv_radius[j]);
    }

    for(int j=0; j < cylinder_positions.size(); j++){
        glUniform3fv(locations.cylinderPositions[j], 1, glm::value_ptr(cylinder_positions[j]));
        glUniform3fv(locations.cylinderAxes[j], 1, glm::value_ptr(cylinder_axes[j]));
        glUniform3fv(locations.cylinderColors[j], 1, glm::value_ptr(cylinder_colors[j]));
        glUniform1f(locations.cylinderRadius[j], cylinder_radius[j]);
        glUniform1f(locations.cylinderLengths[j], cylinder_lengths[j]);
    }

    glDispatchCompute((unsigned int)(WIDTH / 4), (unsigned int)(HEIGHT / 4), 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
void Renderer::DeleteBuffers(){
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &elementBufferID);
}
void Renderer::CreateObjects(){
    cameraDirection = glm::vec3(-0.48f, -0.51f, -glm::sqrt(1.0f - 0.48f * 0.48f - 0.51f * 0.51f));
    cameraRight = glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f));
    cameraUp = glm::cross(cameraRight, cameraDirection);
    cameraPosition = glm::vec3(2550.0f, 2700.0f, 3750.0f);
    
    AddSphere(glm::vec3(900.0f, 900.0f, 900.0f), glm::vec3(0.75f, 0.75f, 0.75f), 80.0f);
    AddSquare(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.4f, 0.78f, 0.8f));
    AddSquare(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.8f, 0.65f, 0.6f));
    AddSquare(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.8f, 0.90f, 0.7f));
    lightPosition = glm::vec3(1400.0f, 2800.0f, 1400.0f);
    glUseProgram(computeProgramID);
    for(int j=0; j < square_positions.size(); j++){
        glUniform3fv(locations.squarePositions[j], 1, glm::value_ptr(square_positions[j]));
        glUniform3fv(locations.squareNormals[j], 1, glm::value_ptr(square_normals[j]));
        glUniform3fv(locations.squareUDirections[j], 1, glm::value_ptr(square_uDirections[j]));
        glUniform3fv(locations.squareVDirections[j], 1, glm::value_ptr(square_vDirections[j]));
        glUniform3fv(locations.squareColors[j], 1, glm::value_ptr(square_colors[j]));
        glUniform1f(locations.squareLengths[j], square_length);
    }
    glUniform3fv(locations.cameraDirection, 1, glm::value_ptr(cameraDirection));
    glUniform3fv(locations.cameraUp, 1, glm::value_ptr(cameraUp));
    glUniform3fv(locations.cameraRight, 1, glm::value_ptr(cameraRight));
    glUniform3fv(locations.cameraPosition, 1, glm::value_ptr(cameraPosition));
    glUseProgram(NULL);
}
void Renderer::AddSphere(glm::vec3 position, glm::vec3 color, float radius){
    sphere_positions.push_back(position);
    sphere_radius2.push_back(radius * radius);
    sphere_inv_radius.push_back(1.0f / radius);
    sphere_colors.push_back(color);
}
void Renderer::AddSquare(glm::vec3 position, glm::vec3 normal, glm::vec3 uDirection, glm::vec3 vDirection, glm::vec3 color){
    square_positions.push_back(position);
    square_normals.push_back(normal);
    square_uDirections.push_back(uDirection);
    square_vDirections.push_back(vDirection);
    square_colors.push_back(color);
}
void Renderer::AddCylinder(glm::vec3 position, glm::vec3 axis, glm::vec3 color, float radius, float length){
    cylinder_positions.push_back(position);
    cylinder_axes.push_back(axis);
    cylinder_colors.push_back(color);
    cylinder_radius.push_back(radius);
    cylinder_lengths.push_back(length);

}
void Renderer::AddSegment(int orientation, float direction){
    if(cylinder_axes.size() == 0 || cylinder_axes.back()[orientation] == 0.0f){
        glm::vec3 axis = glm::vec3(0.0f, 0.0f, 0.0f);
        axis[orientation] = direction;
        
        float radius = 1.0 / sphere_inv_radius.front();
        AddCylinder(sphere_positions.back(), axis, sphere_colors.front(), radius, 0.0f);
        AddSphere(sphere_positions.back(), sphere_colors.front(), radius);
    }
}
void Renderer::UpdateUniformLocations(UniformLocations uniformLocations){
    locations = uniformLocations;
}
glm::vec3 Renderer::quatRot(glm::vec3 point, glm::vec3 axis, float angle, glm::vec3 origin){
    glm::vec3 r = axis * (float)glm::sin(angle / 2.0f);
    glm::vec3 P = point - origin;
    return point + glm::cross( 2.0f * r, glm::cross(r, P) + P * (float)glm::cos(angle/2.0f));;
}
void Renderer::loadBMP(const char* imagepath){
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3

    unsigned char* data;     // Actual RGB data
    FILE* file = fopen(imagepath,"rb");
    if (!file){printf("Image could not be opened\n"); return;}
    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return;
    }

    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    data = new unsigned char [imageSize];
    fread(data,1,imageSize,file);
    fclose(file);

    textureID = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glBindTexture(GL_TEXTURE_2D, NULL);
}
