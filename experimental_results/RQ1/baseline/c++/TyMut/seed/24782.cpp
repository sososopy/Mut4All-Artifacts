
enum class En {A};
struct St {En field :1;};

int main(void)
{
    volatile St s = {.field = En::A};
    switch(s.field) {
        case En::A : break;
    }
}

