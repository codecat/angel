#include <angel_modules/physics.h>

#include <modules/physics/box2d/Physics.h>

#include <scriptarray/scriptarray.h>
#include <glm/vec2.hpp>

#include <class_register.h>

#define instance() (love::Module::getInstance<love::physics::box2d::Physics>(love::Module::M_PHYSICS))

static b2Vec2 tobv(const glm::vec2 &v)
{
	return b2Vec2(v.x, v.y);
}

static glm::vec2 Body_getWorldPoint(love::physics::box2d::Body* self, float x, float y)
{
	glm::vec2 ret;
	self->getWorldPoint(x, y, ret.x, ret.y);
	return ret;
}

static love::physics::box2d::World* module_newWorld(float gx, float gy, bool sleep) { return instance()->newWorld(gx, gy, sleep); }
static love::physics::box2d::Body* module_newBody(love::physics::box2d::World* world, float x, float y, love::physics::box2d::Body::Type type)
{
	return instance()->newBody(world, x, y, type);
}
static love::physics::box2d::Body* module_newBody2(love::physics::box2d::World* world, love::physics::box2d::Body::Type type)
{
	return instance()->newBody(world, type);
}
static love::physics::box2d::Fixture* module_newFixture(love::physics::box2d::Body* body, love::physics::box2d::Shape* shape, float density)
{
	return instance()->newFixture(body, shape, density);
}

static love::physics::box2d::CircleShape* module_newCircleShape(float radius) { return instance()->newCircleShape(radius); }
static love::physics::box2d::CircleShape* module_newCircleShape2(float x, float y, float radius) { return instance()->newCircleShape(x, y, radius); }
static love::physics::box2d::PolygonShape* module_newRectangleShape(float w, float h) { instance()->newRectangleShape(w, h); }
static love::physics::box2d::PolygonShape* module_newRectangleShape2(float x, float y, float w, float h) { instance()->newRectangleShape(x, y, w, h); }
static love::physics::box2d::PolygonShape* module_newRectangleShape3(float x, float y, float w, float h, float angle) { instance()->newRectangleShape(x, y, w, h, angle); }
static love::physics::box2d::PolygonShape* module_newPolygonShape(CScriptArray* vertices)
{
	asUINT pointCount = vertices->GetSize();

	if (pointCount < 3) {
		asGetActiveContext()->SetException("Expected a minimum of 3 vertices.");
		return nullptr;
	}

	if (pointCount > b2_maxPolygonVertices) {
		asGetActiveContext()->SetException("Exceeded the maximum number of vertices.");
		return nullptr;
	}

	auto s = new b2PolygonShape();
	b2Vec2 points[b2_maxPolygonVertices];

	for (asUINT i = 0; i < pointCount; i++) {
		auto &v = *(glm::vec2*)vertices->At(i);
		points[i].x = love::physics::box2d::Physics::scaleDown(v.x);
		points[i].y = love::physics::box2d::Physics::scaleDown(v.y);
	}

	s->Set(points, pointCount);

	return new love::physics::box2d::PolygonShape(s);
}
static love::physics::box2d::EdgeShape* module_newEdgeShape(float x1, float y1, float x2, float y2) { return instance()->newEdgeShape(x1, y1, x2, y2); }
static love::physics::box2d::ChainShape* module_newChainShape(bool looping, CScriptArray* vertices)
{
	auto s = new b2ChainShape();

	asUINT pointCount = vertices->GetSize();
	b2Vec2* points = new b2Vec2[pointCount];

	for (asUINT i = 0; i < pointCount; i++) {
		auto &v = *(glm::vec2*)vertices->At(i);
		points[i].x = v.x;
		points[i].y = v.y;
	}

	if (looping) {
		s->CreateLoop(points, pointCount);
	} else {
		s->CreateChain(points, pointCount);
	}

	delete[] points;

	return new love::physics::box2d::ChainShape(s);
}

