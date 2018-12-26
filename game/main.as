namespace G
{
	angel::graphics::Image@ img_duckloon_normal;
	angel::graphics::Image@ img_duckloon_blink;

	angel::graphics::Image@ img_n;
	angel::graphics::Image@ img_o;
	angel::graphics::Image@ img_g;
	angel::graphics::Image@ img_a;
	angel::graphics::Image@ img_m;
	angel::graphics::Image@ img_e;
	angel::graphics::Image@ img_square;

	angel::graphics::Image@ img_cloud_1;
	angel::graphics::Image@ img_cloud_2;
	angel::graphics::Image@ img_cloud_3;
	angel::graphics::Image@ img_cloud_4;

	array<angel::graphics::Image@> cloud_images;
}

class StatePos
{
	float x;
	float y;
	float r;
}

class State
{
	float t0, t1;
	float x0, x1;
	float y0, y1;
	float r0, r1;

	State(angel::physics::Body@ body)
	{
		t0 = 0;
		x0 = body.getX();
		y0 = body.getY();
		r0 = body.getAngle();

		t1 = t0;
		x1 = x0;
		y1 = y0;
		r1 = r0;
	}

	void save(angel::physics::Body@ body, float t)
	{
		t0 = t1;
		x0 = x1;
		y0 = y1;
		r0 = r1;

		t1 = t;
		x1 = body.getX();
		y1 = body.getY();
		r1 = body.getAngle();
	}

	StatePos get(float t)
	{
		t = min(t, t1);
		t = max(t, t0);

		float p = (t - t0) / (t1 - t0);

		StatePos ret;
		ret.x = x0 + p * (x1 - x0);
		ret.y = y0 + p * (y1 - y0);
		ret.r = r0 + p * (r1 - r0);
		return ret;
	}
}

class Blink
{
	float closed_t = 0;
	float next_blink_t = 5;

	void update(float dt)
	{
		next_blink_t = max(0.0f, next_blink_t - dt);
		closed_t = max(0.0f, closed_t - dt);

		if (next_blink_t == 0) {
			next_blink_t = 5.0f + randf() * 3.0f;
			closed_t = 0.1f;
		}
	}

	bool is_closed()
	{
		return closed_t > 0;
	}
}

class Entity
{
	void step() { }
	void update(float dt) { }
	void draw() { }
}

class Duckloon : Entity
{
	angel::physics::Body@ body;
	angel::physics::Shape@ shape;
	angel::physics::Fixture@ fixture;
	angel::physics::Joint@ pin;

	angel::graphics::Image@ img_normal;
	angel::graphics::Image@ img_blink;
	angel::graphics::Image@ img;

	State@ state;
	Blink@ blink;

	Duckloon(angel::physics::World@ world, float x, float y)
	{
		@body = angel::physics::newBody(world, x, y, angel::physics::BodyType::Dynamic);
		body.setLinearDamping(0.8f);
		body.setAngularDamping(0.8f);
		@shape = angel::physics::newPolygonShape({
			vec2(-55, -60),
			vec2(0, 90),
			vec2(55, -60)
		});
		@fixture = angel::physics::newFixture(body, shape, 1);
		fixture.setRestitution(0.5f);
		@pin = angel::physics::newMouseJoint(body, x, y - 80);

		@img_normal = G::img_duckloon_normal;
		@img_blink = G::img_duckloon_blink;
		@img = img_normal;

		@blink = Blink();
		@state = State(body);
	}

	void step() override
	{
		state.save(body, g_step);

		if (int(g_step % 5.0f) == 0) {
			body.applyForce(randi(30, 50), 0);
		}
	}

	void update(float dt) override
	{
		blink.update(dt);
	}

	void draw() override
	{
		auto drawState = state.get(g_t);

		angel::graphics::setColor(angel::Colorf(1, 1, 1));

		auto img = img_normal;
		if (blink.is_closed()) {
			@img = img_blink;
		}

		angel::graphics::draw(img, vec2(drawState.x, drawState.y), drawState.r, vec2(1, 1), img.getDimensions() / 2.0f);
	}

	vec2 attachment_point()
	{
		return body.getWorldPoint(4, 90);
	}
}

class ChainLink
{
	float x;
	float y;

