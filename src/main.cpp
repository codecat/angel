#include <cstdio>

#include <common/version.h>
#include <common/Module.h>

#include <modules/love/love.h>

#include <modules/filesystem/Filesystem.h>
#include <modules/filesystem/physfs/Filesystem.h>

#include <SDL.h>

enum DoneAction
{
	done_Restart,
	done_Quit,
};

DoneAction runangel()
{
	DoneAction done = done_Quit;

	//auto pfs = love::Module::getInstance<love::filesystem::Filesystem>(love::Module::M_FILESYSTEM);
	love::Module::registerInstance(new love::filesystem::physfs::Filesystem());

	printf("%d modules:\n", love::Module::M_MAX_ENUM);
	for (int i = 0; i < love::Module::M_MAX_ENUM; i++) {
		auto mod = love::Module::getInstance<love::Module>((love::Module::ModuleType)i);
		if (mod == nullptr) {
			printf("  ...\n");
		} else {
			printf("  %s\n", mod->getName());
		}
	}

	return done;
}

int main()
{
	printf("Angel %s (%s)\n", love_version(), love_codename());

	DoneAction done = done_Quit;
	do {
		done = runangel();
	} while (done != done_Quit);

	return 0;
}
