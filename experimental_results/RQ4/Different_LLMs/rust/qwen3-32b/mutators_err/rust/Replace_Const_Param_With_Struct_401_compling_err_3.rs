use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Param_With_Struct_401;

impl Mutator for Replace_Const_Param_With_Struct_401 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Struct_401"
    }
    fn mutate(&self, file: &mut syn::File) {
        let struct_name = "Dimension";
        let struct_exists = file.items.iter().any(|item| {
            if let Item::Struct(s) = item {
                s.ident.to_string() == struct_name
            } else {
                false
            }
        });

        if !struct_exists {
            let new_struct = parse_quote! {
                pub struct #struct_name;
            };
            file.items.insert(0, Item::Struct(new_struct));
        }

        fn is_primitive_type(ty: &Type) -> bool {
            if let Type::Path(type_path) = ty {
                if let Some(segment) = type_path.path.segments.last() {
                    match segment.ident.to_string().as_str() {
                        "u8" | "i32" | "bool" | "char" | "usize" | "isize" => return true,
                        _ => {}
                    }
                }
            }
            false
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if is_primitive_type(&const_param.ty) {
                            const_param.ty = parse_quote! { Dimension };
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for param in &mut item_impl.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if is_primitive_type(&const_param.ty) {
                            const_param.ty = parse_quote! { Dimension };
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}