namespace R
{
	namespace bg
	{
		angel::Data@ cloud_1_png;
		angel::Data@ cloud_2_png;
		angel::Data@ cloud_3_png;
		angel::Data@ cloud_4_png;
	}

	namespace chain
	{
		angel::Data@ a_png;
		angel::Data@ e_png;
		angel::Data@ g_png;
		angel::Data@ m_png;
		angel::Data@ n_png;
		angel::Data@ o_png;
		angel::Data@ square_png;
	}

	namespace duckloon
	{
		angel::Data@ blink_png;
		angel::Data@ normal_png;
	}
}

class StatePos
{
	float x;
	float y;
	float r;
}

class State
{
	int t0;
	int x0;
	int y0;
	int r0;

	State(/*angel::physics::Body@ body*/)
	{
		//TODO
	}

	void save(/*angel::physics::Body@ body,*/ int t)
	{
		//TODO
	}

	StatePos get(int t)
	{
		StatePos ret;
		//TODO
		return ret;
	}
}

class Blink
{
	int closed_t = 0;
	int next_blink_t = 5;

	void update(int dt)
	{
		//TODO
	}

	bool is_closed()
	{
		return closed_t > 0;
	}
}

class Duckloon
{
	State@ state;
	Blink@ blink;

	Duckloon(/*angel::physics::World@ world, */float x, float y)
	{
		//TODO
		@blink = Blink();
		@state = State(/*body*/);
	}

	void step()
	{
		//TODO
	}

	void update(int dt)
	{
		blink.update(dt);
	}

	void draw()
	{
		//TODO
	}

	vec2 attachment_point()
	{
		//TODO
		return vec2();
	}
}

class ChainLink
{
	float x;
	float y;
}

class Chain
{
	Chain(/*angel::physics::World@ world, */float x, float y, Duckloon@ duckloon)
	{
		//TODO
	}

	void step()
	{
		//TODO
	}

	void update(int dt)
	{
	}

	void draw()
	{
		//TODO
	}
}

class CloudTrack
{
	float x;
	float y;

	CloudTrack(float _x, float _y, float offset, float speed/*, angel::graphics::Image@ img*/)
	{
		x = _x;
		y = _y;
		//TODO
	}

	void update(int dt)
	{
	}

	void draw()
	{
		//TODO
	}
}

class Clouds
{
	float layer_height = 100.0f;

	Clouds()
	{
		//TODO
	}

	void draw()
	{
		//TODO
	}
}

void create_world()
{
}

void angel_load()
{
	angel::graphics::setBackgroundColor(angel::Color(43, 165, 223, 255));
}
