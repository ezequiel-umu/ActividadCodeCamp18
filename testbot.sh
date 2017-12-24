#!/usr/bin/env sh
ants/linux/playgame.py -So --player_seed 42 --end_wait=0.25 --verbose --log_dir game_logs --turns 1000 --map_file ants/linux/maps/cell_maze/cell_maze_p03_20.map "$@" \
	"python ants/linux/sample_bots/python/HunterBot.py" \
	"sh framework/C++/bot.sh exec" \
	"python ants/linux/sample_bots/python/LeftyBot.py" |
java -jar ants/linux/visualizer.jar
