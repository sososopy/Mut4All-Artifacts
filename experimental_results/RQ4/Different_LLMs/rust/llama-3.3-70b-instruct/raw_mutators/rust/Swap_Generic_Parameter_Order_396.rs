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

pub struct Swap_Generic_Parameter_Order_396;

impl Mutator for Swap_Generic_Parameter_Order_396 {
    fn name(&self) -> &str {
        "Swap_Generic_Parameter_Order_396"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics {
                    let mut params: Vec<GenericParam> = generics.params.clone();
                    if params.len() > 1 {
                        // Swap the order of the first two generic parameters
                        params.swap(0, 1);
                        generics.params = params;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of generic parameters in struct definitions. This transformation tests the Rust compiler's handling of generic parameter order and default const expressions, which is relevant to the F-generic_const_exprs bug label."
    }
}