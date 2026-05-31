use crate::mutator::Mutator;
use syn::{File, Item, GenericParam, Type};
use proc_macro2::Ident;

pub struct Modify_Const_Generics_Invalid_Type_385;

impl Mutator for Modify_Const_Generics_Invalid_Type_385 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_Invalid_Type_385"
    }
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &mut const_param.ty {
                            if let Some(segment) = type_path.path.segments.last_mut() {
                                if segment.ident == "i32" {
                                    segment.ident = Ident::new("String", segment.ident.span());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies functions using const generics in their signature. It specifically targets the type used in the const generic and replaces it with an incompatible type, such as replacing 'i32' with 'String'. This aims to explore scenarios where the Rust compiler's type checking of const generics might lead to internal errors, particularly with experimental features like 'adt_const_params'. The mutation is syntactically correct but semantically incorrect, designed to trigger type errors during compilation."
    }
}