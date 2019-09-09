/*
* Class: SkyboxEntity
* Author: Robert Ciborowski
* Date: 12/08/2017
* Description: A class used by Aela's GLRenderer to store properties of a skybox entity.
*              Keep in mind that a skybox entity can move and animate, making it behave
*              like other entities.
*/

#pragma once

#include "../../Entities/Entity.h"
#include "Skybox.h"

namespace Aela {
	class SkyboxEntity : public Entity {
		public:
			SkyboxEntity() {

			}

			// These are getters and setters.
			void setSkybox(Skybox* skybox);
			Skybox* getSkybox();
			EntityType getEntityType();

		private:
			// For now, a skybox entity only stores a skybox. In the future, it will do much more!
			Skybox* skybox;
	};
}