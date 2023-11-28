#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include<fstream>

#define ROBOT_TEXTURE_PATH "robot_tile.png"

class Robot : public sf::Drawable, public sf::Transformable
{
public :
	Robot();

	uint64_t upPosture(uint64_t time); // Also updates vertices accordingly

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	double posture[3];

	sf::VertexArray m_vertices;
	sf::Texture m_texture;

	// position loading
	void openFileForLoading(uint8_t N);
	void freeBuffers();
	std::ifstream paramFile;
	std::ifstream parcFile[3];
	double* parcBuffer[3] = { nullptr, nullptr , nullptr };
	uint32_t trailLength = 0;
	std::string fileNames[3] = { "binSave/parc_x_N", "binSave/parc_y_N", "binSave/parc_theta_N" };
};