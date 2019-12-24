#include "File.h"

File::File(string br, string filename)
	: broker(br),
	account(filename.substr(8, 8)),
	count(1),
	last_date(filename.substr(17, 8)) {}

ostream& operator<<(ostream& out, const File& file) {
	out << "broker:" << file.broker << " account:" << file.account
		<< " files:" << file.count << " lastdate:" << file.last_date;
	return out;
}
