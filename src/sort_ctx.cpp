#include "sort_ctx.hpp"
#include <algorithm>
#include <random>

#define SV_COMPARE(arr, a, b)                                                  \
  arr[a] < arr[b];                                                             \
  co_yield sv::SortEvent { sv::SortBehavior::Compare, (a), (b) }

#define SV_SWAP(arr, a, b)                                                     \
  if (a != b)                                                                  \
    std::swap(arr[a], arr[b]);                                                 \
  co_yield sv::SortEvent { sv::SortBehavior::Swap, (a), (b) }

#define SV_SET(arr, i, x)                                                      \
  arr[i] = x;                                                                  \
  co_yield sv::SortEvent { sv::SortBehavior::Set, (i), 0 }

#define SV_NEST(co)                                                            \
  {                                                                            \
    auto c = (co);                                                             \
    while (c)                                                                  \
      co_yield c();                                                            \
  }

#define SV_NONE                                                                \
  co_yield sv::SortEvent { sv::SortBehavior::None, 0, 0 }

sv::SortContext::SortContext(size_t size) : m_Progress{nullptr} {
  SetSize(size);
}

sv::SortContext::~SortContext() { delete m_Progress; }

void sv::SortContext::SetSize(size_t size) {
  delete m_Progress;
  m_Progress = nullptr;

  m_Array.resize(size);
  for (int i = 0; i < m_Array.size(); i++) {
    m_Array[i] = i;
  }
}

sv::SortEvent sv::SortContext::Next() {
  if (m_Progress && (bool)*m_Progress) {
    return (*m_Progress)();
  } else {
    return {sv::SortBehavior::None, 0, 0};
  }
}

std::span<const int> sv::SortContext::GetArray() const {
  return {m_Array.cbegin(), m_Array.cend()};
}

void sv::SortContext::Shuffle() {
  delete m_Progress;
  m_Progress = new sv::Progress{[](std::vector<int> &array) -> sv::Progress {
    std::random_device rd;
    for (int i = 0; i < array.size() - 2; i++) {
      std::uniform_int_distribution<int> dist{i, (int)array.size() - 1};
      int j = dist(rd);
      SV_SWAP(array, i, j);
    }
    SV_NONE;
  }(m_Array)};
}

void sv::SortContext::BubbleSort() {
  delete m_Progress;
  m_Progress = new sv::Progress{[](std::vector<int> &array) -> sv::Progress {
    for (int i = 0; i < array.size() - 1; i++) {
      bool swapped = false;
      for (int j = 0; j < array.size() - i - 1; j++) {
        bool cmp = SV_COMPARE(array, j + 1, j);
        if (cmp) {
          SV_SWAP(array, j, j + 1);
          swapped = true;
        }
      }
      if (!swapped) {
        break;
      }
    }
    SV_NONE;
  }(m_Array)};
}

void sv::SortContext::SelectionSort() {
  delete m_Progress;
  m_Progress = new sv::Progress{[](std::vector<int> &array) -> sv::Progress {
    for (int i = 0; i < array.size() - 1; i++) {
      int min = i;
      for (int j = i + 1; j < array.size(); j++) {
        bool cmp = SV_COMPARE(array, j, min);
        if (cmp) {
          min = j;
        }
      }
      if (min != i) {
        SV_SWAP(array, min, i);
      }
    }
    SV_NONE;
  }(m_Array)};
}

void sv::SortContext::InsertionSort() {
  delete m_Progress;
  m_Progress = new sv::Progress{[](std::vector<int> &array) -> sv::Progress {
    for (int i = 1; i < array.size(); i++) {
      for (int j = i; j > 0; j--) {
        bool cmp = SV_COMPARE(array, j, j - 1);
        if (cmp) {
          SV_SWAP(array, j - 1, j);
        } else {
          break;
        }
      }
    }
    SV_NONE;
  }(m_Array)};
}

static sv::Progress Merge(std::vector<int> &array, int low, int mid, int high) {
  int *arr = new int[high - low];

  int i1 = low, i2 = mid;
  int i = 0;

  while (i1 < mid && i2 < high) {
    int cmp;
    cmp = SV_COMPARE(array, i2, i1);
    if (!cmp) {
      arr[i++] = array[i1++];
    } else {
      arr[i++] = array[i2++];
    }
  }

  while (i1 < mid) {
    arr[i++] = array[i1++];
  }

  while (i2 < high) {
    arr[i++] = array[i2++];
  }

  for (int i = low; i < high; i++) {
    SV_SET(array, i, arr[i - low]);
  }
  delete[] arr;
}

static sv::Progress MergeSort(std::vector<int> &array, int low, int high) {
  if (high - low <= 1)
    co_return;

  int mid = (low + high) / 2;

  SV_NEST(MergeSort(array, low, mid))
  SV_NEST(MergeSort(array, mid, high))
  SV_NEST(Merge(array, low, mid, high))
}

void sv::SortContext::MergeSort() {
  delete m_Progress;
  m_Progress = new sv::Progress{[](std::vector<int> &array) -> sv::Progress {
    int mid = array.size() / 2;
    SV_NEST(::MergeSort(array, 0, array.size()))
  }(m_Array)};
}

static sv::Progress Partition(std::vector<int> &array, int low, int high,
                              int &pivot) {
  SV_SWAP(array, (low + high) / 2, high - 1);
  pivot = high - 1;
  int i = low;
  for (int j = low; j < high - 1; j++) {
    bool cmp = SV_COMPARE(array, j, pivot);
    if (cmp) {
      SV_SWAP(array, i, j);
      i++;
    }
  }
  SV_SWAP(array, i, pivot);
  pivot = i;
}

static sv::Progress QuickSort(std::vector<int> &array, int low, int high) {
  if (high - low <= 1)
    co_return;
  int pivot;
  SV_NEST(Partition(array, low, high, pivot))
  SV_NEST(QuickSort(array, low, pivot))
  SV_NEST(QuickSort(array, pivot + 1, high))
}

void sv::SortContext::QuickSort() {
  delete m_Progress;
  m_Progress = new sv::Progress{[](std::vector<int> &array) -> sv::Progress {
    SV_NEST(::QuickSort(array, 0, array.size()))
  }(m_Array)};
}
