use syn::{parse_quote, GenericParam};
use crate::mutator::Mutator;

struct Replace_Const_Generic_Parameter_With_Function_Pointer_385;

impl Mutator for Replace_Const_Generic_Parameter_With_Function_Pointer_385 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Parameter_With_Function_Pointer_385"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if struct_item.generics.params.is_empty() {
                    struct_item.generics = parse_quote!(<const F: fn() -> usize>);
                } else {
                    for param in &mut struct_item.generics.params {
                        if let GenericParam::Const(_) = param {
                            *param = parse_quote!(const F: fn() -> usize);
                        }
                    }
                }
            } else if let syn::Item::Enum(enum_item) = item {
                if enum_item.generics.params.is_empty() {
                    enum_item.generics = parse_quote!(<const F: fn() -> usize>);
                } else {
                    for param in &mut enum_item.generics.params {
                        if let GenericParam::Const(_) = param {
                            *param = parse_quote!(const F: fn() -> usize);
                        }
                    }
                }
            } else if let syn::Item::Fn(func) = item {
                if func.sig.generics.params.is_empty() {
                    func.sig.generics = parse_quote!(<const F: fn() -> usize>);
                } else {
                    for param in &mut func.sig.generics.params {
                        if let GenericParam::Const(_) = param {
                            *param = parse_quote!(const F: fn() -> usize);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with function pointers in structs, enums, and functions. This transformation tests the compiler's handling of const generic parameters and function pointers, which is a specific area where the bug report indicates a problem."
    }
}