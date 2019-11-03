#!/bin/bash

#install script for submarine game

# -I tells gcc to look in dir for headers

gcc @src_files -Iheaders -osubmarine_game -lncurses -Wall -Wextra