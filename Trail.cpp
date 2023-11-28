#include "Trail.hpp"

Trail::Trail() {
	// loading particle texture
	if (!m_texture.loadFromFile(TRAIL_TEXTURE_PATH)) {
		std::cout << "Trail::Trail() : failed loading texture from file " << TRAIL_TEXTURE_PATH << "\n\tConstruction aborted (no extra memory allocated)" << std::endl;
		return;
	}

	// dimensioning the vertexArray
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(delayLength * 4);

	// adding the textures to circles
	sf::Vector2u textureSize = m_texture.getSize();

	float sizeX = (float)(textureSize.x / 2);
	float sizeY = (float)textureSize.y;

	for (uint32_t p = 0; p < delayLength; p++) { // Applying texture of a circle and initializing vertices positions
		m_vertices[4 * p].texCoords = sf::Vector2f(0.f, 0.f);
		m_vertices[4 * p].position = sf::Vector2f(-dotSize / 2, -dotSize / 2);

		m_vertices[4 * p + 1].texCoords = sf::Vector2f(sizeX, 0.f);
		m_vertices[4 * p + 1].position = sf::Vector2f(dotSize / 2, -dotSize / 2);

		m_vertices[4 * p + 2].texCoords = sf::Vector2f(sizeX, sizeY);
		m_vertices[4 * p + 2].position = sf::Vector2f(dotSize / 2, dotSize / 2);

		m_vertices[4 * p + 3].texCoords = sf::Vector2f(0.f, sizeY);
		m_vertices[4 * p + 3].position = sf::Vector2f(-dotSize / 2, dotSize / 2);

		for (uint8_t i = 0; i < 4; i++) {
			m_vertices[4 * p + i].color.a = 255;
			m_vertices[4 * p + i].color.r = 0;
			m_vertices[4 * p + i].color.g = 255;
			m_vertices[4 * p + i].color.b = 0;
		}
	}

	float mult = 2;
	for (uint8_t i = 0; i < 4 * delayLength; i++) {
		m_vertices[i].position.x = m_vertices[i].position.x * mult;
		m_vertices[i].position.y = m_vertices[i].position.y * mult;
	}
}

void Trail::upPosture(uint64_t time) {
	if (time >= trailLength) {
		// Doesn't really work. The number of position the robot passed by is greater then the number of position of the trajectory to follow. But we don't know of how much. I should've saved both numbers
		time = trailLength-1;
	}

	for (uint8_t i = 0; i < 4*(delayLength-1); i++) {
		m_vertices[i].position.x = m_vertices[i + 4].position.x;
		m_vertices[i].position.y = m_vertices[i + 4].position.y;
	}

	m_vertices[4*(delayLength-1)    ].position = sf::Vector2f(parcBuffer[0][time] - dotSize / 2, parcBuffer[1][time] - dotSize / 2);
	m_vertices[4*(delayLength-1) + 1].position = sf::Vector2f(parcBuffer[0][time] + dotSize / 2, parcBuffer[1][time] - dotSize / 2);
	m_vertices[4*(delayLength-1) + 2].position = sf::Vector2f(parcBuffer[0][time] + dotSize / 2, parcBuffer[1][time] + dotSize / 2);
	m_vertices[4*(delayLength-1) + 3].position = sf::Vector2f(parcBuffer[0][time] - dotSize / 2, parcBuffer[1][time] + dotSize / 2);
}

void Trail::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// on applique la transformation
	//states.transform *= getTransform();

	// on applique la texture du tileset
	states.texture = &m_texture;

	// et on dessine enfin le tableau de vertex
	target.draw(m_vertices, states);
}


void Trail::openFileForLoading(uint8_t N) {
	paramFile.open("binSave/param_N" + std::to_string(N) + ".bin", std::ifstream::in | std::ifstream::binary);
	if (!paramFile.is_open()) {
		std::cout << "Failed opening file : binSave/param_N" + std::to_string(N) << std::endl;
	}
	paramFile.seekg(0, paramFile.beg);
	paramFile.read((char*)&trailLength, sizeof(uint32_t));

	paramFile.close();

	for (uint8_t i = 0; i < 3; i++) {
		parcFile[i].open(fileNames[i] + std::to_string(N) + ".bin", std::ifstream::in | std::ifstream::binary);
		if (!parcFile[i].is_open()) {
			std::cout << fileNames[i] + std::to_string(N) + ".bin" << std::endl;
		}
		parcFile[i].seekg(0, parcFile[i].beg);
		parcBuffer[i] = (double*)malloc(trailLength * sizeof(double));
		parcFile[i].read((char*)parcBuffer[i], trailLength * sizeof(double));

		parcFile[i].close();
	}

	// Rotating by 90° (my screen is horizontal but the trajectory is mostly vertical :/
	double temp;
	for (uint64_t i = 0; i < trailLength; i++) {
		temp = parcBuffer[0][i];
		parcBuffer[0][i] = parcBuffer[1][i];
		parcBuffer[1][i] = temp;
	}
}

void Trail::freeBuffers() {
	delete parcBuffer[0];
	delete parcBuffer[1];
	delete parcBuffer[2];
}