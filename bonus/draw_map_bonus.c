/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:43:36 by auspensk          #+#    #+#             */
/*   Updated: 2024/11/25 15:44:29 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

t_ray	init_ray(t_data *md, int x)
{
	t_ray	ray;
	double	rel_pos;

	rel_pos = 2 * x / (double)SCRNWIDTH - 1;
	ray.dir.x = md->dir.x + md->plane.x * rel_pos;
	ray.dir.y = md->dir.y + md->plane.y * rel_pos;
	ray.origin.x = md->player.x;
	ray.origin.y = md->player.y;
	return (ray);
}

t_dda	init_dda(t_coord player)
{
	t_dda	dda;

	dda.delta_x = 0;
	dda.delta_y = 0;
	dda.map_x = (int)player.x;
	dda.map_y = (int)player.y;
	dda.side = 0;
	dda.side_dist_x = 0;
	dda.side_dist_y = 0;
	dda.step_x = 1;
	dda.step_y = 1;
	dda.hit = 0;
	return (dda);
}

void	get_deltas(t_ray ray, t_dda *dda)
{
	dda->delta_x = DBL_MAX;
	if (ray.dir.x)
		dda->delta_x = fabs(1 / ray.dir.x);
	dda->delta_y = DBL_MAX;
	if (ray.dir.y)
		dda->delta_y = fabs(1 / ray.dir.y);
	if (ray.dir.x < 0)
	{
		dda->step_x = -1;
		dda->side_dist_x = (ray.origin.x - dda->map_x) * dda->delta_x;
	}
	else
		dda->side_dist_x = (dda->map_x + 1 - ray.origin.x) * dda->delta_x;
	if (ray.dir.y < 0)
	{
		dda->step_y = -1;
		dda->side_dist_y = (ray.origin.y - dda->map_y) * dda->delta_y;
	}
	else
		dda->side_dist_y = (dda->map_y + 1 - ray.origin.y) * dda->delta_y;
}

void	perform_dda(t_dda *dda, char **map)
{
	while (dda->hit == 0)
	{
		if (dda->side_dist_x < dda->side_dist_y)
		{
			dda->side_dist_x += dda->delta_x;
			dda->map_x += dda->step_x;
			if (dda->step_x == -1)
				dda->side = WEST;
			else
				dda->side = EAST;
		}
		else
		{
			dda->side_dist_y += dda->delta_y;
			dda->map_y += dda->step_y;
			if (dda->step_y == -1)
				dda->side = NORTH;
			else
				dda->side = SOUTH;
		}
		if (map[dda->map_y][dda->map_x] != '0')
			dda->hit = map[dda->map_y][dda->map_x];
	}
}

void	draw_frame(t_data *data)
{
	t_ray		ray_vect;
	t_dda		dda;
	int			x;
	t_draw_data	draw_data;

	data->can_open = NULL;
	data->sprite = NULL;
	data->redraw = 0;
	x = -1;
	while (++x < SCRNWIDTH)
	{
		ray_vect = init_ray(data, x);
		dda = init_dda(data->player);
		get_deltas(ray_vect, &dda);
		perform_dda(&dda, data->map);
		calc_line_height(dda, &draw_data);
		calc_wall_txtr_x(dda, &draw_data, data, ray_vect);
		draw_line_to_img(data, x, &draw_data, -1);
		data->buffer[x] = draw_data.per_wall_dist;
	}
	handle_sprite(data);
}
