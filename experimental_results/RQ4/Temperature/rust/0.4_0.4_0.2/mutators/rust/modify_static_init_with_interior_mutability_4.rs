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

pub struct Modify_Static_Init_With_Interior_Mutability_4;

impl Mutator for Modify_Static_Init_With_Interior_Mutability_4 {
    fn name(&self) -> &str {
        "Modify_Static_Init_With_Interior_Mutability_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                if let syn::Type::Path(type_path) = &*static_item.ty {
                    if let Some(segment) = type_path.path.segments.first() {
                        if segment.ident == "OnceLock" || segment.ident == "Mutex" || segment.ident == "RwLock" {
                            let new_static_ident = Ident::new(&format!("NEW_{}", static_item.ident), Span::call_site());
                            let new_static_item: syn::Item = parse_quote! {
                                static #new_static_ident: #type_path = #type_path::new();
                            };
                            let ref_static_ident = Ident::new(&format!("{}_REF", static_item.ident), Span::call_site());
                            let ref_static_item: syn::Item = parse_quote! {
                                static #ref_static_ident: &[&#type_path] = &[&#new_static_ident, &#static_item.ident];
                            };
                            new_items.push(new_static_item);
                            new_items.push(ref_static_item);
                        }
                    }
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}