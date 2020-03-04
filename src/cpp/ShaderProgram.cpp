#include "App.h"
ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* computeShaderPath){
    shaderProgramID = glCreateProgram();
    
    if(vertexShaderPath != NULL){
        vertexShaderID = LoadShader(vertexShaderPath, GL_VERTEX_SHADER);
        glAttachShader(shaderProgramID, vertexShaderID);
        glBindAttribLocation(shaderProgramID, 0, "vertexPosition");
        glBindAttribLocation(shaderProgramID, 1, "textureCoordinate");
    }
    if(fragmentShaderPath != NULL){
        fragmentShaderID = LoadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
        glAttachShader(shaderProgramID, fragmentShaderID);
    
    }
    if(computeShaderPath != NULL){
        computeShaderID = LoadShader(computeShaderPath, GL_COMPUTE_SHADER);
        glAttachShader(shaderProgramID, computeShaderID);
    }
    
    glLinkProgram(shaderProgramID);
    int isLinked = 0;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE){
        int maxLength = 0;
        char* err = new char[maxLength + 1];
        glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);
        glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, err);
        std::cout<<err;
        glDeleteProgram(shaderProgramID);
        return;
    }
    LoadUniforms();
}
void ShaderProgram::DeleteProgram(){
    glDetachShader(shaderProgramID, vertexShaderID);
    glDeleteShader(vertexShaderID);
    glDetachShader(shaderProgramID, fragmentShaderID);
    glDeleteShader(fragmentShaderID);
    glDetachShader(shaderProgramID, computeShaderID);
    glDeleteShader(computeShaderID);
    glUseProgram(0);
    glDeleteProgram(shaderProgramID);
}
unsigned int ShaderProgram::LoadShader(const char* path, unsigned int type){
    std::string shaderString;
    std::ifstream sourceFile( path );
    if( sourceFile ){
        shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
    }
    const char* source = shaderString.c_str();
    const int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);
    int isCompiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){
        int maxLength = 0;
        char* err = new char[maxLength + 1];
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, err);
        std::cout<<err;
        glDeleteShader(shaderID);
    }

    return shaderID;
}
void ShaderProgram::LoadUniforms(){
    for(int j=0; j < MAX_SPHERES; j++){
        std::stringstream ss;
        ss<<j;
        std::string jStr = ss.str();
        std::string locStr = "spheres[" + jStr + "].center";
        locations.spherePositions.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "spheres[" + jStr +"].color";
        locations.sphereColors.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "spheres[" + jStr + "].radius2";
        locations.sphereRadius2.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "spheres[" + jStr + "].inv_radius";
        locations.sphereInvRadius.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
    }

    for(int j=0; j < MAX_CYLINDERS; j++){
        std::stringstream ss;
        ss<<j;
        std::string jStr = ss.str();
        std::string locStr = "cylinders[" + jStr +"].pos";
        locations.cylinderPositions.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "cylinders[" + jStr + "].axis";
        locations.cylinderAxes.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "cylinders[" + jStr + "].color";
        locations.cylinderColors.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "cylinders[" + jStr + "].radius";
        locations.cylinderRadius.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "cylinders[" + jStr + "].length";
        locations.cylinderLengths.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
    }

    for(int j = 0; j < 3; j++){
        std::stringstream ss;
        ss<<j;
        std::string jStr = ss.str();
        std::string locStr = "squares[" + jStr +"].pos";
        locations.squarePositions.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "squares[" + jStr +"].color";
        locations.squareColors.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "squares[" + jStr +"].norm";
        locations.squareNormals.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "squares[" + jStr +"].uDir";
        locations.squareUDirections.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "squares[" + jStr +"].vDir";
        locations.squareVDirections.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
        locStr = "squares[" + jStr +"].length";
        locations.squareLengths.push_back(glGetUniformLocation(shaderProgramID, locStr.c_str()));
    }

    locations.cameraDirection = glGetUniformLocation(shaderProgramID, "camera_dir");
    locations.cameraUp = glGetUniformLocation(shaderProgramID, "camera_up");
    locations.cameraRight = glGetUniformLocation(shaderProgramID, "camera_right");
    locations.cameraPosition = glGetUniformLocation(shaderProgramID, "camera_pos");
    locations.lightPosition = glGetUniformLocation(shaderProgramID, "light");
    locations.time = glGetUniformLocation(shaderProgramID, "time");
    locations.numCylinders = glGetUniformLocation(shaderProgramID, "numCylinders");
    locations.numSpheres = glGetUniformLocation(shaderProgramID, "numSpheres");
    locations.texture = glGetUniformLocation(shaderProgramID, "renderedTexture");
}