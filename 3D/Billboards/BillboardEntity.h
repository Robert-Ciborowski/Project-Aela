/*
* Name: Project Aela's BillboardEntity class.
* Author: Robert Ciborowski
* Date: October 2016
* Description: A simple, header-only class used to store properties of a billboard.
*/

#pragma once
#include "../../Old Garbage/texture.hpp"
#include "../Transformable/Transformable3D.h"
#include "../../Entities/Entity.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Aela {
	class BillboardEntity : public Transformable3D, public Entity {
		public:
			BillboardEntity() {

			}

			BillboardEntity(Texture* texture) {
				this->texture = texture;
			}

			BillboardEntity(glm::vec3 setPosition, Texture* setTexture) {
				position = setPosition;
				texture = setTexture;
			}

			void setTexture(Texture* setTexture) {
				texture = setTexture;
			}

			Texture* getTexture() {
				return texture;
			}

			EntityType getEntityType() {
				return EntityType::BILLBOARD;
			}

			void useSpecifiedRotation(bool use) {
				useRotation = use;
			}

			bool usingSpecifiedRotation() {
				return useRotation;
			}

		private:
			Texture* texture;
			bool useRotation = true;
	};
}