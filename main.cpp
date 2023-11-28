#include "Robot.hpp"
#include "Trail.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1900, 1000), "SFML works!"); // sf::Style::Fullscreen
	window.setFramerateLimit(1/0.1);

	Robot robot;
	Trail trail;
	uint8_t N = 3; // either 0, 1, 2, 3, 4
	robot.openFileForLoading(N);
	trail.openFileForLoading(N);

	// ---- view and interactions ----
	sf::View view = window.getDefaultView(); // https://www.sfml-dev.org/tutorials/2.0/graphics-view-fr.php
	// https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1View.php

	bool rightMousePressed = 0;
	sf::Vector2u initialMousePos;
	sf::Vector2f initialCenterPos;
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2f viewSize = view.getSize();

	view.setCenter(0, 0);
	view.zoom(1 / 20.f);
	window.setView(view);

	// ---- FPS tracking ----
	uint32_t nbFrames = 1;
	clock_t sum = 0;
	clock_t start = clock(), end;

	sf::Font font;
	if (!font.loadFromFile("OpenSans-Regular.ttf")) std::cout << "error loading arial.ttf" << std::endl;

	sf::Text FPS_displayer;
	FPS_displayer.setFont(font);
	FPS_displayer.setString("0");
	FPS_displayer.setCharacterSize(24);
	FPS_displayer.setFillColor(sf::Color::White);
	FPS_displayer.setPosition(0.f, 0.f);

	uint64_t t = 0;
    while (window.isOpen() && t != 0xFFFFFFFFFFFFFFFF)
    {
		sf::Event event; // https://www.sfml-dev.org/tutorials/2.6/window-events.php
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::KeyPressed:
				if (event.key.scancode == sf::Keyboard::Scan::Escape) {
					window.close();
				}
				break;
			case sf::Event::MouseWheelScrolled:
				view.zoom((float)(6 + event.mouseWheelScroll.delta) / 6);
				window.setView(view);
				break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Right) {
					rightMousePressed = 1;
					initialMousePos.x = event.mouseButton.x;
					initialMousePos.y = event.mouseButton.y;
					initialCenterPos = view.getCenter();
				}
				break;

			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Right) {
					rightMousePressed = 0;
				}
				break;

			case sf::Event::MouseMoved:
				if (rightMousePressed) {
					windowSize = window.getSize();
					viewSize = view.getSize();

					view.setCenter(initialCenterPos.x + ((float)initialMousePos.x - event.mouseMove.x) * viewSize.x / windowSize.x,
						initialCenterPos.y + ((float)initialMousePos.y - event.mouseMove.y) * viewSize.y / windowSize.y);

					window.setView(view);
				}
				break;
			case sf::Event::Closed:
				window.close();
				break;
			}
		}


		t = robot.upPosture(t);
		trail.upPosture(t);

		window.clear(sf::Color(0, 0, 10, 255));
		//window.setView(view);

		window.draw(trail);
		window.draw(robot);

		//window.draw(FPS_displayer);
		window.display();

		//while(clock() - start < (float)CLOCKS_PER_SEC / 700) {}
		end = clock();
		sum += end - start;
		start = clock();
		nbFrames++;
		if (sum > CLOCKS_PER_SEC) {
			//std::cout << "FPS : " << nbFrames << "\t" << pp.nbParticlesLeft << std::endl; // pour Particles
			//FTYPE speed = sqrt(pp.m_particles[0].speed.x * pp.m_particles[0].speed.x + pp.m_particles[0].speed.y * pp.m_particles[0].speed.y);
			//std::cout << "FPS : " << nbFrames << std::endl; // pour Particles2DArray
			char fpsstr[10]; // enough to hold all numbers up to 32-bits
			sprintf_s(fpsstr, "%I32i", nbFrames);
			//char timestr[21]; // enough to hold all numbers up to 64-bits
			//sprintf_s(timestr, "%I64i", pp.world.time);
			FPS_displayer.setString(fpsstr);
			sum = 0;
			nbFrames = 0;
		}
    }


	window.close();
	robot.freeBuffers();
	trail.freeBuffers();

    return 0;
}