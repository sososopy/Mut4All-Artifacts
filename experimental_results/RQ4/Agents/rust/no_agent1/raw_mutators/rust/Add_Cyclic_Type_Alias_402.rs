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

pub struct Add_Cyclic_Type_Alias_402;

impl Mutator for Add_Cyclic_Type_Alias_402 {
    fn name(&self) -> &str {
        "Add_Cyclic_Type_Alias_402"
    }
    fn mutate(&self, file: &mut syn::File) {
        let type_alias: Item = parse_quote! {
            type Cyclic = Box<Cyclic>;
        };
        
        file.items.push(type_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let stmt: Stmt = parse_quote! {
                        let _cyclic: Cyclic = Box::new(Box::new(Box::new(Box::new(Cyclic))));
                    };
                    func.block.stmts.insert(0, stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a cyclic type alias `type Cyclic = Box<Cyclic>;` into the code. It then inserts a statement in non-main functions that attempts to instantiate this cyclic type. This transformation is designed to test the compiler's handling of recursive type definitions and instantiations, potentially leading to infinite type expansion, stack overflows, or ICEs."
    }
}