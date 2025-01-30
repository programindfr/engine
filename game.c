#include <engine.h>

int main(void)
{
	uint8_t		loop = 1;
	SDL_FRect	rectp = {10, 10, 16, 16};
	
	Window_t *myGame = Window();
	Entity_t *player = Entity(myGame, rectp, LAYER_01, "./tile_0075.png");
	
	while (loop)
	{
		player->entity.update(player);
		player->entity.draw(player);
		loop = myGame->window.update(myGame);
	}
	
	delete(player);
	delete(myGame);
	
	return 0;
}