static love::physics::box2d::DistanceJoint* module_newDistanceJoint(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float x1, float y1, float x2, float y2, bool collideConnected)
{
	return instance()->newDistanceJoint(body1, body2, x1, y1, x2, y2, collideConnected);
}
static love::physics::box2d::MouseJoint* module_newMouseJoint(love::physics::box2d::Body* body, float x, float y)
{
	return instance()->newMouseJoint(body, x, y);
}
static love::physics::box2d::RevoluteJoint* module_newRevoluteJoint(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float xA, float yA, bool collideConnected)
{
	return instance()->newRevoluteJoint(body1, body2, xA, yA, xA, yA, collideConnected);
}
static love::physics::box2d::RevoluteJoint* module_newRevoluteJoint2(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float xA, float yA, float xB, float yB, bool collideConnected)
{
	return instance()->newRevoluteJoint(body1, body2, xA, yA, xB, yB, collideConnected);
}
static love::physics::box2d::RevoluteJoint* module_newRevoluteJoint3(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float xA, float yA, float xB, float yB, bool collideConnected, float referenceAngle)
{
	return instance()->newRevoluteJoint(body1, body2, xA, yA, xB, yB, collideConnected, referenceAngle);
}
static love::physics::box2d::PrismaticJoint* module_newPrismaticJoint(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float xA, float yA, float xB, float yB, float ax, float ay, bool collideConnected)
{
	return instance()->newPrismaticJoint(body1, body2, xA, yA, xB, yB, ax, ay, collideConnected);
}
static love::physics::box2d::PrismaticJoint* module_newPrismaticJoint2(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float xA, float yA, float xB, float yB, float ax, float ay, bool collideConnected, float referenceAngle)
{
	return instance()->newPrismaticJoint(body1, body2, xA, yA, xB, yB, ax, ay, collideConnected, referenceAngle);
}
static love::physics::box2d::PulleyJoint* module_newPulleyJoint(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, const glm::vec2 &groundAnchor1, const glm::vec2 &groundAnchor2, const glm::vec2 &anchor1, const glm::vec2 &anchor2, float ratio, bool collideConnected)
{
	return instance()->newPulleyJoint(body1, body2, tobv(groundAnchor1), tobv(groundAnchor2), tobv(anchor1), tobv(anchor2), ratio, collideConnected);
}
static love::physics::box2d::GearJoint* module_newGearJoint(love::physics::box2d::Joint* joint1, love::physics::box2d::Joint* joint2, float ratio, bool collideConnected)
{
	return instance()->newGearJoint(joint1, joint2, ratio, collideConnected);
}
static love::physics::box2d::FrictionJoint* module_newFrictionJoint(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float xA, float yA, float xB, float yB, bool collideConnected)
{
	return instance()->newFrictionJoint(body1, body2, xA, yA, xB, yB, collideConnected);
}
static love::physics::box2d::WeldJoint* module_newWeldJoint(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float xA, float yA, float xB, float yB, bool collideConnected)
{
	return instance()->newWeldJoint(body1, body2, xA, yA, xB, yB, collideConnected);
}
static love::physics::box2d::WeldJoint* module_newWeldJoint2(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float xA, float yA, float xB, float yB, bool collideConnected, float referenceAngle)
{
	return instance()->newWeldJoint(body1, body2, xA, yA, xB, yB, collideConnected, referenceAngle);
}
static love::physics::box2d::WheelJoint* module_newWheelJoint(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float xA, float yA, float xB, float yB, float ax, float ay, bool collideConnected)
{
	return instance()->newWheelJoint(body1, body2, xA, yA, xB, yB, ax, ay, collideConnected);
}
static love::physics::box2d::RopeJoint* module_newRopeJoint(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float x1, float y1, float x2, float y2, float maxLength, bool collideConnected)
{
	return instance()->newRopeJoint(body1, body2, x1, y1, x2, y2, maxLength, collideConnected);
}
static love::physics::box2d::MotorJoint* module_newMotorJoint(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2)
{
	return instance()->newMotorJoint(body1, body2);
}
static love::physics::box2d::MotorJoint* module_newMotorJoint2(love::physics::box2d::Body* body1, love::physics::box2d::Body* body2, float correctionFactor, bool collideConnected)
{
	return instance()->newMotorJoint(body1, body2, correctionFactor, collideConnected);
}

//HACK: We need to get the protected b2Fixture*
class WrappedFixture : public love::physics::box2d::Fixture
{
public:
	b2Fixture* getInternalFixture() { return fixture; }
};

