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

pub struct Introduce_Closure_Reference_587;

impl Mutator for Introduce_Closure_Reference_587 {
    fn name(&self) -> &str {
        "Introduce_Closure_Reference_587"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ClosureVisitor { modified: false };
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct ClosureVisitor {
    modified: bool,
}

impl VisitMut for ClosureVisitor {
    fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
        if !self.modified {
            let mut has_reference = false;
            for input in &mut i.inputs {
                if let FnArg::Typed(pat_type) = input {
                    if let Type::Reference(_) = *pat_type.ty {
                        has_reference = true;
                        break;
                    }
                }
            }

            if !has_reference {
                if let Some(FnArg::Typed(pat_type)) = i.inputs.first_mut() {
                    let new_type: Type = parse_quote!(&i32);
                    pat_type.ty = Box::new(new_type);
                    self.modified = true;
                }
            }

            if has_reference || self.modified {
                if let Some(FnArg::Typed(pat_type)) = i.inputs.first() {
                    let ref_usage: Expr = parse_quote! {
                        println!("{}", #pat_type.pat);
                    };
                    if let Expr::Block(ref mut block) = *i.body {
                        block.block.stmts.insert(0, Stmt::Expr(ref_usage));
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_closure_mut(self, i);
    }
}