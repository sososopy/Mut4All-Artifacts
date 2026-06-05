
    static unsigned hash(SecurityOrigin* origin)
    {
        unsigned hashCodes[3] = {
            origin->protocol().impl() ? origin->protocol().impl()->hash() : 0,
            origin->host().impl() ? origin->host().impl()->hash() : 0,
            origin->port()
        };
        return StringHasher::hashMemory<sizeof(hashCodes)>(hashCodes);
    }

