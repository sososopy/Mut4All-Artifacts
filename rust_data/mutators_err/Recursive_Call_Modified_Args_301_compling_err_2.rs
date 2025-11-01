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

pub struct Recursive_Call_Modified_Args_301;

impl Mutator for Recursive_Call_Modified_Args_301 {
    fn name(&self) -> &str {
        "Recursive_Call_Modified_Args_301"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    let mut rng = thread_rng();
                    let array_ident = Ident::new("arr", Span::call_site());
                    let index_ident = Ident::new("i", Span::call_site());

                    // Insert a vector declaration at the beginning of the function
                    let vec_stmt: Stmt = parse_quote! {
                        let #array_ident = vec![1, 2, 3];
                    };
                    func.block.stmts.insert(0, vec_stmt);

                    // Create a recursive call with modified arguments
                    let recursive_call: Stmt = parse_quote! {
                        #array_ident[#index_ident];
                        #func.sig.ident(#array_ident[#index_ident]);
                    };

                    // Insert the recursive call at the end of the function
                    func.block.stmts.push(recursive_call);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies the main function and introduces a vector declaration at the start. It then appends a recursive call to the same function with a modified argument using an index that is either undefined or potentially out-of-bounds. This mutation aims to exploit recursive call patterns and indexing issues that may reveal compiler handling flaws."
    }
}