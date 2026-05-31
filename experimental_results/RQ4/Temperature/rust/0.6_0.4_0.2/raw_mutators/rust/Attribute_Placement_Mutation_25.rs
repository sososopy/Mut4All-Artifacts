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

pub struct Attribute_Placement_Mutation_25;

impl Mutator for Attribute_Placement_Mutation_25 {
    fn name(&self) -> &str {
        "Attribute_Placement_Mutation_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_attrs = Vec::new();
                func.block.stmts.retain(|stmt| {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &local.init {
                            if let Expr::Group(expr_group) = &**expr {
                                if !expr_group.attrs.is_empty() {
                                    new_attrs.extend(expr_group.attrs.clone());
                                    return false;
                                }
                            }
                        }
                    }
                    true
                });
                func.attrs.extend(new_attrs);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets attributes applied directly to expressions within function bodies, relocating them to the function level. By doing so, it tests the parser's ability to handle attributes when they are moved to different syntactic constructs, potentially uncovering issues in attribute handling logic."
    }
}