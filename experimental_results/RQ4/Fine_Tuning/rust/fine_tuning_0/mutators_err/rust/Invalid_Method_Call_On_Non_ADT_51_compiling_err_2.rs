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

pub struct Invalid_Method_Call_On_Non_ADT_51;

impl Mutator for Invalid_Method_Call_On_Non_ADT_51 {
    fn name(&self) -> &str {
        "Invalid_Method_Call_On_Non_ADT_51"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Local(local) = item {
                if let Some((_, expr)) = &local.init {
                    if let Expr::Array(_) | Expr::Tuple(_) = **expr {
                        let new_expr = Expr::MethodCall(syn::ExprMethodCall {
                            attrs: vec![],
                            receiver: Box::new(*expr.clone()),
                            dot_token: token::Dot::default(),
                            method: Ident::new("invalid_method", Span::call_site()),
                            turbofish: None,
                            paren_token: Paren::default(),
                            args: Punctuated::new(),
                        });
                        local.init = Some((token::Eq::default(), Box::new(new_expr)));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets array and tuple expressions, appending an invalid method call to them. The goal is to introduce deliberate errors by invoking non-existent methods, testing the compiler's error handling and diagnostic capabilities for unsupported operations on non-ADT constructs."
    }
}