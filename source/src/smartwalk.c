///////////////////////////////

SMARTWALKDATA* smartwalkdata_create()
{
	SMARTWALKDATA* data;
	
	data = (SMARTWALKDATA*)sys_malloc(sizeof(SMARTWALKDATA));
	data->num_regions = 0;
	data->neighbors = NULL;
	data->path_prev = NULL;
	data->path_stack = NULL;
	data->path_dist = NULL;
	data->regionMin = NULL;
	data->regionMax = NULL;
	data->center = NULL;
	data->edgeMin = NULL;
	data->edgeMax = NULL;
	
	return data;
}

void smartwalkdata_destroy(SMARTWALKDATA* data)
{
	sys_free(data);
}

SMARTWALK* smartwalk_create()
{
	SMARTWALK* walk;
	
	walk = (SMARTWALK*)sys_malloc(sizeof(SMARTWALK));
	walk->current_node = 0;
	walk->num_nodes = 0;
	walk->direct = 0;
	walk->nodes = NULL;

	return walk;
}

void smartwalk_destroy(SMARTWALK* walk)
{
	if(walk->nodes) sys_free(walk->nodes);
	sys_free(walk);
}

///////////////////////////

void smartwalkdata_fill(SMARTWALKDATA* data)
{
	var buffer = 1,diff_x,diff_y,line_x,line_y;
	int i,j,k,nodes;
	VECTOR vmin,vmax,size_i,size_j;
	STRING* str_tmp;
	
	str_tmp = str_create("");
	i = 0;
	while(region_get(NULL, i+1, vmin, vmax)) i++;
	if(i != data->num_regions)
	{
		data->num_regions = i;

		if(data->condition) sys_free(data->condition);
		data->condition = (int*)sys_malloc(sizeof(int)*data->num_regions);

		if(data->neighbors) sys_free(data->neighbors);
		data->neighbors = (int*)sys_malloc(sizeof(int)*data->num_regions*data->num_regions);

		if(data->path_prev) sys_free(data->path_prev);
		data->path_prev = (int*)sys_malloc(sizeof(int)*data->num_regions);

		if(data->path_stack) sys_free(data->path_stack);
		data->path_stack = (int*)sys_malloc(sizeof(int)*data->num_regions);
		
		if(data->path_dist) sys_free(data->path_dist);
		data->path_dist = (int*)sys_malloc(sizeof(int)*data->num_regions);

		if(data->regionMin) sys_free(data->regionMin);
		data->regionMin = (VECTOR*)sys_malloc(sizeof(VECTOR)*data->num_regions);
		
		if(data->regionMax) sys_free(data->regionMax);
		data->regionMax = (VECTOR*)sys_malloc(sizeof(VECTOR)*data->num_regions);
		
		if(data->center) sys_free(data->center);
		data->center = (VECTOR*)sys_malloc(sizeof(VECTOR)*data->num_regions);

		if(data->edgeMin) sys_free(data->edgeMin);
		data->edgeMin = (VECTOR*)sys_malloc(sizeof(VECTOR)*data->num_regions*data->num_regions);
		
		if(data->edgeMax) sys_free(data->edgeMax);
		data->edgeMax = (VECTOR*)sys_malloc(sizeof(VECTOR)*data->num_regions*data->num_regions);
	}
	
	for(i = 0; i < data->num_regions; i++)
	{
		region_get(NULL, i+1, (data->regionMin)[i], (data->regionMax)[i]);
		vec_lerp((data->center)[i],(data->regionMin)[i], (data->regionMax)[i],0.5);
		for(j = 0; j < data->num_regions; j++) (data->neighbors)[i*data->num_regions+j] = -1;
	}
	for(k = 0; k < SMWALK_CONDITION_MAX; k++)
	{
		str_printf(str_tmp,"cond_0%d",(int)k);
		j = 1;
		while(region_get(str_tmp, j, vmin, vmax))
		{
			// slow hack :/, praise var though!
			for(i = 0; i < data->num_regions; i++)
			{
				if((data->regionMin)[i].x == vmin.x && (data->regionMin)[i].y == vmin.y
				&& (data->regionMax)[i].x == vmax.x && (data->regionMax)[i].y == vmax.y) (data->condition)[i] = k+1;
			}
			j++;
		}
	}
	
	for(i = 0; i < data->num_regions; i++)
	{
		for(j = 0; j < data->num_regions; j++)
		{
			if(i == j) (data->neighbors)[i*data->num_regions+j] = j;
			else
			{
				if(!((data->regionMin)[i].x > (data->regionMax)[j].x+buffer
				|| (data->regionMin)[j].x > (data->regionMax)[i].x+buffer
				|| (data->regionMin)[i].y > (data->regionMax)[j].y+buffer
				|| (data->regionMin)[j].y > (data->regionMax)[i].y+buffer)) (data->neighbors)[i*data->num_regions+j] = j;
			}
		}
	}
	
	for(i = 0; i < data->num_regions; i++)
	{
		vec_lerp((data->center)[i],(data->regionMin)[i], (data->regionMax)[i],0.5);
		vec_set(size_i,vector((data->regionMax)[i].x-(data->regionMin)[i].x,(data->regionMax)[i].y-(data->regionMin)[i].y,0));
		for(j = 0; j < data->num_regions; j++)
		{
			vec_set((data->edgeMin)[i*data->num_regions+j],vector(0,0,32));
			vec_set((data->edgeMax)[i*data->num_regions+j],vector(0,0,32));
			if(i == j || (data->neighbors)[i*data->num_regions+j] < 0) continue;
			vec_lerp((data->center)[j],(data->regionMin)[j], (data->regionMax)[j],0.5);
			vec_set(size_j,vector((data->regionMax)[j].x-(data->regionMin)[j].x,(data->regionMax)[j].y-(data->regionMin)[j].y,0));
			diff_x = (size_i.x+size_j.x)*0.5-abs((data->center)[i].x-(data->center)[j].x);
			diff_y = (size_i.y+size_j.y)*0.5-abs((data->center)[i].y-(data->center)[j].y);
			if(diff_x > diff_y)
			{
				if((data->center)[i].y > (data->center)[j].y) line_y = (data->regionMin)[i].y;
				else line_y = (data->regionMax)[i].y;
				(data->edgeMin)[i*data->num_regions+j].y = line_y;
				(data->edgeMax)[i*data->num_regions+j].y = line_y;
				nodes = 0;
				if(size_i.x > size_j.x)
				{
					if((data->regionMin)[i].x <= (data->regionMin)[j].x && (data->regionMin)[j].x <= (data->regionMax)[i].x)
					{
						(data->edgeMin)[i*data->num_regions+j].x = (data->regionMin)[j].x;
						nodes++;
					}
					if((data->regionMin)[i].x <= (data->regionMax)[j].x && (data->regionMax)[j].x <= (data->regionMax)[i].x)
					{
						if(!nodes) (data->edgeMin)[i*data->num_regions+j].x = (data->regionMax)[j].x;
						else (data->edgeMax)[i*data->num_regions+j].x = (data->regionMax)[j].x;
						nodes++;
					}
					if(!nodes) error("no node found!");
					if(nodes < 2)
					{
						if((data->edgeMin)[i*data->num_regions+j].x < (data->center)[j].x) (data->edgeMax)[i*data->num_regions+j].x = (data->regionMax)[i].x;
						else (data->edgeMax)[i*data->num_regions+j].x = (data->regionMin)[i].x;
					}
				}
				else
				{
					if((data->regionMin)[j].x <= (data->regionMin)[i].x && (data->regionMin)[i].x <= (data->regionMax)[j].x)
					{
						(data->edgeMin)[i*data->num_regions+j].x = (data->regionMin)[i].x;
						nodes++;
					}
					if((data->regionMin)[j].x <= (data->regionMax)[i].x && (data->regionMax)[i].x <= (data->regionMax)[j].x)
					{
						if(!nodes) (data->edgeMin)[i*data->num_regions+j].x = (data->regionMax)[i].x;
						else (data->edgeMax)[i*data->num_regions+j].x = (data->regionMax)[i].x;
						nodes++;
					}
					if(!nodes) error("no node found! 2");
					if(nodes < 2)
					{
						if((data->edgeMin)[i*data->num_regions+j].x < (data->center)[i].x) (data->edgeMax)[i*data->num_regions+j].x = (data->regionMax)[j].x;
						else (data->edgeMax)[i*data->num_regions+j].x = (data->regionMin)[j].x;
					}
				}
			}
			else
			{
				if((data->center)[i].x > (data->center)[j].x) line_x = (data->regionMin)[i].x;
				else line_x = (data->regionMax)[i].x;
				(data->edgeMin)[i*data->num_regions+j].x = line_x;
				(data->edgeMax)[i*data->num_regions+j].x = line_x;
				nodes = 0;
				if(size_i.y > size_j.y)
				{
					if((data->regionMin)[i].y <= (data->regionMin)[j].y && (data->regionMin)[j].y <= (data->regionMax)[i].y)
					{
						(data->edgeMin)[i*data->num_regions+j].y = (data->regionMin)[j].y;
						nodes++;
					}
					if((data->regionMin)[i].y <= (data->regionMax)[j].y && (data->regionMax)[j].y <= (data->regionMax)[i].y)
					{
						if(!nodes) (data->edgeMin)[i*data->num_regions+j].y = (data->regionMax)[j].y;
						else (data->edgeMax)[i*data->num_regions+j].y = (data->regionMax)[j].y;
						nodes++;
					}
					if(!nodes) error("no node found!");
					if(nodes < 2)
					{
						if((data->edgeMin)[i*data->num_regions+j].y < (data->center)[j].y) (data->edgeMax)[i*data->num_regions+j].y = (data->regionMax)[i].y;
						else (data->edgeMax)[i*data->num_regions+j].y = (data->regionMin)[i].y;
					}
				}
				else
				{
					if((data->regionMin)[j].y <= (data->regionMin)[i].y && (data->regionMin)[i].y <= (data->regionMax)[j].y)
					{
						(data->edgeMin)[i*data->num_regions+j].y = (data->regionMin)[i].y;
						nodes++;
					}
					if((data->regionMin)[j].y <= (data->regionMax)[i].y && (data->regionMax)[i].y <= (data->regionMax)[j].y)
					{
						if(!nodes) (data->edgeMin)[i*data->num_regions+j].y = (data->regionMax)[i].y;
						else (data->edgeMax)[i*data->num_regions+j].y = (data->regionMax)[i].y;
						nodes++;
					}
					if(!nodes) error("no node found! 2");
					if(nodes < 2)
					{
						if((data->edgeMin)[i*data->num_regions+j].y < (data->center)[i].y) (data->edgeMax)[i*data->num_regions+j].y = (data->regionMax)[j].y;
						else (data->edgeMax)[i*data->num_regions+j].y = (data->regionMin)[j].y;
					}
				}
			}
		}
	}
	ptr_remove(str_tmp);
}


