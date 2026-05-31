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

pub struct Replace_Crate_References_214;

impl Mutator for Replace_Crate_References_214 {
    fn name(&self) -> &str {
        "Replace_Crate_References_214"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut new_crate_name = String::new();
        let mut rng = thread_rng();
        for _ in 0..10 {
            new_crate_name.push(rng.gen_range('a'..='z'));
        }

        for item in &mut file.items {
            if let Item::Use(use_stmt) = item {
                for path in &mut use_stmt.path.segments {
                    path.ident = Ident::new(&new_crate_name, path.ident.span());
                }
            } else if let Item::ExternCrate(extern_crate) = item {
                extern_crate.ident = Ident::new(&new_crate_name, extern_crate.ident.span());
            }
        }

        // Add a new extern crate declaration for the new crate
        file.items.push(Item::ExternCrate(ExternCrate {
            attrs: Vec::new(),
            vis: Visibility::Inherited,
            extern_token: token::Extern { span: Span::call_site() },
            crate_token: token::Crate { span: Span::call_site() },
            ident: Ident::new(&new_crate_name, Span::call_site()),
            rename: None,
            span: Span::call_site(),
        }));
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces existing crate references with new ones to test the compiler's ability to resolve crate metadata and handle crate references correctly. It introduces new crate references in use statements and extern crate declarations, and adds a new extern crate declaration for the new crate."
    }
}