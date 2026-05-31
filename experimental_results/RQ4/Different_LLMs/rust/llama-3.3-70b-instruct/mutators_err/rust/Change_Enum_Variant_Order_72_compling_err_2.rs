impl Mutator for Change_Enum_Variant_Order_72 {
    fn name(&self) -> &str {
        "Change_Enum_Variant_Order_72"
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
        "The mutation operator changes the order of enum variants. This transformation tests the compiler's handling of enum layouts and may expose bugs related to transmutability and discriminant values."
    }
}