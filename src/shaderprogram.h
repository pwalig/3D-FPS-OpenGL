/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GL/glew.h"
#include "stdio.h"

class ShaderProgram {
private:
	GLuint shaderProgram; //Shader program handle
	GLuint vertexShader; //Vertex shader handle
	GLuint geometryShader; //Geometry shader handle
	GLuint fragmentShader; //Fragment shader handle
	char* readFile(const char* fileName); //File reading method
	GLuint loadShader(GLenum shaderType,const char* fileName); //Method reads shader source file, compiles it and returns the corresponding handle
public:
	ShaderProgram(const char* vertexShaderFile,const char* geometryShaderFile,const char* fragmentShaderFile);
	~ShaderProgram();
	void use(); //Turns on the shader program
	GLuint u(const char* variableName); //Returns the slot number corresponding to the uniform variableName
	GLuint a(const char* variableName); //Returns the slot number corresponding to the attribute variableName
};

extern ShaderProgram *spConstant;
extern ShaderProgram *spLambert;
extern ShaderProgram* spTextured;
extern ShaderProgram* spColored;
extern ShaderProgram* spLambertTextured;

void initShaders();
void freeShaders();

#endif
