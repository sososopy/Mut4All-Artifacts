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

pub struct Change_Crate_Type_For_Async_Trait_Definitions_299;

impl Mutator for Change_Crate_Type_For_Async_Trait_Definitions_299 {
    fn name(&self) -> &str {
        "Change_Crate_Type_For_Async_Trait_Definitions_299"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_async_fn_in_trait_feature = false;
        let mut has_async_trait_method = false;
        
        for attr in &file.attrs {
            if let syn::AttrStyle::Inner(_) = attr.style {
                if let syn::Meta::List(list) = &attr.meta {
                    if list.path.is_ident("feature") {
                        for nested in &list.nested {
                            if let syn::Meta::Path(path) = nested {
                                if path.is_ident("async_fn_in_trait") {
                                    has_async_fn_in_trait_feature = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        for item in &file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &trait_item.items {
                    if let syn::TraitItem::Fn(method_fn) = trait_item {
                        if method_fn.sig.asyncness.is_some() {
                            has_async_trait_method = true;
                            break;
                        }
                    }
                }
            }
        }
        
        if !has_async_fn_in_trait_feature || !has_async_trait_method {
            return;
        }
        
        for attr in &mut file.attrs {
            if let syn::AttrStyle::Inner(_) = attr.style {
                if let syn::Meta::NameValue(name_value) = &mut attr.meta {
                    if name_value.path.is_ident("crate_type") {
                        if let syn::Expr::Lit(expr_lit) = &name_value.value {
                            if let syn::Lit::Str(lit_str) = &expr_lit.lit {
                                let crate_type = lit_str.value();
                                let new_crate_type = match crate_type.as_str() {
                                    "lib" => "rlib",
                                    "rlib" => "dylib",
                                    "dylib" => "cdylib",
                                    "cdylib" => "staticlib",
                                    "staticlib" => "lib",
                                    _ => continue,
                                };
                                name_value.value = syn::Expr::Lit(syn::ExprLit {
                                    attrs: Vec::new(),
                                    lit: syn::Lit::Str(syn::LitStr::new(new_crate_type, lit_str.span())),
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies crate_type attributes in Rust files containing async trait definitions with the async_fn_in_trait feature. It changes the crate type according to a defined mapping (lib->rlib, rlib->dylib, dylib->cdylib, cdylib->staticlib, staticlib->lib) to explore different compiler paths during metadata encoding and HIR processing. This transformation aims to uncover edge cases in how async trait closures are handled across different compilation outputs."
    }
}