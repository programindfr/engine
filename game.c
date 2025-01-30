#include <engine.h>

int main(void)
{
	uint8_t		loop = 1;
	SDL_FRect	rectp = {10, 10, 16, 16};
	
	Window_t *myGame = Window();
	Entity_t *player = Entity(myGame, rectp, LAYER_01, "./tile_0075.png");
	
	player->entity.transition(player, 0, SDL_KEYDOWN, SDLK_d, ACT_01, 0);
	player->entity.transition(player, 0, SDL_KEYUP, SDLK_d, ACT_03, 0);
	player->entity.transition(player, 0, SDL_KEYDOWN, SDLK_q, ACT_02, 0);
	player->entity.transition(player, 0, SDL_KEYUP, SDLK_q, ACT_03, 0);
	player->entity.transition(player, 0, SDL_KEYDOWN, SDLK_s, ACT_04, 0);
	player->entity.transition(player, 0, SDL_KEYUP, SDLK_s, ACT_06, 0);
	player->entity.transition(player, 0, SDL_KEYDOWN, SDLK_z, ACT_05, 0);
	player->entity.transition(player, 0, SDL_KEYUP, SDLK_z, ACT_06, 0);
	
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