static float module_getDistance(WrappedFixture* a, WrappedFixture* b)
{
	b2DistanceProxy pA, pB;
	b2DistanceInput i;
	b2DistanceOutput o;
	b2SimplexCache c;
	c.count = 0;

	pA.Set(a->getInternalFixture()->GetShape(), 0);
	pB.Set(b->getInternalFixture()->GetShape(), 0);
	i.proxyA = pA;
	i.proxyB = pB;
	i.transformA = a->getInternalFixture()->GetBody()->GetTransform();
	i.transformB = b->getInternalFixture()->GetBody()->GetTransform();
	i.useRadii = true;
	b2Distance(&o, &c, &i);

	return love::physics::box2d::Physics::scaleUp(o.distance);
}

static float module_getMeter() { return instance()->getMeter(); }
static void module_setMeter(float scale) { instance()->setMeter(scale); }

void RegisterPhysics(asIScriptEngine* engine)
{
	auto pPhysics = instance();
	if (pPhysics == nullptr) {
		pPhysics = new love::physics::box2d::Physics();
		love::Module::registerInstance(pPhysics);
	}

	engine->SetDefaultNamespace("angel::physics");

	// BodyType
	engine->RegisterEnum("BodyType");
	engine->RegisterEnumValue("BodyType", "Invalid", love::physics::box2d::Body::BODY_INVALID);
	engine->RegisterEnumValue("BodyType", "Static", love::physics::box2d::Body::BODY_STATIC);
	engine->RegisterEnumValue("BodyType", "Dynamic", love::physics::box2d::Body::BODY_DYNAMIC);
	engine->RegisterEnumValue("BodyType", "Kinematic", love::physics::box2d::Body::BODY_KINEMATIC);

	// World
	auto regWorld = ClassRegister::New(engine, "World", 0, asOBJ_REF, "Object");
	regWorld->Method("void update(float dt)", asMETHODPR(love::physics::box2d::World, update, (float), void), asCALL_THISCALL);
	regWorld->Method("void update(float dt, int velocityIterations, int positionIterations)", asMETHODPR(love::physics::box2d::World, update, (float, int, int), void), asCALL_THISCALL);

	// Body (actually inherited from love::physics::Body)
	auto regBody = ClassRegister::New(engine, "Body", 0, asOBJ_REF, "Object");
	regBody->Method("float getX()", asMETHOD(love::physics::box2d::Body, getX), asCALL_THISCALL);
	regBody->Method("float getY()", asMETHOD(love::physics::box2d::Body, getY), asCALL_THISCALL);
	regBody->Method("float getAngle()", asMETHOD(love::physics::box2d::Body, getAngle), asCALL_THISCALL);

	regBody->Method("void setLinearDamping(float damping)", asMETHOD(love::physics::box2d::Body, setLinearDamping), asCALL_THISCALL);
	regBody->Method("void setAngularDamping(float damping)", asMETHOD(love::physics::box2d::Body, setAngularDamping), asCALL_THISCALL);

	regBody->Method("void applyForce(float x, float y, bool wake = true)", asMETHODPR(love::physics::box2d::Body, applyForce, (float, float, bool), void), asCALL_THISCALL);
	regBody->Method("void applyForce(float x, float y, float rx, float ry, bool wake = true)", asMETHODPR(love::physics::box2d::Body, applyForce, (float, float, float, float, bool), void), asCALL_THISCALL);

	regBody->Method("vec2 getWorldPoint(float x, float y)", asFUNCTION(Body_getWorldPoint), asCALL_CDECL_OBJFIRST);

	// Fixture
	auto regFixture = ClassRegister::New(engine, "Fixture", 0, asOBJ_REF, "Object");
	regFixture->Method("void setRestitution(float restitution)", asMETHOD(love::physics::box2d::Fixture, setRestitution), asCALL_THISCALL);

	// Shape (actually inherited from love::physics::Shape)
	auto regShape = ClassRegister::New(engine, "Shape", 0, asOBJ_REF, "Object");
	// CircleShape
	auto regCircleShape = ClassRegister::New(engine, "CircleShape", 0, asOBJ_REF, "Shape");
	// PolygonShape
	auto regPolygonShape = ClassRegister::New(engine, "PolygonShape", 0, asOBJ_REF, "Shape");
	// EdgeShape
	auto regEdgeShape = ClassRegister::New(engine, "EdgeShape", 0, asOBJ_REF, "Shape");
	// ChainShape
	auto regChainShape = ClassRegister::New(engine, "ChainShape", 0, asOBJ_REF, "Shape");

	// Joint (actually inherited from love::physics::Joint)
	auto regJoint = ClassRegister::New(engine, "Joint", 0, asOBJ_REF, "Object");
	// DistanceJoint
	auto regDistanceJoint = ClassRegister::New(engine, "DistanceJoint", 0, asOBJ_REF, "Joint");
	// MouseJoint
	auto regMouseJoint = ClassRegister::New(engine, "MouseJoint", 0, asOBJ_REF, "Joint");
	// RevoluteJoint
	auto regRevoluteJoint = ClassRegister::New(engine, "RevoluteJoint", 0, asOBJ_REF, "Joint");
	// PrismaticJoint
	auto regPrismaticJoint = ClassRegister::New(engine, "PrismaticJoint", 0, asOBJ_REF, "Joint");
	// PulleyJoint
	auto regPulleyJoint = ClassRegister::New(engine, "PulleyJoint", 0, asOBJ_REF, "Joint");
	// GearJoint
	auto regGearJoint = ClassRegister::New(engine, "GearJoint", 0, asOBJ_REF, "Joint");
	// FrictionJoint
	auto regFrictionJoint = ClassRegister::New(engine, "FrictionJoint", 0, asOBJ_REF, "Joint");
	// WeldJoint
	auto regWeldJoint = ClassRegister::New(engine, "WeldJoint", 0, asOBJ_REF, "Joint");
	// WheelJoint
	auto regWheelJoint = ClassRegister::New(engine, "WheelJoint", 0, asOBJ_REF, "Joint");
	// RopeJoint
	auto regRopeJoint = ClassRegister::New(engine, "RopeJoint", 0, asOBJ_REF, "Joint");
	// MotorJoint
	auto regMotorJoint = ClassRegister::New(engine, "MotorJoint", 0, asOBJ_REF, "Joint");

	// Module
	engine->RegisterGlobalFunction("World@+ newWorld(float gx, float gy, bool sleep = true)", asFUNCTION(module_newWorld), asCALL_CDECL);
	engine->RegisterGlobalFunction("Body@+ newBody(World@+ world, float x, float y, BodyType type)", asFUNCTION(module_newBody), asCALL_CDECL);
	engine->RegisterGlobalFunction("Body@+ newBody(World@+ world, BodyType type)", asFUNCTION(module_newBody2), asCALL_CDECL);
	engine->RegisterGlobalFunction("Fixture@+ newFixture(Body@+ body, Shape@+ shape, float density = 1.0f)", asFUNCTION(module_newFixture), asCALL_CDECL);

	engine->RegisterGlobalFunction("CircleShape@+ newCircleShape(float radius)", asFUNCTION(module_newCircleShape), asCALL_CDECL);
	engine->RegisterGlobalFunction("CircleShape@+ newCircleShape(float x, float y, float radius)", asFUNCTION(module_newCircleShape2), asCALL_CDECL);
	engine->RegisterGlobalFunction("PolygonShape@+ newRectangleShape(float w, float h)", asFUNCTION(module_newRectangleShape), asCALL_CDECL);
	engine->RegisterGlobalFunction("PolygonShape@+ newRectangleShape(float x, float y, float w, float h)", asFUNCTION(module_newRectangleShape2), asCALL_CDECL);
	engine->RegisterGlobalFunction("PolygonShape@+ newRectangleShape(float x, float y, float w, float h, float angle)", asFUNCTION(module_newRectangleShape3), asCALL_CDECL);
	engine->RegisterGlobalFunction("PolygonShape@+ newPolygonShape(const array<vec2>@+ vertices)", asFUNCTION(module_newPolygonShape), asCALL_CDECL);
	engine->RegisterGlobalFunction("EdgeShape@+ newEdgeShape(float x1, float y1, float x2, float y2)", asFUNCTION(module_newEdgeShape), asCALL_CDECL);
	engine->RegisterGlobalFunction("ChainShape@+ newChainShape(bool looping, array<vec2>@+ vertices)", asFUNCTION(module_newChainShape), asCALL_CDECL);

	engine->RegisterGlobalFunction("DistanceJoint@+ newDistanceJoint(Body@+ body1, Body@+ body2, float x1, float y1, float x2, float y2, bool collideConnected = false)", asFUNCTION(module_newDistanceJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("MouseJoint@+ newMouseJoint(Body@+ body, float x, float y)", asFUNCTION(module_newMouseJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("RevoluteJoint@+ newRevoluteJoint(Body@+ body1, Body@+ body2, float xA, float yA, bool collideConnected = false)", asFUNCTION(module_newRevoluteJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("RevoluteJoint@+ newRevoluteJoint(Body@+ body1, Body@+ body2, float xA, float yA, float xB, float yB, bool collideConnected = false)", asFUNCTION(module_newRevoluteJoint2), asCALL_CDECL);
	engine->RegisterGlobalFunction("RevoluteJoint@+ newRevoluteJoint(Body@+ body1, Body@+ body2, float xA, float yA, float xB, float yB, bool collideConnected = false, float referenceAngle = 0.0f)", asFUNCTION(module_newRevoluteJoint3), asCALL_CDECL);
	engine->RegisterGlobalFunction("PrismaticJoint@+ newPrismaticJoint(Body@+ body1, Body@+ body2, float xA, float yA, float xB, float yB, float ax, float ay, bool collideConnected = false)", asFUNCTION(module_newPrismaticJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("PrismaticJoint@+ newPrismaticJoint(Body@+ body1, Body@+ body2, float xA, float yA, float xB, float yB, float ax, float ay, bool collideConnected = false, float referenceAngle = 0.0f)", asFUNCTION(module_newPrismaticJoint2), asCALL_CDECL);
	engine->RegisterGlobalFunction("PulleyJoint@+ newPulleyJoint(Body@+ body1, Body@+ body2, vec2 groundAnchor1, vec2 groundAnchor2, vec2 anchor1, vec2 anchor2, float ratio, bool collideConnected = false)", asFUNCTION(module_newPulleyJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("GearJoint@+ newGearJoint(Joint@ joint1, Joint@ joint2, float ratio, bool collideConnected = false)", asFUNCTION(module_newGearJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("FrictionJoint@+ newFrictionJoint(Body@+ body1, Body@+ body2, float xA, float yA, float xB, float yB, bool collideConnected = false)", asFUNCTION(module_newFrictionJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("WeldJoint@+ newWeldJoint(Body@+ body1, Body@+ body2, float xA, float yA, float xB, float yB, bool collideConnected = false)", asFUNCTION(module_newWeldJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("WeldJoint@+ newWeldJoint(Body@+ body1, Body@+ body2, float xA, float yA, float xB, float yB, bool collideConnected = false, float referenceAngle = 0.0f)", asFUNCTION(module_newWeldJoint2), asCALL_CDECL);
	engine->RegisterGlobalFunction("WheelJoint@+ newWheelJoint(Body@+ body1, Body@+ body2, float xA, float yA, float xB, float yB, float ax, float ay, bool collideConnected = false)", asFUNCTION(module_newWheelJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("RopeJoint@+ newRopeJoint(Body@+ body1, Body@+ body2, float x1, float y1, float x2, float y2, float maxLength, bool collideConnected = false)", asFUNCTION(module_newRopeJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("MotorJoint@+ newMotorJoint(Body@+ body1, Body@+ body2)", asFUNCTION(module_newMotorJoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("MotorJoint@+ newMotorJoint(Body@+ body1, Body@+ body2, float correctionFactor, bool collideConnected = false)", asFUNCTION(module_newMotorJoint2), asCALL_CDECL);

	engine->RegisterGlobalFunction("float getDistance(Fixture@ a, Fixture@ b)", asFUNCTION(module_getDistance), asCALL_CDECL);

	engine->RegisterGlobalFunction("float getMeter()", asFUNCTION(module_getMeter), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setMeter(float scale)", asFUNCTION(module_setMeter), asCALL_CDECL);

	engine->SetDefaultNamespace("");
}
