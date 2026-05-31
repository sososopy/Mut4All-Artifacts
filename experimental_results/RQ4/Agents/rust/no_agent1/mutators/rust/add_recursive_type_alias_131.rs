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

pub struct Add_Recursive_Type_Alias_131;

impl Mutator for Add_Recursive_Type_Alias_131 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_131"
    }
    fn mutate(&self, file: &mut syn::File) {
        let recursive_type_alias: Item = parse_quote! {
            type Recursive = Box<Recursive>;
        };

        file.items.push(recursive_type_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let recursive_var: Stmt = parse_quote! {
                    let _recursive_instance: Recursive = Box::new(0 as Recursive);
                };
                func.block.stmts.insert(0, recursive_var);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation introduces a recursive type alias `type Recursive = Box<Recursive>;` and attempts to instantiate it within non-main functions. This creates a potentially infinite type definition, challenging the compiler's ability to handle recursive types and type alias resolution, which may lead to ICEs or stack overflows during type checking or code generation."
    }
}