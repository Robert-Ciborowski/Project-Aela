/*
* Class: LightEntity
* Author: Robert Ciborowski
* Date: 07/04/2017
* Description: A class used to store the properties of a light.
*/

#pragma once
#include "../Transformable/Transformable3D.h"
#include "../../Utilities/Colour/ColourRGB.h"
#include "../../Entities/Entity.h"

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

namespace Aela {
	class LightEntity : public Entity, public Transformable3D {
		public:
			LightEntity();
			LightEntity(glm::vec3 position, glm::vec3 rotation, ColourRGB* colour, float power);
			~LightEntity();

			void useDefaultValues();
			void setupForNewShadowMap();

			// These are getters and setters.
			void setColour(ColourRGB* colour);
			void setColour(float r, float g, float b);
			void setPower(float power);
			ColourRGB* getColour();
			float getPower();
			GLuint* getShadowMapTexture();
			void setShadowMapTexture(GLuint* shadowMapTexture);
			GLuint* getShadowMapBuffer();
			void setShadowMapBuffer(GLuint* shadowMapBuffer);
			EntityType getEntityType();
			void hasBeenSetUpForShadows();
			bool isSetUpForShadows();

		private:
			ColourRGB colour;
			float power;
			GLuint shadowMapTexture = 0, shadowMapBuffer = 0;

			bool setUpForShadows = false;
	};
}