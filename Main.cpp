# include <Siv3D.hpp>
# define SHOTSIZE 10

typedef struct	s_shot
{
	double		PosX;
	double		PosY;
	bool		isshot;
}	t_shot;

typedef struct	s_sprit
{
	double		PosX;
	double		PosY;
	double		speed;
	bool		isFacingRight;
	bool		isFacingDown;
	bool		isHit;
	int			HP;
}	t_sprit;

typedef struct	s_data
{
	const Texture	emoji{ U"Images/player_left.png" };
	const Texture	emoji_left{ U"Images/player_left.png" };
	const Texture	emoji_right{ U"Images/player_right.png" };
	const Texture	emoji_up{ U"Images/player_up.png" };
	const Texture	emoji_down{ U"Images/player_down.png" };
	const Texture	background_03{ U"Images/background_03.png" };
	const Texture	background_04{ U"Images/background_04.png" };
	const Font		font{ FontMethod::MSDF, 48, Typeface::Bold };
	int				timer;
	const Texture	emoji_enemy{ U"🦖"_emoji };
	const Texture	emoji_enemy01{ U"🐟"_emoji };
	const Texture	emoji_enemy02{ U"🚗"_emoji };
	int				score;
	double			mapPosX[5];
	double			mapPosY[5];
	t_shot			shot[10];
	t_sprit			sprit[10];
	int				hit_time;
	int				i;
	bool			clear_flag;
}	t_data;

int	game_init(t_data *data)
{
	int i;

	Scene::SetBackground(ColorF{ Palette::Black });
	data->timer = 0;
	data->score = 0;
	data->i = 0;
	data->clear_flag = false;
	// Player
	data->sprit[0].speed = 2.0;
	data->sprit[0].PosX = 400;
	data->sprit[0].PosY = 200;
	data->sprit[0].isFacingRight = true;
	data->sprit[0].isFacingDown = true;
	// Enemy
	data->sprit[1].speed = 1.0;
	data->sprit[1].PosX = 800;
	data->sprit[1].PosY = 100;
	data->sprit[1].isFacingRight = false;
	data->sprit[1].isHit = false;
	i = 2;
	while (i <= 3)
	{
		data->sprit[i].speed = 1.0;
		data->sprit[i].PosX = -100;
		data->sprit[i].isFacingRight = false;
		data->sprit[i].HP = 3;
		data->sprit[i].isHit = false;
		++i;
	}
	// Boss
	data->sprit[6].speed = 0.5;
	data->sprit[6].PosX = -100;
	data->sprit[6].PosY = -500;
	data->sprit[6].isFacingRight = false;
	data->sprit[6].HP = 30;
	data->sprit[6].isHit = false;
	// Shot
	i = 0;
	while (i <= 4)
	{
		data->shot[i].isshot = false;
		data->shot[i].PosX = -300;
		data->shot[i].PosY = -300;
		++i;
	}
	// Map
	i = 0;
	while (i < 5)
	{
		data->mapPosX[i] = 0;
		data->mapPosY[i] = 0;
		++i;
	}
	return (0);
}

int	map_routine(t_data *data)
{
	data->mapPosX[0] -= 1;
	if (data->mapPosX[0] < -800)
		data->mapPosX[0] = 0;
	return (0);
}

int	enemy_routine(t_data *data)
{
	if (data->timer % 2 == 0)
	{
		data->sprit[1].PosX -= data->sprit[1].speed;
		data->sprit[2].PosX -= data->sprit[2].speed;
		data->sprit[3].PosX -= data->sprit[3].speed;
		if (data->sprit[6].PosX >= 400)
			data->sprit[6].PosX -= data->sprit[6].speed;
		else if (data->sprit[6].PosX >= 0)
		{
			data->sprit[6].PosY -= data->sprit[6].speed;
			if (data->sprit[6].PosY <= 0)
				data->sprit[6].PosY = 600;
		}
	}
	if (data->sprit[1].PosX <= -100)
		data->sprit[1].PosX = 800;
	if (data->timer == 3000)
	{
		data->sprit[2].PosY = 100;
		data->sprit[2].PosX = 800;
		data->sprit[3].PosY = 500;
		data->sprit[3].PosX = 800;
	}
	if (data->timer == 5999)
	{
		data->sprit[6].PosY = 300;
		data->sprit[6].PosX = 800;
	}
	if (data->sprit[1].isHit == true)
	{
		data->sprit[1].PosX = 900;
		data->sprit[1].PosY += 100;
		if (data->sprit[1].PosY > 500)
			data->sprit[1].PosY = 100;
		data->sprit[1].isHit = false;
	}
	if (data->sprit[2].isHit == true)
	{
		data->sprit[2].HP -= 1;
		if (data->sprit[2].HP <= 0)
		{
			data->score = data->score + 50;
			data->sprit[2].PosX = -100;
			data->sprit[2].PosY = -100;
			data->sprit[2].HP = 3;
		}
		data->sprit[2].isHit = false;
	}
	if (data->sprit[3].isHit == true)
	{
		data->sprit[3].HP -= 1;
		if (data->sprit[3].HP <= 0)
		{
			data->score = data->score + 50;
			data->sprit[3].PosX = -100;
			data->sprit[3].PosY = -100;
			data->sprit[3].HP = 3;
		}
		data->sprit[3].isHit = false;
	}
	if (data->sprit[6].isHit == true)
	{
		data->sprit[6].HP -= 1;
		if (data->sprit[6].HP <= 0)
		{
			data->score = data->score + 1000;
			data->sprit[3].PosX = -100;
			data->sprit[3].PosY = -500;
			data->sprit[3].HP = 30;
			data->clear_flag = true;
		}
		data->sprit[6].isHit = false;
	}
	return (0);
}

