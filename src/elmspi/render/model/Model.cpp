#include "Model.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>
#include "../../maths/Maths.h"

static struct QuadFace
{
	unsigned int vert1, vert2, vert3, vert4;
	unsigned int texC1, texC2, texC3, texC4;
	unsigned int norm1, norm2, norm3, norm4;
};

static bool doesFileExist(const char* fileName)
{
	struct stat buffer;
	return (stat(fileName, &buffer) == 0);
}
static bool isCharNumerical(const char& theChar)
{
	return theChar == '-' || theChar == '.' || (47 < theChar && theChar < 58);
}
espi::vec3f parse3f(const char* line, size_t lineSize)
{
	char floatAsString[32] = {};
	uint8_t pos = 0;
	uint8_t floatsGotten = 0;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	for (int i = 0; i < lineSize; i++)
	{
		char c = line[i];
		if (isCharNumerical(c))
		{
			do
			{
				floatAsString[pos++] = c;
				i++;
				c = line[i];
			} 
			while (isCharNumerical(c) && i < lineSize);
			pos = 0;
			switch (floatsGotten)
			{
			case 0: {x = atof(floatAsString); break; }
			case 1: {y = atof(floatAsString); break; }
			case 2: {z = atof(floatAsString); break; }
			}
			floatsGotten++;
			memset(floatAsString, 0, 32);
		}
		if (floatsGotten == 3) break;
	}
	return espi::vec3f(x, y, z);
}
espi::vec2f parse2f(const char* line, size_t lineSize)
{
	char floatAsString[32] = {};
	uint8_t pos = 0;
	uint8_t floatsGotten = 0;
	float x = 0.0f;
	float y = 0.0f;
	for (int i = 0; i < lineSize; i++)
	{
		char c = line[i];
		if (isCharNumerical(c))
		{
			do
			{
				floatAsString[pos++] = c;
				i++;
				c = line[i];
			} 
			while (isCharNumerical(c) && i < lineSize);
			pos = 0;
			switch (floatsGotten)
			{
			case 0: { x = atof(floatAsString); break; }
			case 1: {y = atof(floatAsString); break; }
			}
			floatsGotten++;
			memset(floatAsString, 0, 32);
		}
		if (floatsGotten == 2) break;
	}
	return espi::vec2f(x, y);
}
QuadFace parseQuadFace(const char* line, size_t lineSize)
{
	char intAsString[32] = {};
	uint8_t pos = 0;
	uint8_t vertInfoIndex = 0;
	unsigned int ints[12] = {};
	for (int i = 0; i < lineSize; i++)
	{
		char c = line[i];
		if (isCharNumerical(c))
		{
			do
			{
				intAsString[pos++] = c;
				i++;
				c = line[i];
			} 
			while (isCharNumerical(c) && i < lineSize);
			pos = 0;
			ints[vertInfoIndex] = atoi(intAsString) - 1;
			memset(intAsString, 0, 32);
		}
		if (c == '/' || c == ' ' && vertInfoIndex > 0 || c == 0) vertInfoIndex++;
		if (vertInfoIndex == 12) break;
	}
	QuadFace face;
	face.vert1 = ints[0]; face.vert2 = ints[3]; face.vert3 = ints[6]; face.vert4 = ints[9];
	face.texC1 = ints[1]; face.texC2 = ints[4]; face.texC3 = ints[7]; face.texC4 = ints[10];
	face.norm1 = ints[2]; face.norm2 = ints[5]; face.norm3 = ints[8]; face.norm4 = ints[11];
	return QuadFace(face);
}

Model::Model(const char* fileName)
{
	if (!doesFileExist(fileName)) throw std::invalid_argument("[Model] : The file for model loading does not exist!");
	std::string s(fileName);
	if (s.find(".obj") == std::string::npos) throw std::invalid_argument("[Model] : Cannot load file '" + s + "' for model loading!");

	glGenBuffers(1, &modelBuffer);

	loadModel(fileName);
}

Model::~Model()
{
	glDeleteBuffers(1, &modelBuffer);
}

void Model::render()
{
	preRender();
	{
		glDrawArrays(GL_QUADS, 0, totalVertexCount);
	}
	postRender();
}

void Model::renderObject(const char* objectName)
{
	std::map<std::string, ModelObject>::iterator obj = objects.find(std::string(objectName));
	if (obj == objects.end()) return;

	preRender();
	{
		glDrawArrays(GL_QUADS, obj->second.startingVertex, obj->second.vertexCount);
	}
	postRender();
}

