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

pub struct Alter_Closure_Structure_71;

impl Mutator for Alter_Closure_Structure_71 {
    fn name(&self) -> &str {
        "Alter_Closure_Structure_71"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureVisitor;

        impl VisitMut for ClosureVisitor {
            fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
                if let Expr::Block(expr_block) = &mut *i.body {
                    if expr_block.block.stmts.iter().any(|stmt| {
                        if let Stmt::Expr(Expr::Unsafe(_)) = stmt {
                            true
                        } else {
                            false
                        }
                    }) {
                        let unsafe_block = expr_block.block.stmts.remove(0);
                        let closure_body = i.body.clone();
                        i.body = Box::new(parse_quote!({ #closure_body }));
                        if let Expr::Block(ref mut new_block) = *i.body {
                            new_block.block.stmts.insert(0, unsafe_block);
                        }
                    }
                }
                syn::visit_mut::visit_expr_closure_mut(self, i);
            }
        }

        let mut visitor = ClosureVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies closures containing unsafe blocks and alters their structure by moving the unsafe block outside the closure's body. This involves restructuring the closure to enclose its body with additional braces and repositioning the unsafe block to precede the closure's body, potentially leading to incorrect syntax handling by the compiler."
    }
}