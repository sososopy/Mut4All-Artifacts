use syn::{parse_quote, FnArg, Type, TypePath, PathArguments, GenericArgument, Path, PathSegment, ImplItem, Ident};
use rand::thread_rng;
use rand::Rng;
use rand::prelude::SliceRandom;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}

struct Replace_Const_Value_Mutator_404;

impl Mutator for Replace_Const_Value_Mutator_404 {
    fn name(&self) -> &str {
        "Replace_Const_Value_Mutator_404"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(TypePath {
                            qself: None,
                            path,
                        }) = &*pat_type.ty
                        {
                            if let Some(segment) = path.segments.iter().next() {
                                if let Some(const_value) = get_const_value(segment) {
                                    let new_value = get_new_value(const_value);
                                    pat_type.ty = Box::new(parse_quote! { #new_value });
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for arg in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path,
                                }) = &*pat_type.ty
                                {
                                    if let Some(segment) = path.segments.iter().next() {
                                        if let Some(const_value) = get_const_value(segment) {
                                            let new_value = get_new_value(const_value);
                                            pat_type.ty = Box::new(parse_quote! { #new_value });
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant values in generic const expressions with values of different types. This transformation aims to test the compiler's handling of type mismatches and const evaluation under altered conditions."
    }
}

fn get_const_value(segment: &PathSegment) -> Option<syn::Expr> {
    if let PathArguments::AngleBracketed(args) = &segment.arguments {
        if let Some(arg) = args.args.iter().next() {
            if let GenericArgument::Type(ty) = arg {
                if let Type::Path(TypePath {
                    qself: None,
                    path,
                }) = ty
                {
                    if let Some(segment) = path.segments.iter().next() {
                        if let Ident(ref ident) = &segment.ident {
                            if ident.to_string() == "u8" || ident.to_string() == "i32" || ident.to_string() == "bool" {
                                return Some(parse_quote! { 0 });
                            }
                        }
                    }
                }
            }
        }
    }
    None
}

fn get_new_value(const_value: syn::Expr) -> syn::Expr {
    let mut rng = thread_rng();
    let choices: Vec<syn::Expr> = vec![parse_quote! { true }, parse_quote! { false }, parse_quote! { 0u8 }, parse_quote! { 0i32 }];
    choices.choose(&mut rng).unwrap().clone()
}