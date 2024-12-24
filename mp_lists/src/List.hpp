/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(head_,tail_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(NULL, tail_);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in mp_lists part 1
  while (head_ != NULL) {
    ListNode* tmp = head_->next;
    delete head_;
    head_ = tmp;
  }
  head_ = NULL;
  tail_ = NULL;
  length_ = 0;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = NULL;
  
  if (head_ != NULL) {
    head_ -> prev = newNode;
  }
  if (tail_ == NULL) {
    tail_ = newNode;
  }
  
  head_ = newNode;

  length_++;

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode* node = new ListNode(ndata);
  node->prev = tail_;

  if (tail_ != NULL) tail_->next = node;

  tail_ = node;

  if (head_ == NULL) head_ = node;
  
  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in mp_lists part 1
  ListNode * curr = start;

  for (int i = 0; i < splitPoint && curr != NULL; i++) {
    curr = curr->next;
  }

  if (curr != NULL) {
    //if splitPoint is 0
      if (curr->prev != NULL) curr->prev->next = NULL;
      curr->prev = NULL;
  }

  return curr;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  /// @todo Graded in part 1
  bool toflip = false;
  ListNode* curr = head_;

  /*no null check needed, since curr moves by one every time,
  so it will reach tail before walking off the end.
  Unless tail is already null, which is already caught by the != tail check
  */
  while (curr != tail_) {
    if (toflip) {
      ListNode* prev = curr->prev;
      ListNode* next = curr->next;

      //link neighboring nodes, next is either tail or before, so it will never be null
      if (prev != NULL) prev->next = next;
      next->prev = prev;

      //tail is not null, attach curr to the end
      tail_->next = curr;
      curr->prev = tail_;
      curr->next = NULL;
      tail_ = curr;

      //reposition curr
      curr = next;
    } else {
      curr = curr->next;
    }
    toflip = !toflip;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in mp_lists part 2
  if (startPoint == NULL || endPoint == NULL) return;

  ListNode* startPrev = startPoint->prev;
  ListNode* endNext = endPoint->next;

  ListNode* curr = startPoint;

  while (curr != endNext) {
    ListNode* tmp = curr->next;

    curr->next = curr->prev;
    curr->prev = tmp;

    curr = tmp;
  }

  //swap the references
  ListNode* tmp = startPoint;
  startPoint = endPoint;
  endPoint = tmp;

  //reconnect startPrev and endNext
  endPoint->next = endNext;
  startPoint->prev = startPrev;
  if (startPrev != NULL) startPrev->next = startPoint;
  if (endNext != NULL) endNext->prev = endPoint;

}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in mp_lists part 2

  ListNode* start = head_;
  
  while (start != NULL) {
    ListNode* end = start;
    //moves next n-1 times for a block of n
    for (int i = 1; i < n && end != NULL; i++) {
      end = end->next;
    }

    //edge cases, to take advantage of reverse using references
    if (start == head_ && (end == tail_ || end == NULL)) {
      reverse(head_, tail_);
      break;
    }
    if (end == NULL) {
      reverse(start, tail_);
      break;
    }

    if (start == head_) {
      reverse(head_, end);
    } else {
      reverse(start, end);
    }
    
    start = end->next;
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in mp_lists part 2
  if (first == NULL) return second;
  if (second == NULL) return first;

  ListNode* fhead;
  //decides which node fhead should start at
  if (first->data < second->data) {
    fhead = first;
    first = first->next;
  } else {
    fhead = second;
    second = second->next;
  }

  ListNode* curr = fhead;
  //while comparisons are still needed
  while (first != NULL && second != NULL) {
    if (first->data < second->data) {
      curr->next = first;
      first->prev = curr;
      first = first->next;
    } else {
      curr->next = second;
      second->prev = curr;
      second = second->next;
    }
    curr = curr->next;
  }

  //one of them must be non-null, since the loop only eliminates 1 element at a time
  if (first == NULL) {
    curr->next = second;
    second->prev = curr;
  } else {
    curr->next = first;
    first->prev = curr;
  }
  return fhead;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in mp_lists part 2
  if (chainLength <= 1) return start;

  int midpt = chainLength/2;
  ListNode* second = split(start, midpt);
  //chainlength - midpt for the second call to account for off by 1 errors with odd chainLengths
  return merge(mergesort(start, midpt), mergesort(second, chainLength - midpt));
}
