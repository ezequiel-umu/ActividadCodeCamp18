#!/usr/bin/env sh
ants/linux/playgame.py -So --player_seed 42 --end_wait=0.25 --verbose --log_dir game_logs --turns 1000 --map_file ants/linux/maps/cell_maze/cell_maze_p03_20.map "$@" \
	"sh judge/bots/easy-bot/bot.sh exec" \
	"sh judge/bots/medium-bot/bot.sh exec" \
	"sh framework/C++/bot.sh exec" |
java -jar ants/linux/visualizer.jar
