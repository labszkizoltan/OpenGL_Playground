#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"

static std::string ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::stringstream ss;

	std::string line;
	while (getline(stream, line))
	{
		ss << line << '\n';
	}

	return ss.str();
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	// source should exist at this point
	const char* src = source.c_str(); // equivalent to &source[0]
	GLCall( glShaderSource(id, 1, &src, nullptr) );
	GLCall( glCompileShader(id) );

	// TODO: Error handling
	int result;
	GLCall( glGetShaderiv(id, GL_COMPILE_STATUS, &result) );
	if (result == GL_FALSE)
	{
		int length;
		GLCall( glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length) );
		char* message = (char*)alloca(length*sizeof(char));

		GLCall( glGetShaderInfoLog(id, length, &length, message) );
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n";
		std::cout << message << std::endl;
		GLCall( glDeleteShader(id) );
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall( glAttachShader(program, vs) );
	GLCall( glAttachShader(program, fs) );
	GLCall( glLinkProgram(program) );
	GLCall( glValidateProgram(program) );

	GLCall( glDeleteShader(vs) );
	GLCall( glDeleteShader(fs) );

	return program;
}



