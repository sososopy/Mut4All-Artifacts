impl Mutator for Replace_Enum_Variant_Order_183 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_Order_183"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut variants: Vec<_> = enum_item.variants.iter().cloned().collect();
                variants.shuffle(&mut thread_rng());
                enum_item.variants = variants.into_iter().collect();
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the order of enum variants. It aims to test the compiler's handling of enum layout and transmutability, potentially leading to bugs related to padding and alignment."
    }
}