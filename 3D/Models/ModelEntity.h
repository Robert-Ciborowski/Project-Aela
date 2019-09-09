/*
* Class: ModelEntity
* Author: Robert Ciborowski
* Date: 06/08/2017
* Description: A class used by Aela's GLRenderer to store properties of a 3D entity.
*              A 3D entity is an entity that is made up by a model in worldspace.
*              Note: This class used to be known as "Model" but was changed.
*              Please keep in mind that the model data is stored in a seperate class
*              since Models are resources.
*/

#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>
#include <iostream>

#include "../../Entities/Entity.h"
#include "../Transformable/Transformable3D.h"
#include "Model.h"
#include "../Bounding Box/BoundingBox3D.h"

namespace Aela {
	class ModelEntity : public Entity, public Transformable3D {
		public:
			ModelEntity() {

			}

			ModelEntity(Model* model) {
				this->model = model;
			}

			~ModelEntity() {
				
			}

			// These are the getters and setters.
			Model* getModel();
			void setModel(Model* model);
			BoundingBox3D* getBoundingBox();
			void generateBoundingBox();
			EntityType getEntityType();

		private:
			// The model stores all of the vertex, UV and normal data of the ModelEntity.
			Model* model;

			// The bounding box is the smallest box that the model fits inside of and is used for physics. The position of the center
			// of the bounding box is what the bounding box's position represents. The width, height and depth represent how far the
			// bounding box reaches from its center.
			BoundingBox3D boundingBox;
	};
}