	float radius;
	angel::graphics::Image@ img;

	angel::physics::Body@ body;
	angel::physics::Shape@ shape;
	angel::physics::Fixture@ fixture;
	angel::physics::State@ state;

	angel::physics::Joint@ joint, join2;
}

class Chain : Entity
{
	array<ChainLink@> links;
	Duckloon@ duckloon;

	vec2 origin;

	Chain(angel::physics::World@ world, float x, float y, Duckloon@ _duckloon)
	{
		origin = vec2(x, y);
		@duckloon = _duckloon;

		addLetter(world, 4); addLetter(world, 4);
		addLetter(world, 11, G::img_n); addLetter(world, 4);
		addLetter(world, 11, G::img_o); addLetter(world, 4);
		addLetter(world, 11, G::img_square); addLetter(world, 4);
		addLetter(world, 11, G::img_g); addLetter(world, 4);
		addLetter(world, 11, G::img_a); addLetter(world, 4);
		addLetter(world, 11, G::img_m); addLetter(world, 4);
		addLetter(world, 11, G::img_e); addLetter(world, 4);
		addLetter(world, 11, G::img_square); addLetter(world, 4);
	}

	void addLetter(angel::physics::World@ world, float radius, angel::graphics::Image@ img = null)
	{
		ChainLink@ prev = null;
		if (links.length() > 0) {
			@prev = links[links.length() - 1];
		}

		auto link = ChainLink();

		link.x = origin.x;
		link.y = origin.y;

		link.radius = radius;
		@link.img = img;

		if (prev !is null) {
			link.y = prev.y + prev.radius + link.radius;
		}

		@link.body = angel::physics::newBody(world, link.x, link.y, angel::physics::BodyType::Dynamic);
		link.body.setLinearDamping(0.5f);
		link.body.setAngularDamping(0.5f);
		@link.shape = angel::physics::newCircleShape(link.radius);
		@link.fixture = angel::physics::newFixture(link.body, link.shape, 1.0f / (links.length() + 1));
		@link.state = State(link.body);

		if (prev !is null) {
			@link.joint = angel::physics::newRevoluteJoint(link.body, prev.body, link.x, link.y - link.radius / 2);
			@link.join2 = angel::physics::newRopeJoint(link.body, duckloon.body, link.x, link.y, origin.x, origin.y, link.y - origin.y);
		} else {
			@link.joint = angel::physics::newRevoluteJoint(link.body, duckloon.body, link.x, link.y);
		}

		links.insertLast(link);
	}

	void step() override
	{
		for (uint i = 0; i < links.length(); i++) {
			auto link = links[i];
			link.state.save(link.body, g_step);
		}
	}

	void draw() override
	{
		array<vec2> rope;

		for (uint i = 0; i < links.length(); i++) {
			auto linkState = links[i].state.get(g_t);
			rope.insertLast(vec2(linkState.x, linkState.y));
		}

		angel::graphics::setLineWidth(3);
		angel::graphics::setColor(angel::Colorf(1, 1, 1, 0.7f));
		angel::graphics::line(rope);

		for (uint i = 0; i < links.length(); i++) {
			auto link = links[i];
			if (link.img is null) {
				continue;
			}

			auto linkState = link.state.get(g_t);
			vec2 origin = link.img.getDimensions() / 2.0f;
			angel::graphics::setColor(angel::Colorf(1, 1, 1));
			angel::graphics::draw(link.img, vec2(linkState.x, linkState.y), linkState.r, vec2(1, 1), origin);
		}
	}
}

class CloudTrack
{
	float x;
	float y;

	float initial_offset;
	float h_spacing;

	angel::graphics::Image@ img;
	int initial_img;

	float w;
	float speed;
	int count;

	CloudTrack(float _x, float _y, float offset, float _speed, angel::graphics::Image@ _img)
	{
		x = _x;
		y = _y;
		initial_offset = offset;
		h_spacing = 50;
		@img = _img;
		w = h_spacing + img.getWidth();
		speed = _speed;
		count = int(angel::graphics::getWidth() / w + 2);
		initial_img = randi(4);
	}

	void update(int dt)
	{
	}

