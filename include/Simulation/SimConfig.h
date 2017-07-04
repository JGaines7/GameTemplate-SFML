#ifndef SIMCONFIG_H
#define SIMCONFIG_H

#include "SFML/graphics.hpp"

struct SimulationSettings
{
    SimulationSettings() : worldDimensions(3000,3000) {}
    sf::Color zombieColor = sf::Color::Red;
    sf::Color humanColor = sf::Color::Green;

    sf::Vector2i worldDimensions;
    int numHumans = 1500;
    int numZombies = 1;

    float zombieMaxSpeed = 0.8;
    float zombieWanderingFactor = .001;
    float zombieTrackingFactor = 4;
    float zombieTrackingFalloff = 0.8;
    float zombieRadius = 2;
    float humanMaxSpeed = 0.75;
    float humanWanderingFactor = .001;
    float humanTrackingFactor = 3;
    float humanTrackingFalloff = 1;
    float humanAccelaration = 1.0005;
    float zombieAccelaration = 1.5;
    float humanRadius = 2;

};

#endif