var smartwalkdata_find_region(SMARTWALKDATA* data, VECTOR* vpos, var modify)
{
	int i, closest_region;
	var dist, min_dist, fac;
	VECTOR vtmp, vpdir, vproj;
	
	for(i = 0; i < data->num_regions; i++)
	{
		if(vpos.x > (data->regionMin)[i].x && vpos.y > (data->regionMin)[i].y
		&& vpos.x < (data->regionMax)[i].x && vpos.y < (data->regionMax)[i].y
		&& (!(data->condition)[i] || smwalk_condition[clamp((data->condition)[i]-1,0,SMWALK_CONDITION_MAX-1)])) break;
	}
	if(i == data->num_regions)
	{
		min_dist = 999999;
		closest_region = -1;
		for(i = 0; i < data->num_regions; i++)
		{
			if((data->condition)[i] && !smwalk_condition[clamp((data->condition)[i]-1,0,SMWALK_CONDITION_MAX-1)]) continue;
			vec_diff(vpdir,vpos,(data->regionMin)[i]);
			fac = clamp(vpdir.x/(double)((data->regionMax)[i].x-(data->regionMin)[i].x),0,1);
			vtmp.x = (data->regionMin)[i].x*(1-fac) + (data->regionMax)[i].x*fac;
			fac = clamp(vpdir.y/(double)((data->regionMax)[i].y-(data->regionMin)[i].y),0,1);
			vtmp.y = (data->regionMin)[i].y*(1-fac) + (data->regionMax)[i].y*fac;
			vtmp.z = vpos.z;
			dist = vec_dist(vpos,vtmp);
			if(dist < min_dist)
			{
				min_dist = dist;
				closest_region = i;
				vec_set(vproj,vtmp);
			}
		}
		i = closest_region;
		if(modify) vec_set(vpos,vproj);
	}
	

	return i;
}

