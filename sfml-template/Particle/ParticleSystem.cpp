#include <SFML/Graphics.hpp>
#include "ParticleSystem.h"
using namespace sf;
using namespace std;
void ParticleSystem::init(int numParticles)
{
	m_Vertices.setPrimitiveType(Points);
	m_Vertices.resize(numParticles);
	// Create the particles
	for (int i = 0; i < numParticles; i++)
	{
		srand(time(0) + i);
		float angle = (rand() % 360) * 3.14f / 180.f;
		float speed = (rand() % 50) + 0.f;
		Vector2f direction;
		direction = Vector2f(cos(angle) * speed,
			sin(angle) * speed);
		m_Particles.push_back(Particle(direction));
	}
}

void ParticleSystem::update(float dt)
{
	m_Duration -= dt;
	vector<Particle>::iterator i;
	int currentVertex = 0;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		// Move the particle
		(*i).update(dt);
		// Update the vertex array
		m_Vertices[currentVertex++].position = i->getPosition();
	}
	if (m_Duration < 0)
	{
		m_IsRunning = false;
	}
}

void ParticleSystem::emitParticles(Vector2f startPosition, bool color)
{
	m_IsRunning = true;
	m_Duration = 0.5f;

	int currentVertex = 0;
	for (auto it = m_Particles.begin();
		it != m_Particles.end();
		it++)
	{
		if (!color)
			m_Vertices[currentVertex++].color = Color(37, 255, 254);
		else
			m_Vertices[currentVertex++].color = Color(248, 147, 30);

		it->setPosition(startPosition);
	}

	//Color(248, 147, 30) orange
	//Color(37, 255, 254) blue
}

void ParticleSystem::
draw(RenderTarget& target,
	RenderStates states) const
{
	target.draw(m_Vertices, states);
}

bool ParticleSystem::running()
{
	return m_IsRunning;
}
