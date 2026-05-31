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

pub struct Replace_Const_Static_With_Dyn_Str_52;

impl Mutator for Replace_Const_Static_With_Dyn_Str_52 {
    fn name(&self) -> &str {
        "Replace_Const_Static_With_Dyn_Str_52"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits_in_scope = HashSet::new();
        let mut has_debug = false;
        for item in &file.items {
            if let Item::Use(use_item) = item {
                if let UseTree::Path(use_path) = &use_item.tree {
                    if let Some(ident) = use_path.ident.clone() {
                        traits_in_scope.insert(ident.to_string());
                    }
                }
            }
            if let Item::Trait(trait_item) = item {
                traits_in_scope.insert(trait_item.ident.to_string());
            }
        }
        if traits_in_scope.contains("Debug") {
            has_debug = true;
        } else {
            file.items.insert(0, parse_quote! { use std::fmt::Debug; });
            has_debug = true;
        }

        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                let expr = &mut const_item.expr;
                let original_type = &const_item.ty;
                let is_string_type = match &**original_type {
                    Type::Path(type_path) => {
                        let last_segment = type_path.path.segments.last();
                        last_segment.map(|seg| seg.ident == "str" || seg.ident == "String").unwrap_or(false)
                    }
                    Type::Reference(type_ref) => match &*type_ref.elem {
                        Type::Path(type_path) => {
                            let last_segment = type_path.path.segments.last();
                            last_segment.map(|seg| seg.ident == "str").unwrap_or(false)
                        }
                        _ => false,
                    },
                    _ => false,
                };

                if is_string_type {
                    const_item.ty = parse_quote! { str };
                    *expr = parse_quote! { *"a" };
                } else {
                    const_item.ty = parse_quote! { dyn Debug + 'static };
                    *expr = parse_quote! { *() };
                }
            }
            if let Item::Static(static_item) = item {
                let expr = &mut static_item.expr;
                let original_type = &static_item.ty;
                let is_string_type = match &**original_type {
                    Type::Path(type_path) => {
                        let last_segment = type_path.path.segments.last();
                        last_segment.map(|seg| seg.ident == "str" || seg.ident == "String").unwrap_or(false)
                    }
                    Type::Reference(type_ref) => match &*type_ref.elem {
                        Type::Path(type_path) => {
                            let last_segment = type_path.path.segments.last();
                            last_segment.map(|seg| seg.ident == "str").unwrap_or(false)
                        }
                        _ => false,
                    },
                    _ => false,
                };

                if is_string_type {
                    static_item.ty = parse_quote! { str };
                    *expr = parse_quote! { *"a" };
                } else {
                    static_item.ty = parse_quote! { dyn Debug + 'static };
                    *expr = parse_quote! { *() };
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const and static items with explicit type annotations and initializers. It replaces the type annotation with either a dynamic trait object (dyn Debug + 'static) or an unsized str type, and replaces the initializer expression with a dereference of a unit value (*()) or a string literal (*\"a\"). This creates type mismatches between unsized types and dereferenced expressions, aiming to trigger compiler confusion during const-evaluation, MIR generation, or borrow checking phases. The operator ensures Debug trait is in scope by adding a use statement if needed."
    }
}