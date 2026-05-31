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

pub struct Modify_Function_Usage_In_Const_Context_431;

impl Mutator for Modify_Function_Usage_In_Const_Context_431 {
    fn name(&self) -> &str {
        "Modify_Function_Usage_In_Const_Context_431"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstContextFinder {
            const_exprs: Vec<Expr>,
        }
        impl<'ast> Visit<'ast> for ConstContextFinder {
            fn visit_expr(&mut self, node: &'ast Expr) {
                match node {
                    Expr::Const(_) | Expr::Repeat(_) | Expr::Array(_) => {
                        self.const_exprs.push(node.clone());
                    }
                    _ => {}
                }
                syn::visit::visit_expr(self, node);
            }
        }
        let mut finder = ConstContextFinder {
            const_exprs: Vec::new(),
        };
        finder.visit_file(file);
        let mut rng = thread_rng();
        let mut const_exprs = finder.const_exprs;
        const_exprs.shuffle(&mut rng);
        for const_expr in const_exprs {
            let mut replacer = ConstExprReplacer {
                target_expr: const_expr,
                found: false,
            };
            replacer.visit_file_mut(file);
            if replacer.found {
                break;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function calls used within constant expressions by replacing them with non-constant expressions, such as direct function calls or local variables, to induce errors in constant evaluation contexts."
    }
}

struct ConstExprReplacer {
    target_expr: Expr,
    found: bool,
}

impl VisitMut for ConstExprReplacer {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if self.found {
            return;
        }
        if node == &self.target_expr {
            if let Expr::Path(_) = node {
                *node = Expr::Call(parse_quote!(some_function()));
                self.found = true;
                return;
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
    fn visit_local_mut(&mut self, node: &mut Local) {
        if self.found {
            return;
        }
        if node.init.is_some() {
            if let Pat::Ident(pat_ident) = &node.pat {
                if pat_ident.ident == "fn_value" {
                    node.pat = parse_quote!(local_variable);
                    node.init = Some(LocalInit {
                        eq_token: token::Eq::default(),
                        expr: parse_quote!(5),
                        diverge: None,
                    });
                    self.found = true;
                    return;
                }
            }
        }
        syn::visit_mut::visit_local_mut(self, node);
    }
}