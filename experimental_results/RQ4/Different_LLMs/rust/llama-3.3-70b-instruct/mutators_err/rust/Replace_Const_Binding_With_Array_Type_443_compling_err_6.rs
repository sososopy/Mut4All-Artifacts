use syn::{parse_quote, GenericParam, Type};
use crate::mutator::Mutator;

pub struct Replace_Const_Binding_With_Array_Type_443;

impl Mutator for Replace_Const_Binding_With_Array_Type_443 {
    fn name(&self) -> &str {
        "Replace_Const_Binding_With_Array_Type_443"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.type_params_mut() {
                    if let GenericParam::Const(param) = param {
                        if param.default.is_some() {
                            if let Type::Array(array_type) = &param.ty {
                                let new_type = parse_quote!([(); #param.default]);
                                param.ty = new_type;
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for param in &mut impl_item.generics.type_params_mut() {
                    if let GenericParam::Const(param) = param {
                        if param.default.is_some() {
                            if let Type::Array(array_type) = &param.ty {
                                let new_type = parse_quote!([(); #param.default]);
                                param.ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic functions or traits with const bindings and replaces the const binding with an array type. This transformation aims to test the compiler's handling of const generics and array types, potentially triggering type mismatches or inference failures."
    }
}