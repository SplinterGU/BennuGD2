import "mod_misc";

process do_dir()
begin
    int dirh = diropen( "*" );
    if ( dirh )
        while( 1 )
            string fname = dirread( dirh );
            if ( fname == "." || fname == ".." ) continue; end
            if ( fname == "" ) break; end
            if ( fileinfo.directory )
                cd( fname );
                do_dir();
                cd( ".." );
            else
                int dot_pos = find(strrev(fname), ".");
                if ( dot_pos > -1 )
                    string ext = substr(fname, -dot_pos);
                    string basename = substr( fname, 0, strlen( fname ) - dot_pos );
                    switch ( lcase( ext ) )
                        case    "fpg", "fnt":
                            // Valid
                            say("process " + fname + " ...");
                            int fpr = fopen( fname, O_ZREAD);
                            if ( fpr )
                                int fsize = flength( fpr );
                                byte * buffer = mem_alloc( fsize );
                                if ( buffer )
                                    string fname_write = basename + "gz." + ext;
                                    int fpw = fopen( fname_write, O_ZWRITE);
                                    if ( fpw )
                                        if ( fread(buffer, fsize, fpr ) == fsize )
                                            if ( fwrite(buffer, fsize, fpw ) ==  fsize )
                                                say( "saving new " + fname );
                                                fclose( fpw );
                                                fmove( fname, basename + "bak." + ext );
                                                fmove( basename + "gz." + ext, fname );
                                            else
                                                fclose( fpw );
                                                say( "ERROR saving " + fname );
                                            end
                                        else
                                            say( "ERROR saving " + fname );
                                            fclose( fpw );
                                        end
                                    end
                                    mem_free( buffer );
                                end
                                fclose( fpr );
                            end
                        end
                    end
                end
            end
        end
        dirclose(dirh);
    end
end


process int main()
begin
    do_dir();
end
