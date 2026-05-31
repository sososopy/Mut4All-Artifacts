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

pub struct Modify_RefCell_Borrow_Scope_126;

impl Mutator for Modify_RefCell_Borrow_Scope_126 {
    fn name(&self) -> &str {
        "Modify_RefCell_Borrow_Scope_126"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct RefCellVisitor;

        impl VisitMut for RefCellVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::MethodCall(method_call) = expr {
                    if method_call.method == "borrow_mut" {
                        method_call.method = Ident::new("borrow", method_call.method.span());
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = RefCellVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets RefCell usage by replacing borrow_mut() with borrow(), enforcing an immutable borrow where mutable was expected. This transformation tests the borrow checker and can lead to borrow conflicts, particularly under NLL (Non-Lexical Lifetimes) rules."
    }
}