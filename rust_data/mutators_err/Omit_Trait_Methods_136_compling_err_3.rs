use syn::Item;

pub struct Omit_Trait_Methods_136;

impl Mutator for Omit_Trait_Methods_136 {
    fn name(&self) -> &str {
        "Omit_Trait_Methods_136"
    }
    fn mutate(&self, file: &mut syn::File) {
        let trait_methods: Vec<_> = file.items.iter().filter_map(|i| {
            if let Item::Trait(item_trait) = i {
                Some((item_trait.ident.clone(), item_trait.items.clone()))
            } else {
                None
            }
        }).collect();

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    let trait_name = &path.segments.last().unwrap().ident;

                    if trait_methods.iter().any(|(name, _)| name == trait_name) {
                        item_impl.items.clear();
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations. It identifies implementation blocks for traits and removes the methods that are required by the trait, leading to a compilation error due to missing method implementations."
    }
}