void line_projection(VECTOR* vresult, VECTOR* vpoint, VECTOR* vstart, VECTOR* vtarget)
{
	VECTOR vline,vpointdir;
	double dot1,dot2,fac;

	vec_diff(vline,vtarget,vstart);
	vec_diff(vpointdir,vpoint,vstart);
	dot1 = (double)vline.x*vline.x + (double)vline.y*vline.y;
	dot2 = (double)vline.x*vpointdir.x + (double)vline.y*vpointdir.y;
	fac = dot2/dot1;
	if(fac < 0) fac = 0;
	if(fac > 1) fac = 1;
	vec_lerp(vresult,vstart,vtarget,fac);
}

SMARTWALK* smartwalk_create_path(SMARTWALKDATA* data, VECTOR* vstart, VECTOR* vtarget)
{
	int i,j,start_region,target_region,current_region,stack_region,stack_region_max,path_num_regions,previous_region;
	var dist;
	VECTOR vproj,vproj1,vproj2;
	SMARTWALK* walk;


	start_region = smartwalkdata_find_region(data, vstart, false);
	target_region = smartwalkdata_find_region(data, vtarget, true);

	if(start_region < 0 || target_region < 0) return NULL;
	if(start_region == target_region)
	{
		walk = smartwalk_create();
		walk->num_nodes = 2;
		walk->nodes = (VECTOR*)sys_malloc(sizeof(VECTOR)*2);
		vec_set((walk->nodes)[0],vstart);
		vec_set((walk->nodes)[1],vtarget);
		walk->direct = 1;
		return walk;
	}

	for(j = 0; j < data->num_regions; j++)
	{
		(data->path_prev)[j] = -1;
		(data->path_dist)[j] = 999999;
	}
	(data->path_prev)[target_region] = target_region;
	(data->path_dist)[target_region] = 0;
	(data->path_stack)[0] = target_region;
	stack_region = 0;
	stack_region_max = 1;
	while(stack_region < stack_region_max)
	{
		current_region = (data->path_stack)[stack_region];
		for(j = 0; j < data->num_regions; j++)
		{
			if(j == current_region || (data->neighbors)[current_region*data->num_regions+j] < 0
			|| ((data->condition)[j] && !smwalk_condition[clamp((data->condition)[j]-1,0,SMWALK_CONDITION_MAX-1)])) continue;
			line_projection(vproj1,(data->center)[j],(data->edgeMin)[current_region*data->num_regions+j],(data->edgeMax)[current_region*data->num_regions+j]);
			line_projection(vproj2,(data->center)[current_region],(data->edgeMin)[current_region*data->num_regions+j],(data->edgeMax)[current_region*data->num_regions+j]);
			vec_lerp(vproj,vproj1,vproj2,0.5);
			dist = vec_dist((data->center)[j],vproj)+vec_dist((data->center)[current_region],vproj);
			if((data->path_prev)[j] < 0)
			{
				(data->path_dist)[j] = (data->path_dist)[current_region]+dist;
				(data->path_prev)[j] = current_region;
				if(stack_region_max >= data->num_regions) error("too stacky!");
				(data->path_stack)[stack_region_max] = j;
				stack_region_max++;
			}
			else
			{
				dist = dist+(data->path_dist)[current_region];
				if(dist < (data->path_dist)[j])
				{
					(data->path_prev)[j] = current_region;
					(data->path_dist)[j] = dist;
				}
			}
		}
		stack_region++;
	}
	path_num_regions = 1;
	current_region = start_region;
	while(current_region != target_region)
	{
		current_region = (data->path_prev)[current_region];
		path_num_regions++;
	}
	path_num_regions++;
	walk = smartwalk_create();
	walk->num_nodes = path_num_regions;
	walk->nodes = (VECTOR*)sys_malloc(sizeof(VECTOR)*path_num_regions);
	vec_set((walk->nodes)[0],vstart);
	current_region = start_region;
	path_num_regions = 1;
	while(current_region != target_region)
	{
		previous_region = current_region;
		current_region = (data->path_prev)[current_region];
		line_projection((walk->nodes)[path_num_regions],(walk->nodes)[path_num_regions-1],(data->edgeMin)[previous_region*data->num_regions+current_region],(data->edgeMax)[previous_region*data->num_regions+current_region]);
		path_num_regions++;
	}
	vec_set((walk->nodes)[path_num_regions],vtarget);
	line_projection((walk->nodes)[path_num_regions-1],(walk->nodes)[path_num_regions],(data->edgeMin)[target_region*data->num_regions+previous_region],(data->edgeMax)[target_region*data->num_regions+previous_region]);
	walk->current_node = 1;

	return walk;
}

