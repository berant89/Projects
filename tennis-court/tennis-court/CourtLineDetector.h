#pragma once

class CourtLineDetector
{
public:
	CourtLineDetector(void);
	~CourtLineDetector(void);
	int run();
private:
	void cld_setup();
	void thresh_canny_line();
	void name_lines();
	void draw_axes();
	void load_raw();
	void write_raw();
};

