#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "Box2D\Box2D\Box2D.h"

#ifdef _DEBUG
#pragma comment( lib,"Box2D/libx86/Debug/Box2D.lib")
#else 
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib")
#endif
// TODO 1: Include Box 2 header and library
#define METERS_TO_PIXELS(meters) (meters/0.002)
#define PIXELS_TO_METERS(pixels) (pixels * 0.002)

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it

	b2Vec2 gravity(0.0f, -10.0f);
	world = new b2World(gravity);

	// TODO 4: Create a a big static circle as "ground"
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 10.0f);

	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(PIXELS_TO_METERS(100), PIXELS_TO_METERS(100));

	b2Body* Body = world->CreateBody(&groundBodyDef);
	b2CircleShape shape;
	shape.m_radius = PIXELS_TO_METERS(100);
	b2FixtureDef Fixture;
	Fixture.shape = &shape;
	Body->CreateFixture(&Fixture);
	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world->Step(timeStep, velocityIterations, positionIterations);
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		debug = !debug;
		b2BodyDef newbody;
		newbody.type = b2_dynamicBody;
		newbody.position.Set(PIXELS_TO_METERS(App->input->GetMouseX()), PIXELS_TO_METERS(App->input->GetMouseY()));
		b2Body* Body = world->CreateBody(&newbody);
		b2CircleShape shape;
		shape.m_radius = PIXELS_TO_METERS(100);
		b2FixtureDef Fixture;
		Fixture.shape = &shape;
		Body->CreateFixture(&Fixture);
	}
	if(!debug)
		return UPDATE_CONTINUE;

	/* Bonus code: this will iterate all objects in the world and draw the circles
	 You need to provide your own macro to translate meters to pixels*/
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}