int shot_routine(t_data *data)
{
	int	i;

	if (KeySpace.down())
	{
		if (data->shot[data->i].isshot == false)
		{
			data->shot[data->i].PosX = data->sprit[0].PosX + 5;
			data->shot[data->i].PosY = data->sprit[0].PosY;
			data->shot[data->i].isshot = true;
			++(data->i);
			if (data->i > 2)
				data->i = 0;
		}
	}
	i = 0;
	while (i < 3)
	{
		if (data->shot[i].isshot == true)
			data->shot[i].PosX += 10;
		else
			data->shot[i].PosX = -200;
		if (data->shot[i].PosX > 800)
			data->shot[i].isshot = false;
		++i;
	}
	return (0);
}

int	player_routine(t_data *data)
{
	if (KeyLeft.pressed())
	{
		data->sprit[0].PosX -= data->sprit[0].speed;
		data->sprit[0].isFacingRight = false;
	}
	if (KeyRight.pressed())
	{
		data->sprit[0].PosX += data->sprit[0].speed;
		data->sprit[0].isFacingRight = true;
	}
	if (KeyUp.pressed())
	{
		data->sprit[0].PosY -= data->sprit[0].speed;
		data->sprit[0].isFacingDown = false;
	}
	if (KeyDown.pressed())
	{
		data->sprit[0].PosY += data->sprit[0].speed;
		data->sprit[0].isFacingDown = true;
	}
	shot_routine(data);
	return (0);
}

int judgement_routine(t_data *data)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (Circle{ data->shot[i].PosX, data->shot[i].PosY, SHOTSIZE }.intersects(Circle{ data->sprit[1].PosX, data->sprit[1].PosY, 30 }))
		{
			data->sprit[1].isHit = true;
			data->shot[i].isshot = false;
			data->hit_time = data->timer + 100;
			data->score = data->score + 10;
		}
		if (Circle{ data->shot[i].PosX, data->shot[i].PosY, SHOTSIZE }.intersects(Circle{ data->sprit[2].PosX, data->sprit[2].PosY, 30 }))
		{
			data->sprit[2].isHit = true;
			data->shot[i].isshot = false;
			data->hit_time = data->timer + 100;
		}
		if (Circle{ data->shot[i].PosX, data->shot[i].PosY, SHOTSIZE }.intersects(Circle{ data->sprit[3].PosX, data->sprit[3].PosY, 30 }))
		{
			data->sprit[3].isHit = true;
			data->shot[i].isshot = false;
			data->hit_time = data->timer + 100;
		}
		if (Circle{ data->shot[i].PosX, data->shot[i].PosY, SHOTSIZE }.intersects(Circle{ data->sprit[6].PosX, data->sprit[6].PosY, 60 }))
		{
			data->sprit[6].isHit = true;
			data->shot[i].isshot = false;
			data->hit_time = data->timer + 100;
		}
		++i;
	}
	return (0);
}

int	print(t_data *data)
{
	int i;

	data->background_03.draw(data->mapPosX[0], data->mapPosY[0]);
	i = 0;
	while (i < 16)
	{
		data->background_04.draw(50 * i, 0);
		++i;
	}
	i = 0;
	while (i < 16)
	{
		data->background_04.draw(50 * i, 570);
		++i;
	}
	data->emoji.scaled(0.75).mirrored(data->sprit[0].isFacingRight).drawAt(data->sprit[0].PosX, data->sprit[0].PosY);
	data->emoji_enemy.scaled(0.75).mirrored(data->sprit[1].isFacingRight).drawAt(data->sprit[1].PosX, data->sprit[1].PosY);
	data->emoji_enemy01.scaled(0.75).mirrored(data->sprit[2].isFacingRight).drawAt(data->sprit[2].PosX, data->sprit[2].PosY);
	data->emoji_enemy01.scaled(0.75).mirrored(data->sprit[3].isFacingRight).drawAt(data->sprit[3].PosX, data->sprit[3].PosY);
	data->emoji_enemy02.scaled(2).mirrored(data->sprit[6].isFacingRight).drawAt(data->sprit[6].PosX, data->sprit[6].PosY);
	i = 0;
	while (i < 3)
	{
		if (data->shot[i].isshot == true)
		{
			Circle{ data->shot[i].PosX, data->shot[i].PosY, SHOTSIZE}.draw(Palette::Seagreen);
		}
		++i;
	}
	data->font(U"Your Score:", data->score).draw(32, Vec2{ 20, 500 }, { Palette::Mistyrose});
	if (data->timer <= data->hit_time)
	{
		if (data->clear_flag == false)
			data->font(U"Hit!").draw(64, Vec2{ 0, 0 }, { Palette::Gold });
		else
		{
			data->font(U"Congraduation!").draw(128, Vec2{ 0, 0 }, { Palette::Gold });
			System::Sleep(5000ms);
			System::Exit();
		}
	}
	return (0);
}

int	timer_routine(t_data *data)
{
	++(data->timer);
	if (data->timer > 6000)
		data->timer = 0;
	return (0);
}

void Main()
{
	t_data	data;

	game_init(&data);
	while (System::Update())
	{
		map_routine(&data);
		enemy_routine(&data);
		player_routine(&data);
		judgement_routine(&data);
		print(&data);
		timer_routine(&data);
	}
	return ;
}
