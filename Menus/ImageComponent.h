/*
* Class: ImageComponent
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents an image.
*/

#pragma once
#include "Component.h"
#include "../2D/Texture/GLTexture.h"
#include "../2D/Transformable/Transformable2D.h"

namespace Aela {
	class ImageComponent : public Component {
		public:
			ImageComponent();
			ImageComponent(GLTexture* texture);
			ImageComponent(GLTexture* texture, Rect<int>* dimensions);
			ImageComponent(GLTexture* texture, Rect<int>* dimensions, Rect<int>* cropping);
			ImageComponent(GLTexture* texture, Rect<int>* dimensions, ColourRGBA* tint);
			ImageComponent(GLTexture* texture, Rect<int>* dimensions, Rect<int>* cropping, ColourRGBA* tint);
			virtual ~ImageComponent();

			virtual void updateComponent();
			virtual void renderComponent(GLRenderer& renderer);

			// These are getters and setters.
			void setTexture(GLTexture* texture);
			GLTexture* getTexture();
			void setCropping(Rect<int>* cropping);
			void setCropping(int x, int y, int width, int height);

		protected:
			GLTexture* texture = nullptr;
			Rect<int> cropping;

			// This is used to make sure that the cropping is set to the texture dimensions if it was never set.
			bool croppingWasInitialised = false;
	};
}