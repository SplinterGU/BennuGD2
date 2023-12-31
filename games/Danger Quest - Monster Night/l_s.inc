PROCESS load_level(string sorc);
BEGIN
	load(sorc,grid);		

END

PROCESS save_level(string dest);
BEGIN
	save(dest,grid);

END

PROCESS save_prof();
BEGIN
	profile[0].lang=lang;
	profile[0].vol=volume_level;
	save("files/prof.dat",profile);
END

PROCESS load_prof();
BEGIN
	load("files/prof.dat",profile);
	lang=profile[0].lang;
	volume_level=profile[0].vol;
END

FUNCTION string interp_diff(a);
PRIVATE
	string answer;
BEGIN
	SWITCH(a)
		CASE 1:
			answer="001";
		END
		CASE 2:
			answer="002";
		END
		CASE 3:
			answer="003";
		END
		CASE 4:
			answer="004";
		END
		CASE 5:
			answer="005";
		END
		CASE 6:
			answer="006";
		END
		CASE 7:
			answer="007";
		END
		CASE 8:
			answer="008";
		END
		CASE 9:
			answer="009";
		END
		CASE 10:
			answer="010";
		END
	END
	RETURN(answer);
END






PROCESS load_sound();
BEGIN     
	
    suc_song=sound_load("files/success.mp3");
    fal_song=sound_load("files/gameover.mp3");
    win_song=sound_load("files/winner.mp3");

    s_click=sound_load("files/click.mp3");
    s_click2=sound_load("files/click2.mp3");
    s_dang=sound_load("files/danger2.mp3");
    s_dead=sound_load("files/dead.mp3");
    s_coin=sound_load("files/coin.mp3");
    s_sonar=sound_load("files/sonar.mp3");
    s_prison=sound_load("files/prison.mp3");
    s_multi=sound_load("files/multi.mp3");
    s_open=sound_load("files/opendoor.mp3");
    s_fail=sound_load("files/fail.mp3");   
   // set_all_wave_volume(10*s_fx_level);
    
    
END



FUNCTION STRING get_diff();
PRIVATE
	string answer;
BEGIN
	SWITCH(diff)
		CASE 1: answer="001"; END
		CASE 2: answer="002"; END
		CASE 3: answer="003"; END
		CASE 4: answer="004"; END
		CASE 5: answer="005"; END
		CASE 6: answer="006"; END
		CASE 7: answer="007"; END
		CASE 8: answer="008"; END
		CASE 9: answer="009"; END
		CASE 10: answer="010"; END
	END

	RETURN(answer);
END


FUNCTION STRING get_iter();
PRIVATE
	string answer;
BEGIN
	SWITCH(iter)
		CASE 1: answer="01"; END
		CASE 2: answer="02"; END
		CASE 3: answer="03"; END
		CASE 4: answer="04"; END
		CASE 5: answer="05"; END
		CASE 6: answer="06"; END
		CASE 7: answer="07"; END
		CASE 8: answer="08"; END
		CASE 9: answer="09"; END
		CASE 10: answer="10"; END
	END

	RETURN(answer);
END
