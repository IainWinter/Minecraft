#pragma once

class sparse_set {
private:
	unsigned int* m_dense;
	unsigned int* m_sparse;
	unsigned int m_count;
	unsigned int m_dense_capacity;
	unsigned int m_sparse_capacity;
public:
	sparse_set(unsigned int dence_capacity, unsigned int sparse_capacity);
	~sparse_set();

	bool insert(unsigned int index);
	bool remove(unsigned int index);
	int at(unsigned int index);

	inline void clear() {
		m_count = 0;
	}

	inline unsigned int count() {
		return m_count;
	}

	inline unsigned int sparse_capacity() {
		return m_sparse_capacity;
	}

	inline unsigned int dense_capacity() {
		return m_dense_capacity;
	}
};