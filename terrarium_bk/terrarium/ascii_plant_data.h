/******************************************************************************
 * ascii_plant_data.h
 *
 * Exposes function(s) to retrieve the 10-line ASCII art for each of the 40 plants.
 ******************************************************************************/

#ifndef ASCII_PLANT_DATA_H
#define ASCII_PLANT_DATA_H

/* Returns the line-th line (0..9) of ASCII art for plant index (0..39). */
const char* getPlantArt(int plantIndex, int line);

#endif

