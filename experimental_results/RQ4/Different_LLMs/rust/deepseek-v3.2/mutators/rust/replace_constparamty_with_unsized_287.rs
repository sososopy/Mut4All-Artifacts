use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_ConstParamTy_With_Unsized_287;

impl Mutator for Replace_ConstParamTy_With_Unsized_287 {
    fn name(&self) -> &str {
        "Replace_ConstParamTy_With_Unsized_287"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let unsized_types: Vec<syn::Type> = vec![
            parse_quote!(str),
            parse_quote!([u8]),
            parse_quote!([i32]),
            parse_quote!([bool]),
            parse_quote!([char]),
        ];
        
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    if item_fn.sig.ident == "main" {
                        continue;
                    }
                    for param in &mut item_fn.sig.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(type_path) = &const_param.ty {
                                if is_adt_type(&type_path.path) {
                                    let new_type = unsized_types.choose(&mut rng).unwrap().clone();
                                    const_param.ty = new_type;
                                }
                            }
                        }
                    }
                }
                Item::Impl(item_impl) => {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            for param in &mut method.sig.generics.params {
                                if let GenericParam::Const(const_param) = param {
                                    if let Type::Path(type_path) = &const_param.ty {
                                        if is_adt_type(&type_path.path) {
                                            let new_type = unsized_types.choose(&mut rng).unwrap().clone();
                                            const_param.ty = new_type;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters in functions and trait implementations where the type is an ADT (struct/enum) that likely implements ConstParamTy. It replaces these types with unsized types like str or slices (e.g., [u8], [i32]). This transformation aims to stress the compiler's handling of unsized types as const generic parameters, which is an edge case known to potentially trigger ICEs in symbol mangling or coverage instrumentation. By introducing unsized types directly as const parameters, it tests the compiler's ability to manage const generics with dynamically sized types."
    }
}

fn is_adt_type(path: &syn::Path) -> bool {
    let ident_str = path.segments.last().map(|seg| seg.ident.to_string()).unwrap_or_default();
    !ident_str.is_empty() && 
    !matches!(ident_str.as_str(), 
        "usize" | "isize" | "bool" | "char" | "str" | "u8" | "i32" | "bool" | "char" |
        "i8" | "i16" | "i64" | "i128" | "u16" | "u32" | "u64" | "u128" | "f32" | "f64")
}