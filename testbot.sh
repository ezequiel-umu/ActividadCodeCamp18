#!/usr/bin/env bash
ants/linux/playgame.py -So --player_seed $RANDOM --end_wait=0.5 --verbose --log_dir game_logs --turns 1000 --map_file ants/linux/maps/random_walk/random_walk_p02_19.map "$@" \
	"sh framework/C++/bot.sh exec " \
	"sh judge/bots/level4-bot/bot.sh exec testbot" \
  | # test
java -jar ants/linux/visualizer.jar
