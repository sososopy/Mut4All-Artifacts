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

pub struct Mutator_Add_Const_Generic_215;

impl Mutator for Mutator_Add_Const_Generic_215 {
    fn name(&self) -> &str {
        "Mutator_Add_Const_Generic_215"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(GenericParam::Const(const_param)) = item_struct.generics.params.first() {
                    let const_ident = &const_param.ident;
                    let struct_ident = &item_struct.ident;
                    
                    let compute_fn: ItemFn = parse_quote! {
                        fn compute_value<const N: usize>() -> usize {
                            N + 1
                        }
                    };

                    let use_fn: ItemFn = parse_quote! {
                        fn use_struct<const N: usize>() {
                            let instance = #struct_ident::<{ compute_value::<N>() }> {};
                        }
                    };

                    file.items.push(Item::Fn(compute_fn));
                    file.items.push(Item::Fn(use_fn));
                }
            }
        }
        
        let main_fn: ItemFn = parse_quote! {
            fn main() {
                use_struct::<10>();
            }
        };
        
        file.items.push(Item::Fn(main_fn));
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies a struct definition with a const generic parameter that has a default value. It introduces a function that performs a non-trivial computation using the const generic parameter and instantiates the struct with this computed value. This stresses the compiler's const generic handling, potentially uncovering issues in type-checking and parameter substitution."
    }
}