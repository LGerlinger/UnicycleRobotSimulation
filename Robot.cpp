#include "Robot.hpp"

#define NB_CORPS 4
#define NB_CIRCLES 2

Robot::Robot() {
	// loading particle texture
	if (!m_texture.loadFromFile(ROBOT_TEXTURE_PATH)) {
		std::cout << "Robot::Robot() : failed loading texture from file " << ROBOT_TEXTURE_PATH << "\n\tConstruction aborted (no extra memory allocated)" << std::endl;
		return;
	}

	posture[0] = 0;
	posture[1] = 0;
	posture[2] = 0;

	// dimensioning the vertexArray
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(NB_CORPS * 4);

	// adding the textures to circles
	sf::Vector2u textureSize = m_texture.getSize();

	float sizeX = (float)(textureSize.x/2);
	float sizeY = (float)textureSize.y;

	for (uint32_t p = 0; p < NB_CIRCLES; p++) { // Loading texture of a circle
		m_vertices[4 * p].texCoords = sf::Vector2f(0.f, 0.f);
		m_vertices[4 * p + 1].texCoords = sf::Vector2f(sizeX, 0.f);
		m_vertices[4 * p + 2].texCoords = sf::Vector2f(sizeX, sizeY);
		m_vertices[4 * p + 3].texCoords = sf::Vector2f(0.f, sizeY);
	}
	for (uint32_t p = NB_CIRCLES; p < NB_CORPS; p++) { // Loading texture of a rectangle
		m_vertices[4 * p].texCoords = sf::Vector2f(sizeX, 0.f);
		m_vertices[4 * p + 1].texCoords = sf::Vector2f(2*sizeX, 0.f);
		m_vertices[4 * p + 2].texCoords = sf::Vector2f(2*sizeX, sizeY);
		m_vertices[4 * p + 3].texCoords = sf::Vector2f(sizeX, sizeY);
	}

	// Corps principal
	m_vertices[0].position = sf::Vector2f( 0.2f, 0.2f);
	m_vertices[1].position = sf::Vector2f( 0.2f, -0.2f);
	m_vertices[2].position = sf::Vector2f(-0.2f, -0.2f);
	m_vertices[3].position = sf::Vector2f(-0.2f, 0.2f);
	for (uint8_t i = 0; i < 4; i++) {
		m_vertices[i].color.a = 127;
		m_vertices[i].color.r = 127;
		m_vertices[i].color.g = 127;
		m_vertices[i].color.b = 127;
	}


	// pointeur
	m_vertices[4].position = sf::Vector2f(0.1f + 0.01f, 0.1f + 0.01f);
	m_vertices[5].position = sf::Vector2f(0.1f + 0.01f, 0.1f - 0.01f);
	m_vertices[6].position = sf::Vector2f(0.1f - 0.01f, 0.1f - 0.01f);
	m_vertices[7].position = sf::Vector2f(0.1f - 0.01f, 0.1f + 0.01f);
	for (uint8_t i = 0; i < 4; i++) {
		m_vertices[4 + i].color.a = 127;
		m_vertices[4 + i].color.r = 0;
		m_vertices[4 + i].color.g = 0;
		m_vertices[4 + i].color.b = 0;
	}

	// roue gauche
	m_vertices[8].position =  sf::Vector2f( 0.05f, 0.15f + 0.02f);
	m_vertices[9].position =  sf::Vector2f( 0.05f, 0.15f - 0.02f);
	m_vertices[10].position = sf::Vector2f(-0.05f, 0.15f - 0.02f);
	m_vertices[11].position = sf::Vector2f(-0.05f, 0.15f + 0.02f);
	for (uint8_t i = 0; i < 4; i++) {
		m_vertices[8 + i].color.a = 127;
		m_vertices[8 + i].color.r = 30;
		m_vertices[8 + i].color.g = 0;
		m_vertices[8 + i].color.b = 255;
	}

	// roue droite
	m_vertices[12].position = sf::Vector2f( 0.05f, -0.15f + 0.02f);
	m_vertices[13].position = sf::Vector2f( 0.05f, -0.15f - 0.02f);
	m_vertices[14].position = sf::Vector2f(-0.05f, -0.15f - 0.02f);
	m_vertices[15].position = sf::Vector2f(-0.05f, -0.15f + 0.02f);
	for (uint8_t i = 0; i < 4; i++) {
		m_vertices[12 + i].color.a = 127;
		m_vertices[12 + i].color.r = 30;
		m_vertices[12 + i].color.g = 0;
		m_vertices[12 + i].color.b = 255;
	}

	// moving every vertices to center p point (0.01, 0.01) of the robot
	for (uint8_t i = 0; i < 4 * NB_CORPS; i++) {
		m_vertices[i].position.x -= 0.1f;
		m_vertices[i].position.y -= 0.1f;
	}

	float mult = 1;
	for (uint8_t i = 0; i < 4 * NB_CORPS; i++) {
		m_vertices[i].position.x = m_vertices[i].position.x * mult;
		m_vertices[i].position.y = m_vertices[i].position.y * mult;
	}
}

uint64_t Robot::upPosture(uint64_t time) {
	if (time >= trailLength) { return 0xFFFFFFFFFFFFFFFF; }

	double _posture[3];
	_posture[0] = parcBuffer[0][time];
	_posture[1] = parcBuffer[1][time];
	_posture[2] = parcBuffer[2][time];

	double dx = _posture[0] - posture[0];
	double dy = _posture[1] - posture[1];
	double dtheta = _posture[2] - posture[2];

	double vx;
	double vy;
	for (uint32_t p = 0; p < 4*NB_CORPS; p++) {
		vx = m_vertices[p].position.x - posture[0];
		vy = m_vertices[p].position.y - posture[1];

		float newPos[2] = {
			(float)(_posture[0] + vx * cos(dtheta) - vy * sin(dtheta)),
			(float)(_posture[1] + vx * sin(dtheta) + vy * cos(dtheta))
		};

		/*if (!p) {
			std::cout << "(" << _posture[0] << ", " << _posture[1] << ", " << _posture[2] << ")" << std::endl;
		}*/

		m_vertices[p].position.x = newPos[0];
		m_vertices[p].position.y = newPos[1];
	}

	posture[0] = _posture[0];
	posture[1] = _posture[1];
	posture[2] = _posture[2];
	return time+1;
}

void Robot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// on applique la transformation
	//states.transform *= getTransform();

	// on applique la texture du tileset
	states.texture = &m_texture;

	// et on dessine enfin le tableau de vertex
	target.draw(m_vertices, states);
}


void Robot::openFileForLoading(uint8_t N) {
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

		parcBuffer[2][i] = - parcBuffer[2][i] + 1.57079632; // pi/2
	}
}

void Robot::freeBuffers() {
	delete parcBuffer[0];
	delete parcBuffer[1];
	delete parcBuffer[2];
}