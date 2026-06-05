struct s{
    template<class T>
    int operator+(const T&)const{return 0;}
};

template<class T>
concept AlwaysTrue=true;

template<AlwaysTrue T>
int operator+(const s&,const T&){return 1;}

int main(){
    return s{}+1;
}