#include <engine.h>

int main(void)
{
	size_t		i;
	uint8_t		loop = 1;
	SDL_FRect	rectp = {10, 10, 16, 16};
	SDL_FRect	screen = {0, 0, 800, 600};
	
	Window_t *myGame = Window();
	Entity_t *player = NULL;
	QTree_t *tree = QTree(screen);
	CList_t *list = NULL;
	
	for (i = 0; i < 5; ++i)
	{
		player = Entity(myGame, rectp, LAYER_01, "./assets/tile_0075.png");
		player->entity.transition(player, 0, SDL_KEYDOWN, SDLK_d, ACT_01, 0);
		player->entity.transition(player, 0, SDL_KEYUP, SDLK_d, ACT_03, 0);
		player->entity.transition(player, 0, SDL_KEYDOWN, SDLK_q, ACT_02, 0);
		player->entity.transition(player, 0, SDL_KEYUP, SDLK_q, ACT_03, 0);
		player->entity.transition(player, 0, SDL_KEYDOWN, SDLK_s, ACT_04, 0);
		player->entity.transition(player, 0, SDL_KEYUP, SDLK_s, ACT_06, 0);
		player->entity.transition(player, 0, SDL_KEYDOWN, SDLK_z, ACT_05, 0);
		player->entity.transition(player, 0, SDL_KEYUP, SDLK_z, ACT_06, 0);
		player->entity.transition(player, 0, SDL_KEYDOWN, SDLK_p, NO_ACT, 1);
		player->entity.transition(player, 1, SDL_KEYDOWN, SDLK_p, NO_ACT, 0);
		
		tree->qtree.insert(tree, player);
	}
	
	while (loop)
	{
		list = tree->qtree.fetch(tree, screen);
		player = list->clist.pop(list);
		
		while (player)
		{
			player->entity.update(player);
			player->entity.draw(player);
			player = list->clist.pop(list);
		}
		delete(list);
		
		tree->qtree.update(tree);
		tree->qtree.draw(tree, myGame);
		
		loop = myGame->window.update(myGame);
	}
	
	delete(tree);
	delete(myGame);
	
	return 0;
}
