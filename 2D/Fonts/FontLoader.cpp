#include "FontLoader.h"
#include "Font.h"
#include "../../Error Handler/ErrorHandling.h"

FT_Library Aela::FontLoader::freetype = nullptr;

Aela::FontLoader::FontLoader() = default;

bool Aela::FontLoader::load(ResourceMap& resources, std::string src) {
	if (freetype == nullptr) {
		FT_Error error = FT_Init_FreeType(&freetype);
		if (error) {
			AelaErrorHandling::consoleWindowError("Font Loader", "Could not initiate FreeType.");
			return false;
		}
	}

	FT_Face face;
	FT_Error error = FT_New_Face(freetype, src.c_str(), 0, &face);

	if (error == FT_Err_Unknown_File_Format) {
		AelaErrorHandling::consoleWindowError("Project Aela's Font Manager", "The font " + src + " uses an unsupported file format.");
		return false;
	} else if (error) {
		AelaErrorHandling::consoleWindowError("Project Aela's Font Manager", "Could not open the font " + src + ".");
	}

	Font* font = new Font(src, face);
	resources.put(src, font);
	return true;
}
