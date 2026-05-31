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

pub struct Add_Recursive_Type_Alias_35;

impl Mutator for Add_Recursive_Type_Alias_35 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        let type_alias: Item = parse_quote! {
            type Recursive = Box<Recursive>;
        };

        file.items.push(type_alias);

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let stmt: Stmt = parse_quote! {
                        let _ = Recursive;
                    };
                    func.block.stmts.insert(0, stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type Recursive = Box<Recursive>;` and injects a usage of this type into non-main functions. This transformation aims to provoke the compiler's handling of recursive types, potentially triggering infinite recursion in type resolution or other deep compiler bugs related to type aliasing."
    }
}