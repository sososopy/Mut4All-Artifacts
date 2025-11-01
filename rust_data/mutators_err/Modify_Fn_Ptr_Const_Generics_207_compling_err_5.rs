use crate::mutator::Mutator;
use syn::{parse_quote, File, GenericArgument, GenericParam, Item, PathArguments, Type, TypePath};

pub struct Modify_Fn_Ptr_Const_Generics_207;

impl Mutator for Modify_Fn_Ptr_Const_Generics_207 {
    fn name(&self) -> &str {
        "Modify_Fn_Ptr_Const_Generics_207"
    }
    
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                if let Some(GenericParam::Const(const_param)) = generics.params.first_mut() {
                    if let Type::Path(TypePath { path, .. }) = &const_param.ty {
                        if path.segments.iter().any(|seg| seg.ident == "fn") {
                            const_param.ty = Type::Path(TypePath {
                                path: parse_quote!(usize),
                                qself: None,
                            });
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Type::Path(TypePath { path, .. }) = &*item_impl.self_ty {
                    if let Some(segment) = path.segments.last() {
                        if segment.ident == "Wrapper" {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                if let Some(GenericArgument::Const(expr)) = args.args.first_mut() {
                                    *expr = parse_quote!(4);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with const generic parameters using function pointers. It modifies the generic parameter from a function pointer to a numeric constant. This involves two main steps: first, changing the struct definition to replace the function pointer with a numeric constant type (usize); second, updating the implementation to replace any usage of the function pointer with a numeric constant value. This mutation helps explore the handling of const generics in the Rust compiler by leveraging existing constructs in the code."
    }
}