var vec_limit(VECTOR* vec, var range)
{
	var norm;

	norm = vec_length(vec);
	if(norm > range) vec_normalize(vec,range);
	norm = minv(norm,range);

	return norm;
}

void draw_line3dfull(VECTOR* vfrom, VECTOR* vto, COLOR* color, var alpha)
{
	draw_line3d(vfrom,NULL,alpha);
	draw_line3d(vfrom,color,alpha);
	draw_line3d(vto,color,alpha);
}

void smartwalk_draw(SMARTWALK* walk, COLOR* color, var alpha)
{
	int i;

	for(i = 0; i < walk->num_nodes-1; i++) draw_line3dfull((walk->nodes)[i],(walk->nodes)[i+1],color,alpha);
}

void smartwalk_level_load_event()
{
	smartwalkdata_fill(smd_level);
}

void smartwalk_on_ent_remove_event(ENTITY* ent)
{
	if(ent.ent_smartwalk) smartwalk_destroy(pSMARTWALK(ent.ent_smartwalk));
}

void smartwalk_init()
{
	int i;
	
	for(i = 0; i < SMWALK_CONDITION_MAX; i++) smwalk_condition[i] = 0;
	
	smd_level = smartwalkdata_create();
	on_level_load = smartwalk_level_load_event;
	on_ent_remove = smartwalk_on_ent_remove_event;
}

ENTITY* ent_flashlight = NULL;

//flag3: office 0
action actCondition()
{
	set(my,INVISIBLE | PASSABLE | UNLIT);
	while(1)
	{
		if(is(my,FLAG3))
		{
			if(ent_flashlight) smwalk_condition[0] = 1;
			else smwalk_condition[0] = 0;
			DEBUG_VAR(smwalk_condition[0],120);
		}
		wait(1);
	}
}