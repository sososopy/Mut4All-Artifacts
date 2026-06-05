

void *Collector::allocateObject(bool hasFinalizer)
{
  if (freePtr_ == &allObjectsList_)
    makeSpace();
  Object *tem = freePtr_;
  freePtr_ = freePtr_->next();
  tem->setColor(currentColor_);
  tem->hasFinalizer_ = hasFinalizer;
  if (hasFinalizer)
    tem->moveAfter(&allObjectsList_);
  return tem;
}

