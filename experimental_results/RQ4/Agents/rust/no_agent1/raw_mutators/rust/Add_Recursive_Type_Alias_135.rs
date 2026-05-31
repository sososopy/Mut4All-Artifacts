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

pub struct Add_Recursive_Type_Alias_135;

impl Mutator for Add_Recursive_Type_Alias_135 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_135"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let alias_name = Ident::new("RecursiveAlias", Span::call_site());

        let type_alias: Item = parse_quote! {
            type #alias_name = Option<#alias_name>;
        };

        if rng.gen_bool(0.5) {
            file.items.push(type_alias);
        } else {
            for item in &mut file.items {
                if let Item::Fn(func) = item {
                    if func.sig.ident != "main" {
                        let stmt: Stmt = parse_quote! {
                            let _: #alias_name = None;
                        };
                        func.block.stmts.insert(0, stmt);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveAlias = Option<RecursiveAlias>;` into the code. This recursive type definition can lead to infinite type expansion or stack overflow errors during type checking and inference, potentially triggering ICEs or other deep compiler bugs. It tests the compiler's handling of recursive types and its ability to manage potentially unbounded type recursion."
    }
}