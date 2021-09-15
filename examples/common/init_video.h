set_mode( SCRW, SCRH );

background.file = 0;
background.graph = map_load("../common/res/bubbles.jpg");
background.size = SCRW * 100.0 / map_info(0,background.graph,g_width);
