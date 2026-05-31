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

pub struct Replace_Constant_Expression_426;

impl Mutator for Replace_Constant_Expression_426 {
    fn name(&self) -> &str {
        "Replace_Constant_Expression_426"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for statement in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = statement {
                        if let Expr::Lit(lit) = &**expr {
                            let new_lit = match lit {
                                Lit::Int(int) => {
                                    let new_value = thread_rng().gen_range(0..100);
                                    Lit::Int(LitInt::new(new_value.to_string(), int.base10_digits()))
                                }
                                Lit::Float(float) => {
                                    let new_value = thread_rng().gen_range(0.0..100.0);
                                    Lit::Float(LitFloat::new(new_value.to_string()))
                                }
                                Lit::Char(char) => {
                                    let new_value = thread_rng().gen_range('a'..='z');
                                    Lit::Char(LitChar::new(new_value, char.suffix()))
                                }
                                _ => continue,
                            };
                            *expr = parse_quote!(#new_lit);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant expressions in the code with new constant expressions of the same type but different values. This transformation aims to test the compiler's handling of constant expressions and may lead to bugs in the constant evaluation system or type checking."
    }
}