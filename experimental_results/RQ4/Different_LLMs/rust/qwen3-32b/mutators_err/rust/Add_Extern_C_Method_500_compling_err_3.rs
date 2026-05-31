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

pub struct Add_Extern_C_Method_500;

impl Mutator for Add_Extern_C_Method_500 {
    fn name(&self) -> &str {
        "Add_Extern_C_Method_500"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let new_method = parse_quote! {
                    extern "C" fn mutated_extern_c_method(&self) -> i32 {
                        0
                    }
                };
                trait_item.items.push(syn::TraitItem::Method(syn::TraitItemMethod {
                    attrs: Vec::new(),
                    sig: new_method.sig,
                    default: None,
                }));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}