use rand::Rng;
use syn::{parse_quote, Item, ItemImpl, ItemTrait, ImplItem, TypeParam};

struct Replace_Generic_Assoc_Type_With_Primitive_Type_81;

impl Mutator for Replace_Generic_Assoc_Type_With_Primitive_Type_81 {
    fn name(&self) -> &str {
        "Replace_Generic_Assoc_Type_With_Primitive_Type_81"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for type_param in &mut trait_item.generics.type_params_mut() {
                    let primitive_type = match rand::thread_rng().gen_range(0..3) {
                        0 => parse_quote!(i32),
                        1 => parse_quote!(bool),
                        2 => parse_quote!(char),
                        _ => unreachable!(),
                    };
                    type_param.default = Some(primitive_type);
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Type(assoc_type) = impl_item {
                        let primitive_type = match rand::thread_rng().gen_range(0..3) {
                            0 => parse_quote!(i32),
                            1 => parse_quote!(bool),
                            2 => parse_quote!(char),
                            _ => unreachable!(),
                        };
                        *assoc_type = parse_quote!(type _ = #primitive_type;);
                    }
                }
            }
        }
    }
}

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
}