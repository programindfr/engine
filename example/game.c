#include <engine.h>

int main(int argc, char *argv[])
{
	size_t	   i;
	uint8_t	  loop = 1;
	SDL_FRect	rectb = { 0, 0, 16, 16 };
	SDL_FRect	rectp = { 3, 3, 10, 12 };
	SDL_FRect	screen = { 0, 0, 400, 250 };
	layer_t	  layer = NO_LAYER;
	float		x, y;
	
	Window_t *myGame = Window();
	Entity_t *player = NULL;
	QTree_t *tree = QTree(screen);
	CList_t *list = NULL;
	clist_block_t *block = NULL;
	
	for (i = 0; i < 400; ++i)
	{
		x = 16 * (i % 25);
		y = 16 * (i / 25);

		if (x == 0 || y == 0 || x == 24*16 || y == 15*16)
		{
			player = Entity(
				myGame,
				x,
				y,
				LAYER_01 | LAYER_02,
				rectb,
				"./assets/Tiles/tile_0075.png"
			);
			tree->qtree.insert(tree, player);
		}
	}

	for (i = 0; i < 348; ++i)
	{
		x = 16 + 16 * (i % 25);
		y = 16 + 16 * (i / 25);

		player = Entity(
			myGame,
			x,
			y,
			LAYER_01,
			rectb,
			"./assets/Tiles/tile_0028.png"
		);
		tree->qtree.insert(tree, player);
	}

	player = Entity(myGame, 32, 128, LAYER_02 | LAYER_03, rectp, "./assets/Tiles/tile_0024.png");
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
	
	while (loop)
	{
		block = NULL;
		list = tree->qtree.fetch(tree, screen);
		player = list->clist.iter(list, &block);
		
		while (player)
		{
			layer = player->entity.getLayer(player);
			if (layer & LAYER_01)
			{
				player->entity.update(player);
				player->entity.draw(player);
			}
			player = list->clist.iter(list, &block);
		}

		player = list->clist.pop(list);
		while (player)
		{
			layer = player->entity.getLayer(player);
			if (layer & LAYER_03)
			{
				player->entity.update(player);
				player->entity.draw(player);
			}
			player = list->clist.pop(list);
		}
		
		delete(list);
		
		tree->qtree.update(tree);
		/*tree->qtree.draw(tree, myGame);*/
		
		loop = myGame->window.update(myGame);
	}
	
	delete(tree);
	delete(myGame);
	
	return 0;
}
