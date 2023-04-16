// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	template <typename ValueType, typename SizeType, SizeType DefaultEmptyValue = 0>
	class SparseSet {
	  public:
		static_assert(std::is_unsigned_v<SizeType>, "Expected an unsigned size value");

		using DenseList = std::vector<ValueType>;
		using SparseList = std::vector<SizeType>;
		using Iterator = typename DenseList::iterator;
		using ConstIterator = typename DenseList::const_iterator;
		static constexpr SizeType EMPTY_ENTRY = std::numeric_limits<SizeType>::max();

		ValueType& operator[](SizeType index) { return Get(index); }

		std::vector<ValueType>& GetData() { return mDenseList; }

		const std::vector<ValueType>& GetData() const { return mDenseList; }

		ValueType& Get(SizeType index)
		{
			ZE_ASSERT(index < mSparseList.size(), "Invalid access at index");
			ZE_ASSERT(mSparseList[index] != DefaultEmptyValue, "Invalid value at index");
			return mDenseList[mSparseList[index]];
		}

		const ValueType& Get(SizeType index) const
		{
			ZE_ASSERT(index < mSparseList.size(), "Invalid access at index");
			ZE_ASSERT(mSparseList[index] != DefaultEmptyValue, "Invalid value at index");
			return mDenseList[mSparseList[index]];
		}

		ValueType* TryGet(SizeType index)
		{
			if (index < mSparseList.size() && mSparseList[index] != DefaultEmptyValue) {
				return &mDenseList[mSparseList[index]];
			}
			return nullptr;
		}

		bool Contains(SizeType index) const { return index < mSparseList.size() && mSparseList[index] != DefaultEmptyValue; }

		size_t GetSize() const { return mDenseList.size(); }

		void Add(SizeType index, ValueType val)
		{
			if (mSparseList.size() <= index) {
				mSparseList.resize(index + 1, DefaultEmptyValue);
			}
			ZE_ASSERT(mSparseList[index] == DefaultEmptyValue, "Index should be empty");
			mDenseList.emplace_back(std::forward<ValueType>(val));
			mSparseList[index] = mDenseList.size() - 1;
		}

		void Remove(SizeType index)
		{
			ZE_ASSERT(Contains(index), "Unable to find the index");
			std::swap(mDenseList[mSparseList[index]], mDenseList[mDenseList.size() - 1]);
			mDenseList.pop_back();
			mSparseList[index] = DefaultEmptyValue;
		}

		Iterator begin() { return mDenseList.begin(); }

		Iterator end() { return mDenseList.end(); }

		ConstIterator cbegin() const { return mDenseList.cbegin(); }

		ConstIterator cend() const { return mDenseList.cend(); }

	  private:
		DenseList mDenseList;
		SparseList mSparseList;
	};

}