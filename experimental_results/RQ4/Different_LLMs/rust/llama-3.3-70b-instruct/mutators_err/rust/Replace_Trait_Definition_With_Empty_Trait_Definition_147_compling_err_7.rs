use syn::{File, Item, Trait, GenericParam, LifetimeParam, Lifetime, Ident, punctuated::Punctuated};
use proc_macro2::{Span, Token};

struct Replace_Trait_Definition_With_Empty_Trait_Definition_147;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Replace_Trait_Definition_With_Empty_Trait_Definition_147 {
    fn name(&self) -> &str {
        "Replace_Trait_Definition_With_Empty_Trait_Definition_147"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Trait(trait_def) = item {
                let mut new_generics = trait_def.generics.clone();
                new_generics.params.clear();
                for _ in &trait_def.generics.params {
                    new_generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: Lifetime {
                            apostrophe: Token!['],
                            ident: Ident::new("'_", Span::call_site()),
                        },
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                }
                trait_def.generics = new_generics;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait definitions with empty trait definitions, replacing all type parameters with underscores. This transformation tests the compiler's handling of trait definitions and type parameter resolution."
    }
}