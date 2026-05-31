use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStatic, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Modify_Static_Interior_Mutability_4;

impl Mutator for Modify_Static_Interior_Mutability_4 {
    fn name(&self) -> &str {
        "Modify_Static_Interior_Mutability_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Static(item_static) = item {
                if let Type::Path(TypePath { path, .. }) = &*item_static.ty {
                    if let Some(last_segment) = path.segments.last() {
                        if last_segment.ident != "OnceLock" && last_segment.ident != "Mutex" && last_segment.ident != "RwLock" {
                            let new_ident = Ident::new(&format!("{}_WRAPPER", item_static.ident), Span::call_site());
                            let new_static: ItemStatic = parse_quote! {
                                static #new_ident: std::sync::OnceLock<&#path> = std::sync::OnceLock::new();
                            };
                            new_items.push(Item::Static(new_static));
                            
                            let init_fn_ident = Ident::new(&format!("initialize_{}", item_static.ident), Span::call_site());
                            let init_fn: ItemFn = parse_quote! {
                                fn #init_fn_ident() {
                                    #new_ident.get_or_init(|| &#item_static.ident);
                                }
                            };
                            new_items.push(Item::Fn(init_fn));

                            item_static.expr = Box::new(parse_quote! {
                                #path { other_interfaces: &[], c_ptr: Some(unsafe { #new_ident.get().unwrap() }) }
                            });
                        }
                    }
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies static variables that reference other static variables without interior mutability. It introduces a new static variable using `OnceLock` to wrap the original reference, then updates the original static variable to use this new wrapper. This transformation aims to expose bugs in the Rust compiler's handling of interior mutability in constant contexts."
    }
}