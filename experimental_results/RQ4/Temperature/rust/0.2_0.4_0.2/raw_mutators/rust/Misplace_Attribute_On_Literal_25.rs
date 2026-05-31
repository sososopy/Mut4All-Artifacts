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

pub struct Misplace_Attribute_On_Literal_25;

impl Mutator for Misplace_Attribute_On_Literal_25 {
    fn name(&self) -> &str {
        "Misplace_Attribute_On_Literal_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &mut local.init {
                            if let Expr::Lit(expr_lit) = expr.as_mut() {
                                let attr: Attribute = parse_quote!(#[misplaced_attribute]);
                                expr_lit.attrs.push(attr);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets literals within local variable initializations and applies an attribute to them. This transformation is intended to test the compiler's handling of attributes in unexpected locations, potentially exposing bugs in attribute parsing and validation logic."
    }
}