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

pub struct Modify_Async_Yield_Interaction_124;

impl Mutator for Modify_Async_Yield_Interaction_124 {
    fn name(&self) -> &str {
        "Modify_Async_Yield_Interaction_124"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if func.sig.asyncness.is_some() || func.sig.ident.to_string().contains("yield") {
                    // Modify return type with a lifetime specifier
                    if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                        *ty = Box::new(parse_quote!(impl Future<Output = &'static str>));
                    }

                    // Alter await expressions to yield expressions
                    let mut visitor = AwaitToYieldVisitor;
                    visitor.visit_block_mut(&mut func.block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions using async/await or yield constructs. It modifies the return type to include a static lifetime and changes await expressions to yield expressions, introducing type mismatches and lifetime issues. This aims to test the compiler's handling of async and generator interactions, potentially exposing ICEs related to type inference and lifetime management."
    }
}

struct AwaitToYieldVisitor;

impl VisitMut for AwaitToYieldVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Await(expr_await) = expr {
            let base_expr = expr_await.base.clone();
            *expr = parse_quote!(yield #base_expr);
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}