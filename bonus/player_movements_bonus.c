/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movements_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:39:30 by auspensk          #+#    #+#             */
/*   Updated: 2025/01/24 11:27:00 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	can_walk(t_data *data, int x, int y)
{
	char	c;
	int		i;
	t_door	*door;

	c = data->map[y][x];
	i = -1;
	if (c == '0')
		return (1);
	if (c == 'D')
	{
		while (++i < 24 && data->doors[i].x)
		{
			if (data->doors[i].x == x && data->doors[i].y == y)
				door = &data->doors[i];
		}
		if (door && door->state == 2)
			return (1);
	}
	return (0);
}

void	make_step(t_data *data, t_coord step)
{
	int	y;
	int	new_y;
	int	x;
	int	new_x;

	y = (int)data->player.y;
	new_y = (int)(data->player.y + step.y);
	x = (int)data->player.x;
	new_x = (int)(data->player.x + step.x);
	if (can_walk(data, new_x, y))
		data->player.x += step.x;
	if (can_walk(data, x, new_y))
		data->player.y += step.y;
}

void	move_player(t_data *data, int key)
{
	t_coord	step;
	double	dist;

	if (key == XK_w)
		step = data->dir;
	else if (key == XK_s)
		step = rotate_vector(data->dir, PI);
	else if (key == XK_a)
		step = rotate_vector(data->dir, -PI / 2);
	else if (key == XK_d)
		step = rotate_vector(data->dir, PI / 2);
	dist = 0.2;
	step.x *= dist;
	step.y *= dist;
	make_step(data, step);
}

void	rotate_player(t_data *data, int key)
{
	double	angle;

	if (key == XK_Right)
		angle = 0.05 * PI;
	else
		angle = 0.05 * -PI;
	data->dir = rotate_vector(data->dir, angle);
	data->plane = rotate_vector(data->plane, angle);
}
