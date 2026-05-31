use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Const_Generics_In_Structs_347;

impl Mutator for Modify_Const_Generics_In_Structs_347 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Structs_347"
    }
    fn mutate(&self, file: &mut syn::File) {
        let offset_decl: Item = parse_quote! {
            const OFFSET: usize = 5;
        };
        let mut offset_decl_added = false;

        let mut impl_items = Vec::new();
        let mut struct_items = Vec::new();

        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generic_param) = item_struct.generics.params.iter().find(|param| matches!(param, GenericParam::Const(_))) {
                    if let GenericParam::Const(const_param) = generic_param {
                        let const_ident = &const_param.ident;
                        struct_items.push((item_struct.ident.clone(), const_ident.clone()));
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    for (struct_ident, const_ident) in &struct_items {
                        if path.segments.last().map_or(false, |segment| segment.ident == *struct_ident) {
                            impl_items.push((item_impl, const_ident));
                        }
                    }
                }
            }
        }

        for (item_impl, const_ident) in impl_items {
            for impl_item in &mut item_impl.items {
                if let ImplItem::Fn(func) = impl_item {
                    if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                        if let Type::Path(type_path) = &mut **return_type {
                            if let Some(last_segment) = type_path.path.segments.last_mut() {
                                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                    for arg in &mut args.args {
                                        if let GenericArgument::Const(expr) = arg {
                                            if let Expr::Path(expr_path) = expr {
                                                if expr_path.path.is_ident(const_ident) {
                                                    if !offset_decl_added {
                                                        file.items.insert(0, offset_decl.clone());
                                                        offset_decl_added = true;
                                                    }
                                                    *arg = parse_quote! { { #const_ident + OFFSET } };
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generics and modifies functions relying on these generics by introducing an external constant into their calculations. It explores the compiler's handling of const expressions with external dependencies, potentially uncovering issues in const evaluation and error reporting."
    }
}