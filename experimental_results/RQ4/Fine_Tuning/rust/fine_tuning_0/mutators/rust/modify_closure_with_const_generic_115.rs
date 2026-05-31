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

pub struct Modify_Closure_With_Const_Generic_115;

impl Mutator for Modify_Closure_With_Const_Generic_115 {
    fn name(&self) -> &str {
        "Modify_Closure_With_Const_Generic_115"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = ClosureVisitor;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct ClosureVisitor;

impl VisitMut for ClosureVisitor {
    fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
        if i.constness.is_none() {
            i.constness = Some(token::Const {
                span: Span::call_site(),
            });
        }
        let mut new_inputs = Punctuated::new();
        new_inputs.push(Pat::Ident(PatIdent {
            attrs: Vec::new(),
            by_ref: None,
            mutability: None,
            ident: Ident::new("N", Span::call_site()),
            subpat: None,
        }));
        new_inputs.extend(i.inputs.iter().cloned().map(|pat| pat));
        i.inputs = new_inputs;
    }
}