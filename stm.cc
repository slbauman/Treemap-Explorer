#include <numeric>
#include <vector>
#include <math.h>
#include "stm.h"

using namespace std;

vector<int> layout_row(int items_sum, vector<int> row, 
		int bounds[4]) 
{
	vector<int> result;
	int pos = 0;
	int row_sum = accumulate(row.begin(), row.end(), 0);
	bool vertical = bounds[2] > bounds[3];
	float row_width = (float)row_sum / (float)(row_sum + items_sum) *
	(float)(vertical ? bounds[2] : bounds[3]);
	for (int i = 0; i < row.size(); i ++) {
		vector<int> rect;
		if (vertical) {
			rect = { bounds[0], bounds[1] + pos, rint(row_width),
				rint(((float)row.at(i) / (float)row_sum) * (float)(bounds[3] - pos))
			};
		} else {
			rect = { bounds[0] + pos, bounds[1], 
				rint(((float)row.at(i) / (float)row_sum) * (float)(bounds[2] - pos)),
				rint(row_width)
			};
		}
		row_sum -= row.at(i);
		result.insert( result.end(), rect.begin(), rect.end());
		pos += (vertical ? rect.at(3) : rect.at(2));
	}
	return result;
}

float worst(int item, int items_sum, int row_sum, int bounds[4]) {
	bool vertical = bounds[2] > bounds[3];
	row_sum += item;
	float w = ((float)row_sum / (float)(row_sum + items_sum)) * 
		(float)(vertical ? bounds[2] : bounds[3]);
	float h = ((float)item / (float)row_sum) * 
		(float)(vertical ? bounds[3] : bounds[2]);
	return fabs((w / h) - 1.0);
}

vector<int> squarify(vector<int> items, vector<int> row, 
		int bounds[4], vector<int> rects) {
	
	int item, items_sum, row_sum;
	items_sum = accumulate(items.begin(), items.end(), 0);
	row_sum = accumulate(row.begin(), row.end(), 0);
	item = items.front();
	items.erase(items.begin());
	
	bool vertical = bounds[2] > bounds[3];
	
	int shrink = rint(((float)row_sum / (float)(row_sum + items_sum)) *
		(float)(vertical ? bounds[2] : bounds[3]));
	
	int next_bounds[4] = {bounds[0], bounds[1], bounds[2], bounds[3]};
	
	if (vertical) {
		next_bounds[0] += shrink;
		next_bounds[2] -= shrink;
	} else {
		next_bounds[1] += shrink;
		next_bounds[3] -= shrink;
	}

	float existing_row = worst(item, items_sum, row_sum, bounds);
	float next_row = worst(item, items_sum, 0, next_bounds);
	bool add_to_next_row = existing_row > next_row;

	if (add_to_next_row) {
		vector<int> new_rects = layout_row(items_sum, row, bounds);
		rects.insert( rects.end(), new_rects.begin(), new_rects.end() );
		row.clear();
	}

	row.push_back(item);
	
	if (items.size() > 0) {
		return squarify(items, row, add_to_next_row ? next_bounds : bounds, rects);
	} else {
		vector<int> new_rects = layout_row(0, row, 
				row.size() == 1 ? next_bounds : bounds);
		rects.insert( rects.end(), new_rects.begin(), new_rects.end() );
		return rects;
	}
}

