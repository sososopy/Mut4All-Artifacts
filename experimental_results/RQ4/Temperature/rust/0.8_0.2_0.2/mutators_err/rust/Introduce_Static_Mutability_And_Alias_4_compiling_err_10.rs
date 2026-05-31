use proc_macro2::Span;
use quote::*;
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
    *,
};

use crate::mutator::Mutator;

pub struct Introduce_Static_Mutability_And_Alias_4;

impl Mutator for Introduce_Static_Mutability_And_Alias_4 {
    fn name(&self) -> &str {
        "Introduce_Static_Mutability_And_Alias_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Static(ref mut static_item) = item {
                if static_item.mutability.is_none() {
                    // Convert the static variable to mutable
                    static_item.mutability = Some(token::Mut { span: Span::call_site() });
                    
                    // Create a new static alias for the mutable static
                    let alias_ident = Ident::new(&format!("{}_ALIAS", static_item.ident), Span::call_site());
                    let alias_static: Item = parse_quote! {
                        pub static #alias_ident: &'static #static_item.ty = unsafe { &*#static_item.ident };
                    };
                    
                    // Add the alias to the new items
                    new_items.push(alias_static);
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static variable declarations that are immutable. It converts them to mutable and introduces a new static alias that references the mutable static. This transformation is designed to test the handling of mutable statics and their references, potentially exposing issues related to const evaluation and layout."
    }
}