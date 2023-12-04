#ifndef __SPRSHEET_H
#define __SPRSHEET_H

#define strncmp(a,b,c)  ( substr(a,0,c) != b )

type __SPRSHEET_SPRITE_DATA
    G_RECT      clip;
    G_POINTD    center;
    int         cpoint_count;
    G_POINTD    * cpoints;
end

type __SPRSHEET_LIB
    int graph;
    int sprites_count;
    __SPRSHEET_SPRITE_DATA * sprites;
end

local
    int __sprsheet_sprite_id = 0;
    __SPRSHEET_LIB * __sprsheet_lib = NULL;
end

function sprsheet_get_cpoint( int number, G_POINTD * cp, int proc_id = 0 )
begin
    G_POINTD ret = POINT_UNDEFINED, POINT_UNDEFINED;
    *cp = ret;

    if ( !proc_id )
        proc_id = father;
    end
    if ( proc_id.__sprsheet_lib && number >= 0 &&
         proc_id.__sprsheet_sprite_id > 0 &&
         proc_id.__sprsheet_sprite_id <= proc_id.__sprsheet_lib.sprites_count &&
         proc_id.__sprsheet_lib.sprites[ proc_id.__sprsheet_sprite_id - 1 ].cpoint_count > number
         )
        *cp = proc_id.__sprsheet_lib.sprites[ proc_id.__sprsheet_sprite_id - 1 ].cpoints[ number ];
    end
end

function int sprsheet_set_sprite( __SPRSHEET_LIB * lib, int gid, int proc_id = 0 )
begin
    if ( lib && gid > 0 && lib.sprites_count >= gid )
        if ( !proc_id )
            proc_id = father;
        end
        proc_id.graph = lib.graph;
        proc_id.clip = lib.sprites[ gid - 1 ].clip;
        proc_id.center = lib.sprites[ gid - 1 ].center;
        proc_id.__sprsheet_lib = lib;
        proc_id.__sprsheet_sprite_id = gid;
    end
end

function sprsheet_unload( __SPRSHEET_LIB * lib )
begin
    if ( lib )
        for (int i = 0; i < lib.sprites_count; ++i)
            free(lib.sprites[i].cpoints);
        end
        free(lib.sprites);
        map_unload(0, lib.graph);
        free(lib);
    end
end

function __SPRSHEET_LIB * sprsheet_load(string filename)
begin
    int map = map_load( filename );
    if ( !map )
        return NULL;
    end

    string folder;
    string line;

    int sep_pos = rfind( filename, "/" );
    if ( sep_pos == -1 )
        sep_pos = rfind( filename, "\\" );
    end

    if ( sep_pos > -1 )
        folder = substr( filename, 0, sep_pos + 1 );
        filename = substr( filename, sep_pos + 1 );
    end

    string name = filename;
    sep_pos = rfind( filename, "." );
    if ( sep_pos > -1 )
        name = substr( filename, 0, sep_pos );
    end

    __SPRSHEET_LIB * lib = calloc( 1, sizeof( __SPRSHEET_LIB ));
    if ( !lib )
        map_unload( 0, map );
        return NULL;
    end

    lib.graph = map;

    int fp = fopen( folder + name + ".spr", O_READ );
    if ( fp )
        int spriteID = -1;
        while ( !feof( fp ) )
            line = fgets( fp );

            int comments = find( line, ";" );
            if ( comments > -1 )
                line = substr( line, 0, comments );
            end
            string keyword = strtok( lcase( line ), "=" );
            if ( keyword != "" )
                string value = strtok( "", "=" );
                switch ( keyword )
                    case "sprites":
                        int sprites_count = atoi(value);
                        if ( sprites_count <= 0 )
                            free(lib);
                            map_unload( 0, map );
                            return NULL;
                        end
                        lib.sprites = (__SPRSHEET_SPRITE_DATA *) calloc(sprites_count, sizeof(__SPRSHEET_SPRITE_DATA));
                        if (!lib.sprites)
                            sprsheet_unload(lib);
                            return NULL;
                        end
                        lib.sprites_count = sprites_count;
                        continue;
                    end
                end

                if (lib.sprites)
                    if ( line[0] == '[' )
                        spriteID = atoi(substr(line,1)) - 1;
                        if ( spriteID < 0 || spriteID > lib.sprites_count )
                            sprsheet_unload(lib);
                            return NULL;
                        end

                        // Inicializar los datos del sprite
                        memset(&lib.sprites[spriteID], '\0', sizeof(__SPRSHEET_SPRITE_DATA));
                        lib.sprites[spriteID].center.x = POINT_UNDEFINED;
                        lib.sprites[spriteID].center.y = POINT_UNDEFINED;
                        continue;
                    end

                    switch ( keyword )
                        // Procesar los datos del sprite
                        case "clip":
                            lib.sprites[spriteID].clip.x = atoi(strtok(value, ","));
                            lib.sprites[spriteID].clip.y = atoi(strtok("", ","));
                            lib.sprites[spriteID].clip.w = atoi(strtok("", ","));
                            lib.sprites[spriteID].clip.h = atoi(strtok("", ","));
                        end
                        case "center":
                            lib.sprites[spriteID].center.x = atoi(strtok(value, ","));
                            lib.sprites[spriteID].center.y = atoi(strtok("", ","));
                        end
                        case "cpoints":
                            int cpoints = atoi(value);
                            if (cpoints >= 0)
                                lib.sprites[spriteID].cpoint_count = cpoints;
                                lib.sprites[spriteID].cpoints = (G_POINTD *)calloc(cpoints, sizeof(G_POINTD));
                                if (!lib.sprites[spriteID].cpoints)
                                    sprsheet_unload(lib);
                                    return NULL;
                                end
                            end
                        end
                    end
                    if ( !strncmp( keyword, "cpoint[", 7 ) )
                        int cpointID = atoi(substr(keyword,7));
                        if ( cpointID < 0 || cpointID > lib.sprites[spriteID].cpoint_count)
                            sprsheet_unload(lib);
                            return NULL;
                        end
                        lib.sprites[spriteID].cpoints[cpointID].x = atoi(strtok(value, ","));
                        lib.sprites[spriteID].cpoints[cpointID].y = atoi(strtok("", ","));
                    end
                end
            end
        end
        fclose( fp );
    end
    return lib;
end

#ifdef SPRSHEET_TEST
int main()
begin
    set_mode(320,240);

    __SPRSHEET_LIB *lib = sprsheet_load("buttons.png");
    if (lib == NULL)
        say("Error!!!");
        return -1;
    end

    // Show data
    say("INFO:");
    for (int i = 0; i < lib.sprites_count; ++i)
        say("Sprite: " + ( i + 1) );
        say("Clip: " + lib.sprites[i].clip.x + "," + lib.sprites[i].clip.y + "," + lib.sprites[i].clip.w + "," + lib.sprites[i].clip.h);
        say("Center: " + lib.sprites[i].center.x + "," + lib.sprites[i].center.y);
        say("CPoints count: " + lib.sprites[i].cpoint_count);
        for (int j = 0; j < lib.sprites[i].cpoint_count; ++j)
            say("CPoint[" + j + "]: " +  lib.sprites[i].cpoints[j].x + "," + lib.sprites[i].cpoints[j].y);
        end
        say("");
    end

    sprsheet_unload( lib );

    return 0;
end
#endif

#endif
