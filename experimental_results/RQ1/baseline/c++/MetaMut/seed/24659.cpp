
#include <stdio.h>
#include <vector>

class Path
{
public:
	int num_subpaths() const { return static_cast<int>(m_subpaths.size()); }

	struct SubpathRecord
	{
		int	m_start_index;
		int	m_closed;
		int	m_type;
	};
	
public:
	Path()
	{
		m_subpaths.push_back( SubpathRecord() );
	}

private:
	std::vector<SubpathRecord>	m_subpaths;
};

