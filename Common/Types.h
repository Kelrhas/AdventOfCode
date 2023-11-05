#pragma once

#include <cstdint>
#include <iostream>

using U8  = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;
using S64 = int64_t;

const U8 CONSOLE_WHITE			  = 0x0F;
const U8 CONSOLE_WHITE_BACKGROUND = 0xF0;
const U8 CONSOLE_GREEN			  = 0x0A;
const U8 CONSOLE_RED			  = 0x0C;

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtx/rotate_vector.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm/gtx/hash.hpp>

template<typename cT>
std::basic_ostream<cT> &operator<<(std::basic_ostream<cT> &out, glm::vec2 const &_vec)
{
	out << _vec.x << "," << _vec.y;
	return out;
}
template<typename cT>
std::basic_ostream<cT> &operator<<(std::basic_ostream<cT> &out, glm::vec3 const &_vec)
{
	out << _vec.x << "," << _vec.y << "," << _vec.z;
	return out;
}
template<typename cT>
std::basic_ostream<cT> &operator<<(std::basic_ostream<cT> &out, glm::ivec2 const &_vec)
{
	out << fixed << _vec.x << "," << _vec.y;
	return out;
}
template<typename cT>
std::basic_ostream<cT> &operator<<(std::basic_ostream<cT> &out, glm::ivec3 const &_vec)
{
	out << fixed << _vec.x << "," << _vec.y << "," << _vec.z;
	return out;
}

template<int N, typename T, glm::qualifier Q>
T ManhatanDist(const glm::vec<N, T, Q> &lhs, const glm::vec<N, T, Q> &rhs)
{
	T dist = 0;
	for (int i = 0; i < N; ++i)
		dist += glm::abs(lhs[i] - rhs[i]);
	return dist;
}

template<int N, typename T, glm::qualifier Q>
T ChebyshevDist(const glm::vec<N, T, Q> &lhs, const glm::vec<N, T, Q> &rhs)
{
	T dist = 0;
	for (int i = 0; i < N; ++i)
		dist = glm::max(dist, glm::abs(lhs[i] - rhs[i]));
	return dist;
}

template<typename T>
constexpr int Sign(T val)
{
	return (T(0) < val) - (val < T(0));
}


template<size_t Size, typename T>
struct SquareMatrix
{
	array<array<T, Size>, Size> mat { 0 };

	constexpr SquareMatrix() noexcept = default;
	constexpr SquareMatrix(const initializer_list<initializer_list<T>> &init) noexcept
	{
		int i = 0, j = 0;
		assert(init.size() == Size);
		for (const auto &row : init)
		{
			assert(row.size() == Size);
			for (const auto &v : row)
			{
				mat[i][j] = v;
				++j;
			}
			j = 0;
			++i;
		}
	}

	SquareMatrix &operator=(const SquareMatrix &rhs)
	{
		mat = rhs.mat;
		return *this;
	}

	SquareMatrix operator*(const SquareMatrix &rhs) const
	{
		SquareMatrix res;
		for (int i = 0; i < Size; ++i)
		{
			for (int j = 0; j < Size; ++j)
			{
				res.mat[i][j] = 0;
				for (int k = 0; k < Size; ++k)
				{
					res.mat[i][j] += mat[i][k] * rhs.mat[k][j];
				}
			}
		}
		return res;
	}

	SquareMatrix &operator*=(const SquareMatrix &rhs)
	{
		SquareMatrix res = *this * rhs;
		*this			 = res;
		return *this;
	}

	T &	   at(size_t i, size_t j) { return mat[i][j]; }
	size_t size() const { return Size; }
};


template<typename T>
struct CircleList
{
	CircleList() = default;
	CircleList(CircleList const& rhs)
	{
		for (auto it = rhs.begin(); it != rhs.end(); ++it)
		{
			Push(*it);
		}
		Push(*rhs.end());
	}
	CircleList(CircleList &&) = delete;


	struct Node
	{
		T	  value;
		Node *pPrev = nullptr;
		Node *pNext = nullptr;
	};

	struct iterator
	{
		Node *		   pNode = nullptr;
		const CircleList<T> &ref;

		iterator(Node *_pNode, const CircleList<T> &_ref) : pNode(_pNode), ref(_ref) {}
		iterator(iterator const &_it) : pNode(_it.pNode), ref(_it.ref) {}
		iterator &operator=(iterator const &) = default;

		iterator &operator++()
		{
			pNode = pNode->pNext;
			return *this;
		}
		iterator operator++(int)
		{
			iterator it = *this;
			it.pNode	= pNode;
			pNode		= pNode->pNext;
			return it;
		}
		iterator &operator--()
		{
			pNode = pNode->pPrev;
			return *this;
		}
		bool  operator==(iterator const &_rhs) { return pNode == _rhs.pNode; }
		bool  operator!=(iterator const &_rhs) { return !(*this == _rhs); }
		T &	  operator*() { return pNode->value; }
		Node *operator->() { return pNode; }

	protected:
		iterator() = default;
	};

	iterator begin() const { return iterator { m_pHead, *this }; }
	iterator end() const { return iterator { m_pTail, *this }; }
	size_t	 size() const { return m_size; }

	void Push(const T &_val)
	{
		Node *pNew = new Node { _val };
		if (m_pHead == nullptr) // first add
		{
			m_pHead = pNew;
		}
		else
		{
			m_pHead->pPrev = pNew;
			pNew->pNext	   = m_pHead;
		}

		if (m_pTail)
		{
			m_pTail->pNext = pNew;
			pNew->pPrev	   = m_pTail;
		}

		m_pTail = pNew;
		m_size++;
	}

	void MoveAfter(iterator _it, iterator _where)
	{
		// remove
		_it->pPrev->pNext = _it->pNext;
		_it->pNext->pPrev = _it->pPrev;

		// add
		_where->pNext->pPrev = _it.pNode;
		_it->pNext			 = _where->pNext;
		_where->pNext		 = _it.pNode;
		_it->pPrev			 = _where.pNode;
	}

private:
	Node * m_pHead = nullptr;
	Node * m_pTail = nullptr;
	size_t m_size  = 0;
};
