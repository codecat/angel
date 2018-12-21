namespace path
{
	string normalslashes(const string &in p)
	{
		return p.replace("\\", "/");
	}

	string endslash(const string &in p)
	{
		if (p.length() == 0 || p.substr(p.length() - 1) != "/") {
			return p + "/";
		}
		return p;
	}

	bool abs(const string &in p)
	{
		if (p.length() > 0) {
			string tmp = normalslashes(p);
			if (tmp.substr(0, 1) == "/") {
				return true;
			}

			if (tmp.length() > 2 && tmp.substr(1, 1) == ":") {
				return true;
			}
		}

		return false;
	}

	string getFull(const string &in p)
	{
		if (abs(p)) {
			return normalslashes(p);
		}

		string cwd = angel::filesystem::getWorkingDirectory();
		cwd = normalslashes(cwd);
		cwd = endslash(cwd);

		string full = cwd + normalslashes(p);
		if (full.substr(full.length() - 2) == "/.") {
			full = full.substr(0, full.length() - 2);
		}

		return full;
	}

	string leaf(string p)
	{
		p = normalslashes(p);
		if (p.length() == 0) {
			return p;
		}

		if (p.substr(p.length() - 1) == "/") {
			p = p.substr(0, p.length() - 1);
		}

		int index = p.findLast("/");
		if (index == -1) {
			return p;
		}

		return p.substr(index + 1);
	}
}

void angel_boot()
{
	string exepath = angel::filesystem::getExecutablePath();

	bool canHasGame = angel::filesystem::setSource(exepath);
	angel::filesystem::setFused(canHasGame);

	string identity = "";

	if (!canHasGame) {
		string fullSource = path::getFull("game/");
		canHasGame = angel::filesystem::setSource(fullSource);
		identity = path::leaf(fullSource);
	} else {
		identity = path::leaf(exepath);
	}

	angel::filesystem::setIdentity(identity);

	if (!canHasGame) {
		print("No game found");
	}
}

void angel_init()
{
	angel::window::WindowSettings settings;
	if (!angel::window::setMode(800, 600, settings)) {
		print("Could not set window mode");
		return;
	}

	angel::timer::step();
}

void angel_run()
{
	angel_init();

	angel::game_load();

	while (true) {
		angel::event::pump();

		angel::event::Message@ msg = angel::event::poll();
		while (msg !is null) {
			if (msg.getName() == "quit") {
				return;
			}

			@msg = angel::event::poll();
		}

		double dt = angel::timer::step();
		angel::game_update(dt);

		//TODO: graphics stuff
		angel::game_draw();
	}
}
