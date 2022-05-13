#pragma once

#include <vector>
#include <string>
typedef unsigned int uint;

class HashSet{
public:	
	HashSet(uint max_rank, bool verbose = false);

	bool contains(uint item);
	void remove(uint item);
	void insert(uint item);
	void layout(uint rank);

	void printLayout(uint rank = 0);

private:
	class Bucket{
	public:
		Bucket(uint key);
		bool contains(uint item, HashSet* set);
		void remove(uint item);
		Bucket* insert(uint item);
		std::string toString() const;
		
		uint key;
		uint orig_item;
		Bucket* next;
	private:
		Bucket* safeInsertNext(uint key);
	};

	std::vector<std::vector<uint>> copyBuckets(uint rank);

	static uint markAsSentinal(uint number);
	static uint unmarkSentinal(uint number);
	static bool isSentinal(uint key);
	static uint reverseBits(uint num);

	Bucket* getContainingBucket(uint item, uint rank);

	const uint max_rank;
	const uint table_size;
	Bucket** buckets;
	bool verbose;
};