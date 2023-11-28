#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include<fstream>

#define TRAIL_TEXTURE_PATH "robot_tile.png" // We will only use the robot's circle texture

class Trail : public sf::Drawable, public sf::Transformable
{
public:
	Trail();

	void upPosture(uint64_t time); // Also updates vertices accordingly

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	uint8_t delayLength = 30;

	sf::VertexArray m_vertices;
	float dotSize = 0.1;
	sf::Texture m_texture;

	// position loading
	void openFileForLoading(uint8_t N);
	void freeBuffers();
	std::ifstream paramFile;
	std::ifstream parcFile[3]; // The third file is unused for now
	double* parcBuffer[3] = {nullptr, nullptr , nullptr };
	uint32_t trailLength = 0;
	std::string fileNames[3] = { "binSave/traj_x_N", "binSave/traj_y_N", "binSave/traj_theta_N" };
};