/*
	Copyright (C) 2020 Minho Jo <whitestone8214@gmail.com>
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <cell.h>


#ifndef TURNED_ON_BYTER
#define TURNED_ON_BYTER

typedef unsigned char byte;


byte byter_check_empty(char *string);
char *byter_read_number(int number);
char *byter_connect_strings(int nStrings, ...);
char *byter_cut_part(char *string, int from, int to);
char *byter_take_part(char *string, int from, int to);
cell *byter_find_string(char *from, char *string);
cell *byter_split_string(char *string, char *by, byte include_by);
char *byter_replace_string(char *string, char *replace, char *with);

#endif
