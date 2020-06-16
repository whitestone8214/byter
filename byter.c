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

/* gcc -I. $(pkg-config --cflags --libs cell) byter.c -shared -o libbyter.so */


#include <byter.h>


// Internal use only
int byter_10(int nTimes);


byte byter_check_empty(char *string) {
	if (string == NULL) return 1;
	if (strcmp(string, "") == 0) return 1;
	
	return 0;
}
char *byter_read_number(int number) {
	// Absolute number
	int _number = number;
	if (_number < 0) _number = 0 - _number;
	
	// Digits
	int _nBytes = 0;
	if (number < 0) _nBytes++; // Minus
	int _nPow = 0;
	while (_number >= byter_10(_nPow)) {_nBytes++; _nPow++;}
	
	// Rooms
	char _bytes[_nBytes + 1];
	memset(&_bytes[0], 0, _nBytes + 1);
	if (number < 0) _bytes[0] = '-';
	
	// Fill the rooms
	int _nth = _nBytes - 1;
	while (_number >= 10) {
		_bytes[_nth] = 0x30 + (_number % 10);
		_number /= 10;
		_nth--;
	}
	_bytes[_nth] = 0x30 + _number;
	
	return strdup(&_bytes[0]);
}
char *byter_connect_strings(int nStrings, ...) {
	// Start tracking arguments
	va_list _stream;
	va_start(_stream, nStrings);
	
	// Connect Strings!
	char *_stringThis = va_arg(_stream, char *);
	char *_string = NULL;
	int _counter = 0;
	while (_counter < nStrings) {
		// Breaker
		if (_stringThis == NULL) break;
		
		// Do it
		if (_string == NULL) _string = strdup(_stringThis);
		else {
			// Connect Strings! Re:Dive :-P
			char _stringConnected[strlen(_string) + strlen(_stringThis) + 1];
			_stringConnected[strlen(_string) + strlen(_stringThis)] = '\0';
			memcpy(&_stringConnected[0], &_string[0], strlen(_string));
			memcpy(&_stringConnected[strlen(_string)], &_stringThis[0], strlen(_stringThis));
			
			// Save state
			free(_string);
			_string = strdup(&_stringConnected[0]);
		}
		
		// Get closer to breaker
		_stringThis = va_arg(_stream, char *);
		_counter++;
	}
	
	// Done!
	va_end(_stream);
	return _string;
}
char *byter_cut_part(char *string, int from, int to) {
	// Sanity check 1
	if (byter_check_empty(string) == 1) return NULL;
	if ((from < 0) || (from >= strlen(string))) return NULL;
	
	// Sanity check 2
	int _to = to;
	if (_to == -1) _to = strlen(string) - 1;
	if ((_to < 0) || (to >= strlen(string)) || (to > from)) return NULL;
	if ((from == 0) && (_to == strlen(string) - 1)) return NULL;
	
	// Rooms
	char _part[strlen(string) - (_to - from)];
	memset(&_part[0], 0, strlen(string) - (_to - from));
	if (from >= 1) memcpy(&_part[0], &string[0], from);
	if (_to < strlen(string) - 1) memcpy(&_part[from], &string[_to + 1], strlen(string) - _to);
	
	// Done!
	return strdup(&_part[0]);
}
char *byter_take_part(char *string, int from, int to) {
	/*// Rooms
	char _part[(_to - from) + 2];
	memset(&_part[0], 0, (_to - from) + 2);
	memcpy(&_part[0], &string[from], (_to - from) + 1);
	
	// Done!
	return strdup(&_part[0]);*/
	
	// Sanity check
	if (byter_check_empty(string) == 1) return NULL;
	if ((from < 0) || (from >= strlen(string))) return NULL;
	
	// Set end point to end of string if it is -1
	int _to = to;
	if (_to == -1) _to = strlen(string) - 1;
	if ((_to < 0) || (to >= strlen(string)) || (from > _to)) return NULL;
	if ((from == 0) && (_to == strlen(string) - 1)) return strdup(string);
	
	// Copy the part
	char _substring[(_to - from) + 1 + 1];
	_substring[(_to - from) + 1] = '\0';
	memcpy(&_substring[0], &string[from], (_to - from) + 1);
	
	// Rooms
	char _part[(_to - from) + 2];
	memset(&_part[0], 0, (_to - from) + 2);
	memcpy(&_part[0], &string[from], (_to - from) + 1);
	
	// Done!
	return strdup(&_part[0]);
}
cell *byter_find_string(char *from, char *string) {
	// Sanity check
	if (byter_check_empty(from) == 1) return NULL;
	if (byter_check_empty(string) == 1) return NULL;
	if (strlen(from) < strlen(string)) return NULL;
	
	// Find it
	cell *_arraySpots = NULL;
	int _nth; for (_nth = 0; _nth < strlen(from); _nth++) {
		// It's too late if rest is shorter than string to find
		if (_nth + strlen(string) > strlen(from)) break;
		
		// This is not. Next!
		if (strncmp(&from[_nth], string, strlen(string)) != 0) continue;
		
		// Mark current position
		if (_arraySpots == NULL) {
			_arraySpots = cell_create(_nth);
			cell_insert(_arraySpots, -1, -1);
			_arraySpots = _arraySpots->next;
		}
		else {
			_arraySpots->data = _nth;
			cell_insert(_arraySpots, -1, -1);
			_arraySpots = _arraySpots->next;
		}
		
		// Next!
		_nth += strlen(string) - 1;
	}
	
	// Done!
	return cell_nth(_arraySpots, 0);
}
cell *byter_split_string(char *string, char *by, byte include_by) {
	// Sanity check
	if (byter_check_empty(string) == 1) return NULL;
	if (byter_check_empty(by) == 1) {
		cell *_arraySlices = cell_create((void *)strdup(string));
		cell_insert(_arraySlices, -1, NULL);
		return _arraySlices;
	}
	if (strcmp(string, by) == 0) {
		if (include_by != 1) return NULL;
		
		cell *_arraySlices = cell_create((void *)strdup(by));
		cell_insert(_arraySlices, -1, NULL);
		return _arraySlices;
	}
	if (strlen(string) <= strlen(by)) return NULL;
	
	// Find position(s)
	cell *_arraySpots = byter_find_string(string, by);
	if (_arraySpots == NULL) {
		cell *_arraySlices = cell_create((void *)strdup(string));
		cell_insert(_arraySlices, -1, NULL);
		return _arraySlices;
	}
	
	// Split and pack slice(s)
	cell *_arraySpot0 = _arraySpots;
	cell *_arraySlices = cell_create(NULL);
	if (_arraySlices == NULL) {cell_delete_all(_arraySpots, 0); return NULL;}
	int _nthInString = 0;
	while (_arraySpot0->data != -1) {
		// Last slice after last 'by'
		if (_arraySpot0->next == NULL) {
			if (_nthInString <= strlen(string) - 1) {
				_arraySlices->data = (void *)strndup(&string[_nthInString], strlen(string) - _nthInString);
				cell_insert(_arraySlices, -1, NULL);
				_arraySlices = _arraySlices->next;
			}
			break;
		}
		
		// Slice
		if (_arraySpot0->data != 0) {
			_arraySlices->data = (void *)strndup(&string[_nthInString], _arraySpot0->data - _nthInString);
			cell_insert(_arraySlices, -1, NULL);
			_arraySlices = _arraySlices->next;
		}
		if (include_by == 1) {
			_arraySlices->data = (void *)strdup(by);
			cell_insert(_arraySlices, -1, NULL);
			_arraySlices = _arraySlices->next;
		}
		
		// Next 'by'
		_nthInString = _arraySlices->data + strlen(by);
		_arraySpot0 = _arraySpot0->next;
	}
	
	// Done
	cell_delete_all(_arraySpots, 0);
	return cell_nth(_arraySlices, 0);
}
char *byter_replace_string(char *string, char *replace, char *with) {
	// Sanity check
	if (byter_check_empty(string) == 1) return NULL;
	if (byter_check_empty(replace) == 1) return strdup(string);
	if ((strcmp(string, replace) == 0) || (strcmp(string, with) == 0)) return strdup(with);
	if (strlen(string) <= strlen(replace)) return NULL;
	
	// Split string
	cell *_arraySlices = byter_split_string(string, replace, 1);
	if (_arraySlices == NULL) return NULL;
	
	// Replace 'replace' with 'with'
	cell *_arraySlice0 = _arraySlices;
	int _sizeTotal = 1;
	while (_arraySlices != NULL) {
		char *_slice = (char *)_arraySlice0->data;
		if (strcmp(_slice, replace) == 0) {
			free(_arraySlice0->data);
			_arraySlice0->data = NULL;
			if (byter_check_empty(with) != 1) {
				_arraySlice0->data = (void *)strdup(with);
				_sizeTotal += strlen(with);
			}
		}
		else _sizeTotal += strlen(_slice);
		
		if (_arraySlice0->next == NULL) break;
		_arraySlice0 = _arraySlice0->next;
	}
	
	// Connect slices
	char _string[_sizeTotal];
	memset(&_string[0], 0, _sizeTotal);
	_arraySlice0 = _arraySlices;
	int _nthInString = 0;
	while (_arraySlices != NULL) {
		// Copy slice
		if (_arraySlice0->data != NULL) {
			char *_slice = (char *)_arraySlice0->data;
			memcpy(&_string[_nthInString], &_slice[0], strlen(_slice));
			_nthInString += strlen(_slice);
		}
		
		// Next slice
		if (_arraySlice0->next == NULL) break;
		_arraySlice0 = _arraySlice0->next;
	}
	
	// Done
	cell_delete_all(_arraySlices, 1);
	return strdup(&_string[0]);
}

int byter_10(int nTimes) {
	if (nTimes == 0) return 1;
	if (nTimes < 0) return 0;
	
	int _number = 1;
	int _n = 0; while (_n < nTimes) {
		_number *= 10;
		_n++;
	}
	
	return _number;
}

