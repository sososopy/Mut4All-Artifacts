use syn::{parse_quote, Item, GenericParam, Generics};
use rand::thread_rng;
use rand::Rng;
use crate::mutator::Mutator;

struct Replace_Const_Generic_Parameters_With_Different_Types_350;

impl Mutator for Replace_Const_Generic_Parameters_With_Different_Types_350 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Paramifiers_With_Different_Types_350"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                match item_struct.generics {
                    syn::Generics::WhereClause(_) => continue,
                    syn::Generics::None => continue,
                    syn::Generics::Multiple(generics) => {
                        for param in &mut generics.params {
                            if let GenericParam::Const(param) = param {
                                let new_type = match rng.gen_range(0..3) {
                                    0 => parse_quote!(u8),
                                    1 => parse_quote!(i32),
                                    2 => parse_quote!(usize),
                                    _ => unreachable!(),
                                };
                                param.ty = new_type;
                            }
                        }
                    }
                };
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with different types (u8, i32, usize) in struct definitions. This transformation tests the compiler's handling of different types in const generic parameters and can help reveal bugs related to type parameter handling and substitution."
    }
}