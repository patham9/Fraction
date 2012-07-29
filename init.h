void init()  
{			
	uint i,j;
	hfio_LoadTex("forest.tga",&FOREST);									//load forest texture
	hfio_LoadTex("house.tga",&CITY);									//load city texture
	hfio_LoadTex("rock.tga",&ROCK);										//load rock texture
	hfio_LoadTex("water.tga",&WATER);									//load water texture
	hfio_LoadTex("street.tga",&STREET);									//load street texture
	hfio_LoadTex("man.tga",&MEN);										//load forest texture
	hfio_LoadTex("grass.tga",&GRASS);									//load grass texture
	hfio_LoadTex("water_grass_right.tga",&GRASS_L);						//load grass texture
	hfio_LoadTex("water_grass_down.tga",&GRASS_T);						//load grass texture
	hfio_LoadTex("water_grass_left.tga",&GRASS_R);						//load grass texture
	hfio_LoadTex("water_grass_top.tga",&GRASS_D);						//load grass texture
	hfio_LoadTex("water_grass_left_down.tga",&GRASS_RT);				//load grass texture
	hfio_LoadTex("water_grass_left_top.tga",&GRASS_RD);					//load grass texture
	hfio_LoadTex("water_grass_right_down.tga",&GRASS_LT);				//load grass texture
	hfio_LoadTex("water_grass_right_top.tga",&GRASS_LD);				//load grass texture
	hfio_LoadTex("water_grass_around.tga",&GRASS_A);					//load grass texture
#if RENDERMODE==2 || RENDERMODE==3
#if RENDERMODE==2
	shader=hshade_CreateShaderPair("vertexshader","pixelshader");  		//load pixel (and vertex) shader	
#endif	
#if RENDERMODE==3
	shader=hshade_CreateShaderPair("vertexshader3d","pixelshader3d");  	//load pixel (and vertex) shader	
#endif				
	hfio_LoadTex("grass.tga",&GPUTex);									//ehm
	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "data"), 0);
	glUniform1i(glGetUniformLocation(shader, "grass_texture"), 1);
	glUniform1i(glGetUniformLocation(shader, "city_texture"), 2);
	glUniform1i(glGetUniformLocation(shader, "forest_texture"), 3);
	glUniform1i(glGetUniformLocation(shader, "rock_texture"), 4);
	glUniform1i(glGetUniformLocation(shader, "street_texture"), 5);
	glUniform1i(glGetUniformLocation(shader, "water_texture"), 6);
	shader_t=glGetUniformLocation(shader, "t");							//time
	shader_px=glGetUniformLocation(shader, "px");
	shader_py=glGetUniformLocation(shader, "py");
	shader_zoom=glGetUniformLocation(shader, "zoom");
	glUniform1f(glGetUniformLocation(shader, "ROCK"),1.0/(float)ROCK);				//make GPU know of our state constants
	glUniform1f(glGetUniformLocation(shader, "FOREST"),1.0/(float)FOREST);			
	glUniform1f(glGetUniformLocation(shader, "CITY"),1.0/(float)CITY);
	glUniform1f(glGetUniformLocation(shader, "WATER"),1.0/(float)WATER);
	glUniform1f(glGetUniformLocation(shader, "GRASS"),1.0/(float)GRASS);
	glUniform1f(glGetUniformLocation(shader, "STREET"),1.0/(float)STREET);
	glUniform1i(glGetUniformLocation(shader, "worldsize"),worldsize);	
#endif
	men=(Men**)malloc(maxmen*sizeof(Men*));
	toGPU=(float*)malloc(worldsize*worldsize*4*sizeof(float));	
	hnav_SetRendFunc(draw);												//set hamlib render routine
	hrend_2Dmode(0.5,0.6,0.5);											//set hamlib render mode to 2D
	hinput_AddMouseDown(mouse_down);									//add mouse down event
	hinput_AddMouseDragged(mouse_down);									//add mouse dragged event
	hinput_AddKeyUp(key_up);											//add key up event
	type=GRASS;															//set default object to place on click to grass
	srand(1);															//random generator seed. use current time and world will be different on every execution
	landscape=GeneratePerlinNoise(worldsize,worldsize,WhiteNoise(worldsize,worldsize),8,2);	//generate landscape
	automat=Hauto_OBJ_NEW(10,worldsize,Simulate,Cell_NEW);				//create cellular automat object
	GenerateNature();													//generate the nature on it
	thread = Thread_NEW(Automat_Thread,NULL);									//create a thread for the automat where it can execute on its own core then
}
