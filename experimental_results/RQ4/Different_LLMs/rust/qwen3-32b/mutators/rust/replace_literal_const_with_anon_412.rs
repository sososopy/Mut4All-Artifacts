use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_Literal_Const_With_Anon_412;

impl Mutator for Replace_Literal_Const_With_Anon_412 {
    fn name(&self) -> &str {
        "Replace_Literal_Const_With_Anon_412"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            MutatorVisitor.visit_item_mut(item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inherent associated types with literal const parameters. It replaces the literal with an anon const expression (e.g., {5 + 1 - 1}), preserving the evaluated value but introducing complex const evaluation paths. This stresses the compiler's handling of anon const expressions in generic parameters, testing const evaluation, type resolution, and potential ICEs in const context propagation."
    }
}

struct MutatorVisitor;

impl MutatorVisitor {
    fn process_path_arguments(&self, args: &mut PathArguments) {
        if let PathArguments::AngleBracketed(angle_args) = args {
            for arg in &mut angle_args.args {
                if let GenericArgument::Const(expr) = arg {
                    if let Expr::Lit(_) = *expr {
                        let original_expr = expr.clone();
                        let new_expr: Box<Expr> = parse_quote! { #original_expr + 1 - 1 };
                        let expr_block = parse_quote! { { #new_expr } };
                        *arg = GenericArgument::Const(expr_block);
                    }
                }
            }
        }
    }
}

impl VisitMut for MutatorVisitor {
    fn visit_expr_call_mut(&mut self, expr_call: &mut ExprCall) {
        visit_mut::visit_expr_call_mut(self, expr_call);
    }

    fn visit_type_path_mut(&mut self, type_path: &mut TypePath) {
        for segment in type_path.path.segments.iter_mut() {
            self.process_path_arguments(&mut segment.arguments);
        }
        visit_mut::visit_type_path_mut(self, type_path);
    }

    fn visit_path_mut(&mut self, path: &mut SynPath) {
        for segment in path.segments.iter_mut() {
            self.process_path_arguments(&mut segment.arguments);
        }
        visit_mut::visit_path_mut(self, path);
    }

    fn visit_expr_path_mut(&mut self, expr_path: &mut ExprPath) {
        for segment in expr_path.path.segments.iter_mut() {
            self.process_path_arguments(&mut segment.arguments);
        }
        visit_mut::visit_expr_path_mut(self, expr_path);
    }
}