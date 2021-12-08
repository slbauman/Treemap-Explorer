// stm.h
// 
// Samuel Bauman 2021

std::vector<int> layout_row(
		int items_sum,
		std::vector<int> row,
		int bounds[4]);

float worst(
		int item,
		int items_sum,
		int row_sum,
		int bounds[4]);

std::vector<int> squarify(
		std::vector<int> items,
		std::vector<int> row,
		int bounds[4],
		std::vector<int> rects = {});

