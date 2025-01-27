#include <unistd.h>
#include <engine.h>

int main(void)
{
	SDL_FRect rectp = {10, 10, 16, 16};
	Window_t *myGame = Window();
	Entity_t *player = Entity(myGame, rectp, LAYER_01, "./tile_0075.png");
	
	player->entity.draw(player);
	myGame->window.update(myGame);
	
	sleep(5);
	
	delete(player);
	delete(myGame);
	
	return 0;
}
