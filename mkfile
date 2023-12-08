</$objtype/mkfile

CFLAGS=-Fpw -I/sys/include/npe -I/sys/include/npe/SDL2 -D__plan9__ -D__${objtype}__
BIN=/$objtype/bin/games
TARG=cstory

OFILES=\
	src/ArmsItem.$O\
	src/Back.$O\
	src/Bitmap.$O\
	src/Boss.$O\
	src/BossAlmo1.$O\
	src/BossAlmo2.$O\
	src/BossBallos.$O\
	src/BossFrog.$O\
	src/BossIronH.$O\
	src/BossLife.$O\
	src/BossOhm.$O\
	src/BossPress.$O\
	src/BossTwinD.$O\
	src/BossX.$O\
	src/BulHit.$O\
	src/Bullet.$O\
	src/Caret.$O\
	src/Config.$O\
	src/Draw.$O\
	src/Ending.$O\
	src/Escape.$O\
	src/Fade.$O\
	src/File.$O\
	src/Flags.$O\
	src/Flash.$O\
	src/Font.$O\
	src/Frame.$O\
	src/Game.$O\
	src/Generic.$O\
	src/GenericLoad.$O\
	src/Input.$O\
	src/KeyControl.$O\
	src/Main.$O\
	src/Map.$O\
	src/MapName.$O\
	src/MiniMap.$O\
	src/MyChar.$O\
	src/MycHit.$O\
	src/MycParam.$O\
	src/NpcAct000.$O\
	src/NpcAct020.$O\
	src/NpcAct040.$O\
	src/NpcAct060.$O\
	src/NpcAct080.$O\
	src/NpcAct100.$O\
	src/NpcAct120.$O\
	src/NpcAct140.$O\
	src/NpcAct160.$O\
	src/NpcAct180.$O\
	src/NpcAct200.$O\
	src/NpcAct220.$O\
	src/NpcAct240.$O\
	src/NpcAct260.$O\
	src/NpcAct280.$O\
	src/NpcAct300.$O\
	src/NpcAct320.$O\
	src/NpcAct340.$O\
	src/NpChar.$O\
	src/NpcHit.$O\
	src/NpcTbl.$O\
	src/Organya.$O\
	src/PixTone.$O\
	src/Profile.$O\
	src/Random.$O\
	src/Resource.$O\
	src/SelStage.$O\
	src/Shoot.$O\
	src/Sound.$O\
	src/Stage.$O\
	src/Star.$O\
	src/TextScr.$O\
	src/Triangle.$O\
	src/ValueView.$O\
	src/Backends/Platform/9front.$O\
	src/Backends/Audio/9front.$O\
	src/Backends/Controller/9front.$O\
	src/Backends/Rendering/Software.$O\
	src/Backends/Rendering/Window/Software/9front.$O\

ASSETS=\
	src/Resource/BITMAP/Credit01.bmp.h\
	src/Resource/BITMAP/Credit02.bmp.h\
	src/Resource/BITMAP/Credit03.bmp.h\
	src/Resource/BITMAP/Credit04.bmp.h\
	src/Resource/BITMAP/Credit05.bmp.h\
	src/Resource/BITMAP/Credit06.bmp.h\
	src/Resource/BITMAP/Credit07.bmp.h\
	src/Resource/BITMAP/Credit08.bmp.h\
	src/Resource/BITMAP/Credit09.bmp.h\
	src/Resource/BITMAP/Credit10.bmp.h\
	src/Resource/BITMAP/Credit11.bmp.h\
	src/Resource/BITMAP/Credit12.bmp.h\
	src/Resource/BITMAP/Credit14.bmp.h\
	src/Resource/BITMAP/Credit15.bmp.h\
	src/Resource/BITMAP/Credit16.bmp.h\
	src/Resource/BITMAP/Credit17.bmp.h\
	src/Resource/BITMAP/Credit18.bmp.h\
	src/Resource/BITMAP/pixel.bmp.h\
	src/Resource/BITMAP/pixel_jp.bmp.h\
	src/Resource/CURSOR/CURSOR_IKA.png.h\
	src/Resource/CURSOR/CURSOR_NORMAL.png.h\
	src/Resource/ICON/GLFW_ICON.ico.h\
	src/Resource/ICON/ICON_MINI.png.h\
	src/Resource/ORG/Access.org.h\
	src/Resource/ORG/Anzen.org.h\
	src/Resource/ORG/Balcony.org.h\
	src/Resource/ORG/Ballos.org.h\
	src/Resource/ORG/BreakDown.org.h\
	src/Resource/ORG/Cemetery.org.h\
	src/Resource/ORG/Curly.org.h\
	src/Resource/ORG/Dr.org.h\
	src/Resource/ORG/Ending.org.h\
	src/Resource/ORG/Escape.org.h\
	src/Resource/ORG/Fanfale1.org.h\
	src/Resource/ORG/Fanfale2.org.h\
	src/Resource/ORG/Fanfale3.org.h\
	src/Resource/ORG/FireEye.org.h\
	src/Resource/ORG/Gameover.org.h\
	src/Resource/ORG/Ginsuke.org.h\
	src/Resource/ORG/Grand.org.h\
	src/Resource/ORG/Gravity.org.h\
	src/Resource/ORG/Hell.org.h\
	src/Resource/ORG/Jenka.org.h\
	src/Resource/ORG/Jenka2.org.h\
	src/Resource/ORG/Kodou.org.h\
	src/Resource/ORG/LastBtl.org.h\
	src/Resource/ORG/LastBtl3.org.h\
	src/Resource/ORG/LastCave.org.h\
	src/Resource/ORG/MDown2.org.h\
	src/Resource/ORG/Marine.org.h\
	src/Resource/ORG/Maze.org.h\
	src/Resource/ORG/Mura.org.h\
	src/Resource/ORG/Oside.org.h\
	src/Resource/ORG/Plant.org.h\
	src/Resource/ORG/Requiem.org.h\
	src/Resource/ORG/Toroko.org.h\
	src/Resource/ORG/Vivi.org.h\
	src/Resource/ORG/Wanpak2.org.h\
	src/Resource/ORG/Wanpaku.org.h\
	src/Resource/ORG/Weed.org.h\
	src/Resource/ORG/White.org.h\
	src/Resource/ORG/XXXX.org.h\
	src/Resource/ORG/Zonbie.org.h\
	src/Resource/ORG/ironH.org.h\
	src/Resource/ORG/quiet.org.h\
	src/Resource/WAVE/Wave.dat.h\

CLEANFILES=\
	$OFILES\
	$O.bin2h\
	bin2h/bin2h.$O\
	$ASSETS\

</sys/src/cmd/mkone

%.$O:	%.cpp
	$CC $CFLAGS -o $stem.$O $stem.cpp

%.$O:	%.c
	$CC $CFLAGS -o $stem.$O $stem.c

$O.bin2h:	bin2h/bin2h.$O
	$LD $LDFLAGS -o $target $prereq

src/Resource/%.h:	$O.bin2h assets/resources/$stem
	$O.bin2h assets/resources/$stem $target

src/Resource.$O:	$ASSETS
