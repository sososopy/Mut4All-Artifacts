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

pub struct Modify_Const_Generic_With_Lifetime_69;

impl Mutator for Modify_Const_Generic_With_Lifetime_69 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_With_Lifetime_69"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_lifetime = false;
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Lifetime(_) = param {
                        has_lifetime = true;
                        break;
                    }
                }

                if !has_lifetime {
                    continue;
                }

                let mut visitor = ConstGenericVisitor::new();
                visitor.visit_item_fn_mut(item_fn);

                if visitor.modified {
                    let lifetime: Lifetime = parse_quote!('a);
                    item_fn.sig.generics.params.push(GenericParam::Lifetime(LifetimeParam::new(lifetime)));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with const generics. It modifies const expressions used in function calls by introducing a lifetime-bound reference, thereby creating a complex evaluation scenario. This can potentially trigger compiler errors related to const generics and lifetimes."
    }
}

struct ConstGenericVisitor {
    modified: bool,
}

impl ConstGenericVisitor {
    fn new() -> Self {
        Self { modified: false }
    }
}

impl VisitMut for ConstGenericVisitor {
    fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
        if let Some(GenericArgument::Const(expr)) = expr_call.args.first_mut() {
            *expr = parse_quote!({
                let _: &'a ();
                #expr
            });
            self.modified = true;
        }
        syn::visit_mut::visit_expr_call_mut(self, expr_call);
    }
}