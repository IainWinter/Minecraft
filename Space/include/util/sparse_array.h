#pragma once

#include "sparse_set.h"

template<typename ItemT>
class sparse_array {
private:
	ItemT* m_data;
	sparse_set m_index;

public:
	sparse_array(unsigned int count);
	~sparse_array();

	bool insert(unsigned int index, const ItemT& data);
	bool remove(unsigned int index);
	ItemT* at(unsigned int index);
	
	inline void clear() {
		m_index.clear();
	}

	inline ItemT* data() const {
		return m_data;
	}
};

template<typename ItemT>
sparse_array<ItemT>::sparse_array(unsigned int count)
	: m_index(count, count)
{
	m_data = new ItemT[count];
}

template<typename ItemT>
inline sparse_array<ItemT>::~sparse_array() {
	delete m_data;
}

template<typename ItemT>
bool sparse_array<ItemT>::insert(unsigned int index, const ItemT& data) {
	m_index.insert(index);
	int data_index = m_index.at(index);
	bool data_has_index = data_index != -1;
	if (data_has_index) {
		m_data[data_index] = data;
	}

	return data_has_index;
}

template<typename ItemT>
bool sparse_array<ItemT>::remove(unsigned int index) {
	unsigned int data_count = m_index.count();
	int data_index = m_index.at(index);
	bool data_has_index = data_index != -1;
	if (data_has_index) {
		m_data[data_index] = m_data[data_count - 1];
		m_index.remove(index);
	}

	return data_has_index;
}

template<typename ItemT>
ItemT* sparse_array<ItemT>::at(unsigned int index) {
	int data_index = m_index.at(index);
	bool data_has_index = data_index != -1;
	if (data_has_index) {
		return &m_data[data_index];
	}

	return nullptr;
}