void Model::loadModel(const char* fileName)
{
	using namespace espi;

	std::ifstream modelFile(fileName, std::ios::in | std::ios::ate);
	size_t fileSize = modelFile.tellg();
	modelFile.seekg(0, std::ios::beg);
	char* fileData = new char[fileSize];
	modelFile.read(fileData, fileSize);
	modelFile.close();

	char* line = fileData;
	int lineSize = 0;

	unsigned int numOfVertices = 0;
	unsigned int numOfTexCoords = 0;
	unsigned int numOfNormals = 0;
	unsigned int numOfFaces = 0;

	for (unsigned int i = 0; i < fileSize; i++)
	{
		char c = fileData[i];
		lineSize++;
		if (c == '\n')
		{
			if (line[0] == 'f') numOfFaces++;
			else if (line[0] == 'v' && line[1] == 'n') numOfNormals++;
			else if (line[0] == 'v' && line[1] == 't') numOfTexCoords++;
			else if (line[0] == 'v') numOfVertices++;
			lineSize = 0;
			line = &fileData[i] + 1;
		}
	} line = fileData;

	vec3f* vertices = new vec3f[numOfVertices]; unsigned int vertIndex = 0;
	vec2f* texCoords = new vec2f[numOfTexCoords]; unsigned int texCIndex = 0;
	vec3f* normals = new vec3f[numOfNormals]; unsigned int normIndex = 0;
	QuadFace* faces = new QuadFace[numOfFaces]; unsigned int faceIndex = 0;
	unsigned int objIndex = 0;

	unsigned int vertexCounted = 0;
	unsigned int startingVertex = vertexCounted;
	unsigned int vertexCount = 0;
	char objName[64];
	for (unsigned int i = 0; i < fileSize; i++)
	{
		char c = fileData[i];
		lineSize++;
		if (c == '\n')
		{
			if (line[0] == 'f') { faces[faceIndex++] = parseQuadFace(line, lineSize); vertexCounted += 4; vertexCount += 4; }
			else if (line[0] == 'o')
			{
				if (objIndex > 0)
				{
					ModelObject o; o.startingVertex = startingVertex; o.vertexCount = vertexCount;
					addObject(o, objName);
					/*Debug*/ std::cout << '[' << objName << "] : Staring Vertex = " << objects[objName].startingVertex << ", Vertex Count = " << objects[objName].vertexCount << std::endl;
					startingVertex = vertexCounted;
					vertexCount = 0;
				}
				memset(objName, 0, 64);
				memcpy(objName, &line[2], lineSize - 3 > 64 ? 64 : lineSize - 3);
				objIndex++;
			}
			else if (line[0] == 'v' && line[1] == 'n') normals[normIndex++] = parse3f(line, lineSize);
			else if (line[0] == 'v' && line[1] == 't') { vec2f coord = parse2f(line, lineSize); texCoords[texCIndex++] = vec2f(coord.u, -coord.v + 1.0f); }
			else if (line[0] == 'v') vertices[vertIndex++] = parse3f(line, lineSize);
			lineSize = 0;
			line = &fileData[i] + 1;
		}
	}
	ModelObject o; o.startingVertex = startingVertex; o.vertexCount = vertexCount;
	addObject(o, objName);
	/*Debug*/ std::cout << '[' << objName << "] : Staring Vertex = " << objects[objName].startingVertex << ", Vertex Count = " << objects[objName].vertexCount << std::endl;

	float* modelData = new float[numOfFaces * 32];
	float* dataPointer = modelData;

	for (int i = 0; i < numOfFaces; i++)
	{
		QuadFace face = faces[i];
		vec3f vert1 = vertices[face.vert1], vert2 = vertices[face.vert2], vert3 = vertices[face.vert3], vert4 = vertices[face.vert4];
		vec2f texC1 = texCoords[face.texC1], texC2 = texCoords[face.texC2], texC3 = texCoords[face.texC3], texC4 = texCoords[face.texC4];
		vec3f norm1 = normals[face.norm1], norm2 = normals[face.norm2], norm3 = normals[face.norm3], norm4 = normals[face.norm4];
		float faceData[32] = {
			vert1.x, vert1.y, vert1.z, texC1.u, texC1.v, norm1.x, norm1.y, norm1.z,

			vert2.x, vert2.y, vert2.z, texC2.u, texC2.v, norm2.x, norm2.y, norm2.z,

			vert3.x, vert3.y, vert3.z, texC3.u, texC3.v, norm3.x, norm3.y, norm3.z,

			vert4.x, vert4.y, vert4.z, texC4.u, texC4.v, norm4.x, norm4.y, norm4.z
		};
		memcpy(dataPointer, faceData, sizeof(faceData));
		dataPointer += 32;
	}

	delete[] vertices;
	delete[] texCoords;
	delete[] normals;
	delete[] faces;
	delete[] fileData;

	totalVertexCount = numOfFaces * 4;
	glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
	glBufferData(GL_ARRAY_BUFFER, numOfFaces * 32 * sizeof(float), modelData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] modelData;
}

void Model::addObject(const ModelObject& object, const char* objectName)
{
	objects[std::string(objectName)] = object;
}

void Model::preRender()
{
	glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 8, (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*) (sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, true, 8 * sizeof(float), (void*)(sizeof(float) * 5));
}

void Model::postRender()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