	void draw()
	{
		float abs_offset = (initial_offset + (speed * g_t));
		float offset = abs_offset % w;
		angel::graphics::setColor(angel::Colorf(1, 1, 1, 0.3f));
		for (int i = 0; i < count; i++) {
			vec2 pos(x + i * (img.getWidth() + h_spacing) + offset - w, y);
			int img_no = int(abs_offset / w);
			int index = (initial_img + 1 + i + 1 - img_no) % 4;
			while (index < 0) {
				index += 4;
			}
			angel::graphics::draw(G::cloud_images[index], pos, -0.05f);
		}
	}
}

class Clouds
{
	float layer_height = 100.0f;

	array<CloudTrack@> tracks;

	Clouds()
	{
		int max = int(angel::graphics::getHeight() / layer_height);
		for (int i = 0; i < max; i++) {
			tracks.insertLast(CloudTrack(0, 20 + i * layer_height, G::img_cloud_1.getWidth() / 2 * (i + 1), 40, G::img_cloud_1));
		}
	}

	void draw()
	{
		for (uint i = 0; i < tracks.length(); i++) {
			tracks[i].draw();
		}
	}
}

float g_t;
float g_step;
const float STEP = 1.0f / 20.0f;

angel::physics::World@ g_world;
Duckloon@ g_duckloon;
Chain@ g_chain;
Clouds@ g_clouds;

array<Entity@> g_entities;

void create_world()
{
	vec2 worldSize = angel::graphics::getDimensions();
	@g_world = angel::physics::newWorld(0, 9.81f * 64);
	@g_duckloon = Duckloon(g_world, worldSize.x / 2, worldSize.y / 2 - 100);
	vec2 attach = g_duckloon.attachment_point();
	@g_chain = Chain(g_world, attach.x, attach.y, g_duckloon);
	@g_clouds = Clouds();

	g_entities.insertLast(g_chain);
	g_entities.insertLast(g_duckloon);
}

void angel_load()
{
	angel::window::setTitle("Angel " + angel::getVersion() + " (" + angel::getVersionCodename() + ")");

	R::loadContent();

	angel::graphics::setBackgroundColor(angel::Color(43, 165, 223, 255));
	angel::physics::setMeter(64);

	@G::img_duckloon_normal = angel::graphics::newImage(R::duckloon::normal_png);
	@G::img_duckloon_blink = angel::graphics::newImage(R::duckloon::blink_png);

	@G::img_n = angel::graphics::newImage(R::chain::n_png);
	@G::img_o = angel::graphics::newImage(R::chain::o_png);
	@G::img_g = angel::graphics::newImage(R::chain::g_png);
	@G::img_a = angel::graphics::newImage(R::chain::a_png);
	@G::img_m = angel::graphics::newImage(R::chain::m_png);
	@G::img_e = angel::graphics::newImage(R::chain::e_png);
	@G::img_square = angel::graphics::newImage(R::chain::square_png);

	@G::img_cloud_1 = angel::graphics::newImage(R::bg::cloud_1_png);
	@G::img_cloud_2 = angel::graphics::newImage(R::bg::cloud_2_png);
	@G::img_cloud_3 = angel::graphics::newImage(R::bg::cloud_3_png);
	@G::img_cloud_4 = angel::graphics::newImage(R::bg::cloud_4_png);

	G::cloud_images.insertLast(G::img_cloud_1);
	G::cloud_images.insertLast(G::img_cloud_2);
	G::cloud_images.insertLast(G::img_cloud_3);
	G::cloud_images.insertLast(G::img_cloud_4);

	create_world();
}

void angel_update(double dt)
{
	g_t += dt;

	while (g_t > g_step) {
		g_world.update(STEP);
		g_step += STEP;

		for (uint i = 0; i < g_entities.length(); i++) {
			g_entities[i].step();
		}
	}

	for (uint i = 0; i < g_entities.length(); i++) {
		g_entities[i].update(dt);
	}
}

void angel_draw()
{
	g_clouds.draw();

	for (uint i = 0; i < g_entities.length(); i++) {
		g_entities[i].draw();
	}
}

void angel_keypressed(string key, string scancode, bool isrepeat)
{
	if (key == "escape") {
		angel::event::quit();
	}
}

void angel_resize()
{
	create_world();
}
