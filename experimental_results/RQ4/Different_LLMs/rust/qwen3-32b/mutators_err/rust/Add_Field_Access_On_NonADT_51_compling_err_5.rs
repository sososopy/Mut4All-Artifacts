use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprCall, ExprLit, ExprPath, ExprReference, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Add_Field_Access_On_NonADT_51;

impl Mutator for Add_Field_Access_On_NonADT_51 {
    fn name(&self) -> &str {
        "Add_Field_Access_On_NonADT_51"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut non_adt_vars = HashSet::new();

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(init) = &mut local.init {
                            if is_non_adt_initializer(&init.expr) {
                                if let Pat::Ident(pat_ident) = &local.pat {
                                    non_adt_vars.insert(pat_ident.ident.to_string());
                                }
                            }
                        }
                    }
                }
            }
        }

        let mut visitor = ReplaceFieldVisitor::new(non_adt_vars);
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn is_non_adt_initializer(expr: &Expr) -> bool {
    match expr {
        Expr::Array(_) => true,
        Expr::Lit(_) => true,
        Expr::Reference(expr_ref) => {
            if let Expr::Array(_) = &*expr_ref.expr {
                true
            } else {
                false
            }
        },
        _ => false,
    }
}

fn get_ident_from_path(path: &syn::Path) -> Option<&Ident> {
    if path.segments.len() == 1 {
        let segment = &path.segments[0];
        if let syn::PathArguments::None = segment.arguments {
            Some(&segment.ident)
        } else {
            None
        }
    } else {
        None
    }
}

struct ReplaceFieldVisitor {
    variables: HashSet<String>,
}

impl ReplaceFieldVisitor {
    fn new(vars: HashSet<String>) -> Self {
        Self { variables: vars }
    }
}

impl VisitMut for ReplaceFieldVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(expr_path) = expr {
            if let Some(ident) = get_ident_from_path(&expr_path.path) {
                if self.variables.contains(&ident.to_string()) {
                    let field_name = "nonexistent_field";
                    let dot_token = token::Dot::default();
                    let member = syn::Member::Named(Ident::new(field_name, Span::call_site()));
                    let field_expr = syn::ExprField {
                        attrs: Vec::new(),
                        expr: Box::new((*expr_path).clone()),
                        dot_token,
                        member,
                    };
                    *expr = syn::Expr::Field(Box::new(field_expr));
                }
            }
        }
        visit_mut::visit_expr_mut(self, expr);
    }
}