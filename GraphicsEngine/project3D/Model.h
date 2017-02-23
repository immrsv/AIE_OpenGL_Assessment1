#pragma once

#include <map>
#include <FBXFile.h>

class Model
{
public:
	Model(std::string filename);
	~Model();
	
	void draw();

	FBXFile* m_fbx;

private:
	void pushToGfx();
};

