#pragma once

#ifndef __F3D_MESH_H
#define __F3D_MESH_H

#include <memory>
#include <cstdint>
#include <vector>

#include "core/platform.h"
#include "utils/transform.h"

namespace f3d {
	namespace tree {
		class F3D_API Mesh {
		public:
			Mesh();
			~Mesh();
			
			std::string		getName() const;
			void			setName(const std::string val);

			std::string		getMaterialName() const;
			void			setMaterialName(const std::string val);


			uint32_t		numVertices() const;
			void			addVertex(const float x, const float y, const float z);
			void			addVertex(glm::vec3& v);

			uint32_t		numNormals() const;
			void			addNormal(const float x, const float y, const float z);
			void			addNormal(glm::vec3& n);

			uint32_t		numIndices() const;
			uint32_t		numTriangles() const;
			void			addTriangle(uint32_t a, uint32_t b, uint32_t c);

			uint32_t		numUV() const;
			void			addUV(float u, float v);

			virtual bool			makeRenderReady() = 0;
			virtual bool			isRenderReady() const;

		protected:
			bool						_ready;
			std::string					_name;
			std::string					_material_name;
			uint32_t					_vertices_count;
			std::vector<float>			_vertices;
			uint32_t					_normals_count;
			std::vector<float>			_normals;
			uint32_t					_indices_count;
			std::vector<uint32_t>		_indices;
			uint32_t					_uv_count;
			std::vector<float>			_uvs;
			std::vector<float>			_colors;
		};
	}
}

#endif