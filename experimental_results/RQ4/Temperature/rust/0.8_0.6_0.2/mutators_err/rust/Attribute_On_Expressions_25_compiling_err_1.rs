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

pub struct Attribute_On_Expressions_25;

impl Mutator for Attribute_On_Expressions_25 {
    fn name(&self) -> &str {
        "Attribute_On_Expressions_25"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, expr)) = &local.init {
                            let attr: Attribute = parse_quote!(#[unstable_feature]);
                            let new_expr = Expr::Verbatim(quote! {
                                #attr
                                #expr
                            });
                            local.init = Some((token::Eq::default(), Box::new(new_expr)));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an unstable attribute directly onto expressions within local variable initializations. This transformation attempts to expose potential parsing issues or internal compiler errors by using attributes in non-standard locations, challenging the compiler's attribute handling mechanisms."
    }
}