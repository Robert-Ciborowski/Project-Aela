/*
* Class: Specifications Manager
* Author: Robert Ciborowski
* Date: 18/08/2017
* Description: A class for finding and storing the specifications of the user's machine.
*/

#pragma once
#include "../Renderer/GLRenderer.h"

namespace Aela {
	class UserEnvironment {
		public:
			UserEnvironment() {

			}

			UserEnvironment(GLRenderer* renderer) {
				generateInfo(renderer);
			}

			// This generates the info on the user's machine.
			void generateInfo(GLRenderer* renderer);

			// These are getters and setters for properties that won't change duing runtime:
			std::vector<std::string>* getAudioDeviceNames();
			std::vector<std::string>* getAudioDeviceDrivers();
			std::string getCurrentAudioDriver();
			std::string getGraphicsVendor();
			std::string getGraphicsRenderer();
			std::string getOpenGLVersion();
			std::string getGLSLVersion();
			std::string getOpenGLExtensions();
			std::string getPlatform();
			unsigned int getL1CacheLineSize();
			unsigned int getLogicalCPUCores();
			unsigned int getSystemRAM();
			void getNativeDisplayResolution(int displayID, int* width, int* height);
			int getNativeDisplayWidth(int displayID);
			int getNativeDisplayHeight(int displayID);
			void getDisplayDPI(int displayID, float* diagonalDPI, float* horizontalDPI, float* verticalDPI);
			float getDisplayDPIDiagonal(int displayID);
			float getDisplayDPIWidth(int displayID);
			float getDisplayDPIHeight(int displayID);

		private:
			// Things that will never change:
			std::vector<std::string> audioDeviceNames;
			std::vector<std::string> audioDeviceDrivers;
			std::string currentAudioDriver;
			std::string graphicsVendor, graphicsRenderer, openGLVersion, GLSLVersion, openGLExtensions;
			std::string platform;	
			unsigned int l1CacheLineSize, logicalCPUCores, systemRAM;
	};
}
