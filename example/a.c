#include <engine.h>
#include <example/b.h>

int main(int argc, char *argv[])
{
	uint8_t		loop = 1;
	Window_t	*myGame = Window();
	
	while (loop)
	{
		loop = myGame->window.update(myGame);
	}
	
	waitazerty();
	
	delete(myGame);
	
	return 0;
}
