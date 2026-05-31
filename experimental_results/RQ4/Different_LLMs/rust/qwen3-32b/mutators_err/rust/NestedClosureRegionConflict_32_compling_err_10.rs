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

pub struct NestedClosureRegionConflict_32;

impl Mutator for NestedClosureRegionConflict_32 {
    fn name(&self) -> &str {
        "NestedClosureRegionConflict_32"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureTransformer::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures within match or if let arms that capture variables from outer scopes. It transforms these closures into nested structures where the inner closure explicitly captures a variable from the outer closure's environment. This stresses region inference between nested closures and parent scopes, potentially exposing bugs in lifetime elision or capture semantics."
    }
}

struct ClosureTransformer {
    in_match_arm: bool,
    in_if_arm: bool,
}

impl ClosureTransformer {
    fn new() -> Self {
        Self { in_match_arm: false, in_if_arm: false }
    }

    fn transform_closure(&mut self, expr: &mut syn::ExprClosure) {
        let mut collector = VariableCollector::new();
        collector.visit_expr(&expr.body);
        let used_vars = collector.get_vars();

        let closure_params: Vec<syn::Ident> = expr
            .inputs
            .iter()
            .filter_map(|arg| {
                if let &FnArg::Typed(pat_type) = arg {
                    if let Pat::Ident(ident_pat) = &*pat_type.pat {
                        Some(ident_pat.ident.clone())
                    } else {
                        None
                    }
                } else {
                    None
                }
            })
            .collect();

        let captured_vars: Vec<_> = used_vars
            .into_iter()
            .filter(|var| !closure_params.contains(var))
            .collect();

        if captured_vars.is_empty() {
            return;
        }

        let captured_var = captured_vars[0].clone();
        let new_var = Ident::new(&format!("{}_{}", captured_var, "copy"), Span::call_site());
        let param = Ident::new("arg", Span::call_site());

        let new_body = parse_quote! {
            {
                let #new_var = #captured_var;
                move |#param| nested(#new_var, #param)
            }
        };

        expr.body = Box::new(new_body);
    }
}

impl VisitMut for ClosureTransformer {
    fn visit_expr_match_mut(&mut self, expr: &mut syn::ExprMatch) {
        self.in_match_arm = true;
        for arm in &mut expr.arms {
            self.visit_expr_mut(&mut arm.body);
        }
        self.in_match_arm = false;
    }

    fn visit_expr_if_mut(&mut self, expr: &mut syn::ExprIf) {
        self.in_if_arm = true;
        self.visit_block_mut(&mut expr.then_branch);
        if let Some(else_branch) = &mut expr.else_branch {
            self.visit_expr_mut(&mut else_branch.1);
        }
        self.in_if_arm = false;
    }

    fn visit_expr_closure_mut(&mut self, expr: &mut syn::ExprClosure) {
        if self.in_match_arm || self.in_if_arm {
            self.transform_closure(expr);
        }
        syn::visit_mut::visit_expr_closure_mut(self, expr);
    }
}

struct VariableCollector {
    vars: HashSet<syn::Ident>,
}

impl VariableCollector {
    fn new() -> Self {
        Self { vars: HashSet::new() }
    }

    fn get_vars(&self) -> HashSet<syn::Ident> {
        self.vars.clone()
    }
}

impl<'a> Visit<'a> for VariableCollector {
    fn visit_expr_path(&mut self, expr: &syn::ExprPath) {
        if let Some(segment) = expr.path.segments.last() {
            if let syn::PathArguments::None = segment.arguments {
                self.vars.insert(segment.ident.clone());
            }
        }
        syn::visit::visit_expr_path(self, expr);
    }
}