
struct ptr
{
  ptr& operator= (ptr&& __u);
  explicit operator bool() const;
};

ptr getShare ();

template<class T>
class AnaToolHandle final
{
public:
  int getMode (ptr& sharedTool) const;
};

template<class T>
int AnaToolHandle<T>::getMode (ptr& sharedTool) const
{
  if ((sharedTool = getShare ()))
    return 0;
  
  return 1;
}

void initialize(AnaToolHandle<int>& jetCleaningTool)
{
  ptr sharedTool;
  jetCleaningTool.getMode (sharedTool);
}
