
#include "FailoverQueue.hpp"

/*
** Copyright (c) 2010-2011 Blizzard Entertainment
** 
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
*/

#include <boost/shared_ptr.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <iostream>
#include <fstream>
#include <string>

#define TEST_PATH "./"

class Basic {
public:
	Basic() { }
	~Basic() { }
private:
	friend class boost::serialization::access;

	int age_;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int /* version */) {
		ar & age_;
	}
};

typedef boost::shared_ptr<Basic> BasicPtr;

void reset() {
	if (!boost::filesystem::is_directory(TEST_PATH)) {
		return;
	}
	boost::filesystem::directory_iterator end_iter;
	for (boost::filesystem::directory_iterator dir_itr(TEST_PATH); dir_itr != end_iter; ++dir_itr) {
		if (boost::filesystem::is_regular_file(dir_itr->status())) {
			std::string fileName = dir_itr->path().filename();
			if (fileName.find("failover") == 0) {
				boost::filesystem::remove(dir_itr->path().filename());
			}
		}
	}
}

int main() {
	reset();

	FailoverQueue<Basic, BasicPtr> basicFQ(TEST_PATH, 25);
	assert(basicFQ.size() == 0);

	BasicPtr bptr1(new Basic());

	basicFQ.push(bptr1);
	assert(basicFQ.size() == 1);

	basicFQ.clear();
	assert(basicFQ.size() == 0);

	return 0;
}
