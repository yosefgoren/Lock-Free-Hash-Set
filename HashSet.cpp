#include "HashSet.h"
#include <iostream>
#include <exception>
using namespace std;

// template<class Value>
// void CAS(bool condition, Value* target, Value new_value){
// 	if(condition)
// 		* target = new_value;
// }

HashSet::HashSet(uint max_rank, bool verbose)
		:max_rank(max_rank), table_size(1<<max_rank), verbose(verbose){
	buckets = new Bucket*[table_size];
	for(int i = 0; i < table_size; ++i)
		buckets[i] = nullptr;
	buckets[0] = new Bucket(reverseBits(markAsSentinal(0)));
}

bool HashSet::contains(uint item){
	Bucket* bucket = getContainingBucket(item, max_rank);
	return bucket->contains(item, this);
}
void HashSet::remove(uint item){
	getContainingBucket(item, max_rank)->remove(item);
	if(verbose)
		printLayout();
}
void HashSet::insert(uint item){
	getContainingBucket(item, max_rank)->insert(item);
	if(verbose)
		printLayout();
}

void HashSet::layout(uint rank){
	printLayout(rank);
}

vector<vector<uint>> HashSet::copyBuckets(uint rank){
	if(rank == 0){
		vector<uint> all_items;
		for(Bucket* cur = buckets[0]; cur != nullptr; cur = cur->next)
			all_items.push_back(cur->key);
		return {all_items}; 
	}
	vector<vector<uint>> super_buckets = copyBuckets(rank-1);
	vector<vector<uint>> buckets;
	for(vector<uint> super_bucket : super_buckets){
		if(super_bucket.size() == 0){
			buckets.push_back(vector<uint>());
			buckets.push_back(vector<uint>());
		}else{
			uint first_half_idx = unmarkSentinal(reverseBits( super_bucket[0]));
			uint second_half_idx = first_half_idx+(1<<(rank-1));
			uint second_half_sentinal_key = reverseBits(markAsSentinal(second_half_idx));
			vector<uint> first_bucket, second_bucket;
			for(uint key: super_bucket){
				(key < second_half_sentinal_key ? first_bucket : second_bucket).push_back(key);
			}
			buckets.push_back(first_bucket);
			buckets.push_back(second_bucket);
		}
	}
	return buckets;
}

string getBinaryRepresentation(uint number, uint rank){
	string res;
	while(number > 0){
		res = (number%2 ? "1" : "0")+res;
		number = number>>1;
	}
	while(res.length() <= rank){
		res = "0"+res;
	}
	
	return res;
}

void HashSet::printLayout(uint rank){
	uint base = (1<<rank);
	// Bucket* cur_node = buckets[0];
	// for(uint bucket_remainder = 0; bucket_remainder < base; ++bucket_remainder){
	// 	cout << "%" << base  << " = "<< getBinaryRepresentation(bucket_remainder, rank) << ":  ";
	// 	while(cur_node != nullptr && cur_node->orig_item%base == bucket_remainder){
	// 		cout << cur_node->toString() << "--> ";
	// 		cur_node = cur_node->next;
	// 	}
	// 	cout << "(end)\n";
	// }
	
	for(uint bucket_remainder = 0; bucket_remainder < base; ++bucket_remainder){
		cout << "%" << base  << " = "<< getBinaryRepresentation(bucket_remainder, rank) << ":  ";
		for(Bucket* cur_node = buckets[bucket_remainder]
				; cur_node != nullptr && cur_node->orig_item%base == bucket_remainder
				; cur_node = cur_node->next){
			cout << cur_node->toString() << "--> ";
		}
		cout << "(end)\n";
	}
}

string HashSet::Bucket::toString() const{
	string res;
	if(isSentinal(key))
		res += "s";
	res += std::to_string(unmarkSentinal(reverseBits(key)));
	return res;
}

HashSet::Bucket::Bucket(uint key)
	:key(key),next(nullptr),orig_item(unmarkSentinal(reverseBits(key))){}

bool HashSet::Bucket::contains(uint item, HashSet* set){
	uint target_key = reverseBits(item);
	for(Bucket* current_node = this
			; current_node != nullptr && current_node->key <= target_key
			; current_node = current_node->next){
		if(current_node->key == target_key)
			return true;
	}
	return false;
}

void HashSet::Bucket::remove(uint item){
	uint target_key = reverseBits(item);
	Bucket* prior_node = nullptr;
	for(Bucket* current_node = this; current_node->next != nullptr; current_node = current_node->next)
		if(current_node->next->key == target_key)
			prior_node = current_node;
	if(prior_node == nullptr)
		return;
	Bucket* to_remove = prior_node->next;
	prior_node->next = to_remove->next;
	delete to_remove;
}

HashSet::Bucket* HashSet::Bucket::insert(uint item){
	uint new_key = reverseBits(item);
	if(key > new_key){
		return safeInsertNext(new_key);
	}
	
	for(Bucket* current_bucket = this; true; current_bucket = current_bucket->next)
		if(current_bucket->next == nullptr || current_bucket->next->key > new_key)
			return current_bucket->safeInsertNext(new_key);
}

HashSet::Bucket* HashSet::Bucket::safeInsertNext(uint key){
	Bucket* new_bucket = new Bucket(key);
	new_bucket->next = next;
	next = new_bucket;
	return new_bucket;
}

uint HashSet::markAsSentinal(uint number){
	return number | 0x80000000;
}

uint HashSet::unmarkSentinal(uint number){
	return number & 0x7fffffff;
}

bool HashSet::isSentinal(uint key){
	return (reverseBits(key) & 0x80000000) > 0;
}

uint HashSet::reverseBits(uint number){
    uint NO_OF_BITS = sizeof(number) * 8;
    uint reverse_num = 0;
    for (uint i = 0; i < NO_OF_BITS; i++) {
        if ((number & (1 << i)))
            reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return reverse_num;
}

HashSet::Bucket* HashSet::getContainingBucket(uint item, uint rank){
	uint bucket_idx = item%(1<<rank);
	if(buckets[bucket_idx] != nullptr)
		return buckets[bucket_idx];

	Bucket* super_bucket = getContainingBucket(bucket_idx, rank-1);

	if(buckets[bucket_idx] == nullptr){
		buckets[bucket_idx] = super_bucket->insert(markAsSentinal(bucket_idx));
		if(verbose)
			printLayout();
	}
	return buckets[bucket_idx];
}