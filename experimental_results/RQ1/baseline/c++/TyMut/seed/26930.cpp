
template <class T>
 inline
 SPORef<T>::SPORef
  /// Constructor: increment pointed to T's reference count.
  ( T* p                ///< Pointer to the T to be managed
  )
  : rep_(p)
{
  badcode();
  if (rep_)
    rep_->incReferences();
}

template <class T>
 inline
 SPORef<T>::~SPORef
  /// Destructor: decrement pointed to T's reference count, possibly
  /// triggering its destruction.
  ()
{
  badcode();
  if (rep_) {
    rep_->decReferences();
    rep_ = 0;
  }
}

