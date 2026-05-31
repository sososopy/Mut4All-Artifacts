use syn::{parse_quote, Item, TraitItem, TypeParam};
use crate::mutator::Mutator;

struct Replace_Generic_Associated_Type_With_Regular_Associated_Type_138;

impl Mutator for Replace_Generic_Associated_Type_With_Regular_Associated_Type_138 {
    fn name(&self) -> &str {
        "Replace_Generic_Associated_Type_With_Regular_Associated_Type_138"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let TraitItem::Type(TypeParam {
                        attrs: _,
                        ident,
                        bounds,
                        eq_token,
                        colon_token: _,
                        default: _,
                    }) = item {
                        let new_type_param = parse_quote!(type #ident;);
                        *item = TraitItem::Type(new_type_param);
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets traits with generic associated types and replaces them with regular associated types. This transformation removes the generic parameter, potentially leading to type mismatches or inference failures. It aims to test the compiler's handling of generic associated types and their interactions with other type system features."
    }
}