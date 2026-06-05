
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <list>


class CPriceLst;
class DPriceLst;
class DDPriceLst;
class ProdBook;

class PriceLst {
public:
    virtual ~PriceLst();
};


class DPriceLst : public PriceLst
{
public:
};

class DDPriceLst : public DPriceLst
{
public:
};


class ProdBook {
public:
    PriceLst    m_bs_pr[2];
    DPriceLst   m_bs_d_pr[2];  // derived order lst
    DDPriceLst  m_bs_dd_pr[2]; // 2nd derived lst

    ProdBook();
};


class OrderBook {
public:
    std::vector<ProdBook> m_prod;
};



int main() {
        void *ptr=NULL;
        ProdBook prod;

        ((OrderBook*)ptr)->m_prod.push_back(prod);

        return 0;
}
