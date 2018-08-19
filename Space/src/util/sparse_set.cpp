#include "util\sparse_set.h"

sparse_set::sparse_set(unsigned int dence_capacity, unsigned int sparse_capacity)
	: m_dense_capacity(dence_capacity), m_sparse_capacity(sparse_capacity), m_count(0) {
	m_dense = new unsigned int[dence_capacity];
	m_sparse = new unsigned int[sparse_capacity];
}

sparse_set::~sparse_set() {
	delete[] m_dense;
	delete[] m_sparse;
}

bool sparse_set::insert(unsigned int index) {
	if (index > m_sparse_capacity)  return false; //Index too big
	if (m_count > m_dense_capacity) return false; //No more space
	if (at(index) != -1)			return false; //Index already exists

	m_sparse[index] = m_count;
	m_dense[m_count] = index;

	m_count++;

	return true;
}

bool sparse_set::remove(unsigned int index) {
	if (index > m_sparse_capacity) return false; //Index too big

	unsigned int dence_index = m_sparse[index];
	int swap = m_dense[m_count - 1];

	m_dense[dence_index] = swap;
	m_sparse[swap] = dence_index;

	m_count--;

	return true;
}

int sparse_set::at(unsigned int index) {
	if (index > m_sparse_capacity) return -1; //Index too big

	unsigned int dence_index = m_sparse[index];
	if (dence_index < m_count && m_dense[dence_index] == index) {
		return dence_index;
	}

	return -1;
}