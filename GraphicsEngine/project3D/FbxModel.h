#pragma once

#include <FBXFile.h>

class FbxModel 
{
public:
	FbxModel(std::string filename);
	~FbxModel();

	virtual void draw();

	FBXFile* m_fbx;

private:
	void pushToGfx();
};

