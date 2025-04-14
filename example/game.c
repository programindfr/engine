#include <engine.h>

int main(int argc, char *argv[])
{
	int		  day = -1;
	size_t	   i;
	size_t	   light = 255;
	SDL_Color	lighting;
	uint8_t	  loop = 1;
	SDL_FRect	rectb = { 0, 0, 16, 16 };
	SDL_FRect	rectp = { 3, 3, 10, 12 };
	SDL_FRect	rectt = { 6, 8, 4, 6 };
	SDL_FRect	screen = { 0, 0, 400, 250 };
	layer_t	  layer = NO_LAYER;
	float		x, y;
	SDL_Color	color = { 255, 200, 128, 255 };
	
	Window_t *myGame = Window();
	Entity_t *player = NULL;
	QTree_t *tree = QTree(screen);
	CList_t *list = NULL;
	clist_block_t *block = NULL;
	lighting.a = 255;

	srand(SDL_GetTicks64());
	
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

	for (i = 0; i < 322; ++i)
	{
		x = 16 + 16 * (i % 23);
		y = 16 + 16 * (i / 23);

		player = Entity(
			myGame,
			x,
			y,
			LAYER_01,
			rectb,
			"./assets/Tiles/tile_0028.png"
		);
		tree->qtree.insert(tree, player);

		if (rand() % 10 == 0)
		{
			player = Entity(
				myGame,
				x,
				y,
				LAYER_03 | LAYER_04,
				rectt,
				"./assets/Tiles/tile_0259.png"
			);
			tree->qtree.insert(tree, player);

			player = Entity(
				myGame,
				x,
				y - 16,
				LAYER_05,
				rectb,
				"./assets/Tiles/tile_0232.png"
			);
			tree->qtree.insert(tree, player);
		}
	}

	player = Entity(myGame, 32, 128, LAYER_02 | LAYER_03, rectp, "./assets/Tiles/tile_0024.png");
	player->entity.setLighting(player, 16, color);
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
				player->entity.draw(player);
			}
			player = list->clist.iter(list, &block);
		}

		block = NULL;
		player = list->clist.iter(list, &block);
		
		while (player)
		{
			layer = player->entity.getLayer(player);
			if (layer & LAYER_03)
			{
				player->entity.update(player);
				player->entity.draw(player);
			}
			player = list->clist.iter(list, &block);
		}

		block = NULL;
		player = list->clist.iter(list, &block);
		
		while (player)
		{
			layer = player->entity.getLayer(player);
			if (layer & LAYER_04)
			{
				player->entity.draw(player);
			}
			player = list->clist.iter(list, &block);
		}

		player = list->clist.pop(list);
		while (player)
		{
			layer = player->entity.getLayer(player);
			if (layer & LAYER_05)
			{
				player->entity.draw(player);
			}
			player = list->clist.pop(list);
		}
		
		delete(list);
		
		tree->qtree.update(tree);
		/*tree->qtree.draw(tree, myGame);*/
		lighting.r = light / 100;
		lighting.g = light / 100;
		lighting.b = light / 100;
		light += day;
		if (light == 0 || light == 25500)
			day *= -1;
		
		myGame->window.setLighting(myGame, lighting);
		
		loop = myGame->window.update(myGame);
	}
	
	delete(tree);
	delete(myGame);
	
	return 0;
}
