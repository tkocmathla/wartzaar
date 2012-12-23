#ifndef WARTZAAR_PRIORITY_VECTOR_H_
#define WARTZAAR_PRIORITY_VECTOR_H_

#include <vector>

namespace wartzaar {

/// PriorityVector is an unbounded priority queue based on a vector.
template <class T> class PriorityVector {
 public:
  PriorityVector();

  /// Adds a new element to the queue, in priority order.
  void add(const T &element);

  /// Removes all elements from the queue.
  void clear();

  /// Returns true if the queue is empty.
  bool empty() const;

  /// Removes the queue element matching the specified element.
  bool remove(const T &element);

  /// Returns a pointer to the first element in the queue.
  T* top() const;

  /// Removes and returns a copy of the first element in the queue.
  T pop();

  /// Returns true if the queue contains the specified element.
  bool contains(const T &element) const;

  /// Returns an iterator to the front of the queue.
  typename std::vector<T>::iterator begin();

  /// Returns an iterator to the end of the queue.
  typename std::vector<T>::iterator end();

  /// Returns the number of elements in the queue.
  size_t size() const;

 private:
  /// The container of elements.
  std::vector<T> elements_;
};

template <class T> PriorityVector<T>::PriorityVector()
    : elements_(std::vector<T>()) {}

template <class T> void PriorityVector<T>::add(const T &element) {
  bool found = false;
  typename std::vector<T>::iterator itr = elements_.begin();

  while (itr != elements_.end()) {
    if (element > *itr) {
      elements_.insert(itr, element);
      found = true;
      break;
    }
    else {
      ++itr;
    }
  }

  if (!found)
    elements_.push_back(element);
}

template <class T> void PriorityVector<T>::clear() {
  elements_.clear();
}

template <class T> bool PriorityVector<T>::empty() const {
  return elements_.empty();
}

template <class T> bool PriorityVector<T>::remove(const T &element) {
  bool found = false;
  typename std::vector<T>::iterator itr = elements_.begin();

  while (itr != elements_.end()) {
    if (*itr == element) {
      elements_.erase(itr);
      found = true;
      break;
    }
    else {
      ++itr;
    }
  }

  return found;
}

template <class T> T* PriorityVector<T>::top() const {
  return const_cast<T*>(&elements_[0]);
}

template <class T> T PriorityVector<T>::pop() {
  T temp = T(elements_[0]);
  elements_.erase(elements_.begin());
  return temp;
}

template <class T> bool PriorityVector<T>::contains(const T &element) const {
  bool found = false;
  typename std::vector<T>::const_iterator itr = elements_.begin();

  while (itr != elements_.end()) {
    if (*itr == element) {
      found = true;
      break;
    }
    else {
      ++itr;
    }
  }

  return found;
}

template <class T> typename std::vector<T>::iterator PriorityVector<T>::begin() {
  return elements_.begin();
}

template <class T> typename std::vector<T>::iterator PriorityVector<T>::end() {
  return elements_.end();
}

template <class T> size_t PriorityVector<T>::size() const {
  return elements_.size();
}

} // namespace wartzaar

#endif // WARTZAAR_PRIORITY_VECTOR_H_
