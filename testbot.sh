#!/usr/bin/env bash
ants/linux/playgame.py -So --player_seed $RANDOM --end_wait=0.5 --verbose --log_dir game_logs --turns 1000 --map_file ants/linux/maps/cell_maze/cell_maze_p02_06.map "$@" \
	"sh framework/C++/bot.sh exec" \
	"sh framework/C++/bot.sh exec" \
  | # test
java -jar ants/linux/visualizer.jar
