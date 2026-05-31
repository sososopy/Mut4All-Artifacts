use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Expr, ExprArray, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct ConstantEvaluatorArraySizeMutation_77;

impl Mutator for ConstantEvaluatorArraySizeMutation_77 {
    fn name(&self) -> &str {
        "ConstantEvaluatorArraySizeMutation_77"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Expr::Array(array) = &*const_item.expr {
                    let mut new_array = array.clone();
                    let mut rng = thread_rng();
                    let new_size = rng.gen_range(1..1000);
                    let new_elems: Punctuated<Expr, Comma> = (0..new_size).map(|_| parse_quote!(0)).collect();
                    new_array.elems = new_elems;
                    const_item.expr = Box::new(Expr::Array(new_array));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions that involve array sizes in the Rust code. It modifies the size of the array in the constant expression to a different value, which could be a smaller or larger size, to see if the compiler correctly handles the change."
    }
}