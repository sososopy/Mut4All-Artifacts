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
                if let syn::Generics::None = struct_item.generics {
                    struct_item.generics = parse_quote!(<const F: fn() -> usize>);
                } else {
                    let generics = struct_item.generics.get_mut().unwrap();
                    for param in &mut generics.params {
                        if let GenericParam::Const(const_param) = param {
                            *param = parse_quote!(const F: fn() -> usize);
                        }
                    }
                }
            } else if let syn::Item::Enum(enum_item) = item {
                if let syn::Generics::None = enum_item.generics {
                    enum_item.generics = parse_quote!(<const F: fn() -> usize>);
                } else {
                    let generics = enum_item.generics.get_mut().unwrap();
                    for param in &mut generics.params {
                        if let GenericParam::Const(const_param) = param {
                            *param = parse_quote!(const F: fn() -> usize);
                        }
                    }
                }
            } else if let syn::Item::Fn(func) = item {
                if let syn::Generics::None = func.sig.generics {
                    func.sig.generics = parse_quote!(<const F: fn() -> usize>);
                } else {
                    let generics = func.sig.generics.get_mut().unwrap();
                    for param in &mut generics.params {
                        if let GenericParam::Const(const_param) = param {
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