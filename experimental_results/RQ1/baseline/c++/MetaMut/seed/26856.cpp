
bool operator< (const chave& lhs, const chave& rhs) {
    if(lhs.numeros_ord != rhs.numeros_ord)
        return lhs.numeros_ord < rhs.numeros_ord;
    return lhs.estrelas_ord < rhs.estrelas_ord;
}

