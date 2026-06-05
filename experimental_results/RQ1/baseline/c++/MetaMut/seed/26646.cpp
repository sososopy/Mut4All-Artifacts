

int d;

void foo(){
    __transaction_atomic {
        d++;
    }
}

int main() {
    for(int i = 0; i < 2; i++){
        __transaction_atomic {
            foo();
        }

        std::vector<int> myVec;
        myVec.push_back(40);
    }
}

