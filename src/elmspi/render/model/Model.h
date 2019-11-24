#pragma once
#include <map>
#include <string>

class Model
{
	public:
		Model(const char* fileName);
		~Model();

		void render();
		void renderObject(const char* objectName);
	private:
		static struct ModelObject
		{
			unsigned int startingVertex;
			unsigned int vertexCount;
		};

		unsigned int modelBuffer;
		unsigned int totalVertexCount;
		std::map<std::string, ModelObject> objects;

		void loadModel(const char*);
		void addObject(const ModelObject&, const char*);
		void preRender();
		void postRender();
};

