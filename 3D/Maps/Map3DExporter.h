/*
* Class: Map3D Exporter
* Author: Robert Ciborowski
* Date: 22/08/2017
* Description: A class used to export a Map3D to a map file.
*/

#pragma once

#include <fstream>

#include "Map3D.h"

namespace Aela {
	class Map3DExporter {
		public:
			Map3DExporter() {

			}

			bool exportMap(std::string path, Map3D* map, bool mapIsReadable);

		private:
			std::string getTransformableString(Transformable3D* transformable);
			std::string getLightString(LightEntity* light);
			std::string getBillboardString(BillboardEntity* billboard);

			void abbreviate(std::string& src, std::string defaultPath);
	};
}