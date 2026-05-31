use rand::prelude::SliceRandom;
use syn::parse_quote;
use syn::GenericParam;
use syn::Type;
use syn::FnArg;
use syn::ReturnType;
use rand::thread_rng;
use crate::mutator::Mutator;

pub struct Replace_Type_With_Const_Param_In_Struct_Impl_403;

impl Mutator for Replace_Type_With_Const_Param_In_Struct_Impl_403 {
    fn name(&self) -> &str {
        "Replace_Type_With_Const_Param_In_Struct_Impl_403"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let has_type_params = item_struct.generics.params.iter().any(|p| matches!(p, GenericParam::Type(_)));
                let has_const_params = item_struct.generics.params.iter().any(|p| matches!(p, GenericParam::Const(_)));
                if has_type_params && has_const_params {
                    let const_params: Vec<_> = item_struct.generics.params.iter()
                        .filter_map(|p| match p {
                            GenericParam::Const(c) => Some(&c.ident),
                            _ => None,
                        })
                        .collect();
                    if const_params.is_empty() {
                        continue;
                    }
                    if let syn::Fields::Named(named) = &mut item_struct.fields {
                        for field in &mut named.named {
                            let const_name = const_params.choose(&mut thread_rng()).unwrap();
                            let new_type = parse_quote! { #const_name };
                            field.ty = new_type;
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                let has_type_params = item_impl.generics.params.iter().any(|p| matches!(p, GenericParam::Type(_)));
                let has_const_params = item_impl.generics.params.iter().any(|p| matches!(p, GenericParam::Const(_)));
                if has_type_params && has_const_params {
                    let const_params: Vec<_> = item_impl.generics.params.iter()
                        .filter_map(|p| match p {
                            GenericParam::Const(c) => Some(&c.ident),
                            _ => None,
                        })
                        .collect();
                    if const_params.is_empty() {
                        continue;
                    }
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            for input in &mut method.sig.inputs {
                                if let FnArg::Typed(pat_ty) = input {
                                    let const_name = const_params.choose(&mut thread_rng()).unwrap();
                                    let new_type = parse_quote! { #const_name };
                                    pat_ty.ty = new_type;
                                }
                            }
                            if let ReturnType::Type(_, ty) = &mut method.sig.output {
                                let const_name = const_params.choose(&mut thread_rng()).unwrap();
                                let new_type = parse_quote! { #const_name };
                                *ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator replaces type parameters in struct fields and impl blocks with const parameters from the same struct definition. It targets structs with both type and const parameters, substituting const parameters into type positions to create invalid but syntactically valid ASTs. This exploits compiler logic for generic parameter substitution, potentially exposing bugs in const parameter handling and type resolution systems."
    }
}