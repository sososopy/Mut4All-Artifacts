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

pub struct Insert_Inline_Const_In_Closure_With_Lifetime_429;

impl Mutator for Insert_Inline_Const_In_Closure_With_Lifetime_429 {
    fn name(&self) -> &str {
        "Insert_Inline_Const_In_Closure_With_Lifetime_429"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureMutatorVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures, adding a for<'a> binder if missing, then inserting an inline const block at the start of the closure body. The const block declares a temporary variable and creates a reference annotated with the closure's lifetime parameter, exposing lifetime interactions. This transformation stresses the compiler's region handling for inline consts within late-bound parameter scopes, potentially triggering region conversion errors or ICEs in borrow checking."
    }
}

struct ClosureMutatorVisitor;

impl VisitMut for ClosureMutatorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Closure(closure) = expr {
            let mut has_for_binder = false;
            let lifetime_param_name = if let Some(bound_lifetimes) = &closure.lifetimes {
                has_for_binder = true;
                if let Some(first) = bound_lifetimes.lifetimes.first() {
                    if let GenericParam::Lifetime(lifetime_param) = first {
                        lifetime_param.lifetime.ident.to_string()
                    } else {
                        "a".to_string()
                    }
                } else {
                    "a".to_string()
                }
            } else {
                "a".to_string()
            };

            if !has_for_binder {
                let new_lifetime = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new(&format!("'{}", lifetime_param_name), Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                closure.lifetimes = Some(BoundLifetimes {
                    for_token: token::For::default(),
                    lt_token: token::Lt::default(),
                    lifetimes: {
                        let mut lifetimes = Punctuated::new();
                        lifetimes.push(GenericParam::Lifetime(new_lifetime));
                        lifetimes
                    },
                    gt_token: token::Gt::default(),
                });
            }

            let lifetime_ident = Ident::new(&lifetime_param_name, Span::call_site());
            let const_block: Stmt = parse_quote! {
                const {
                    let temp = 0;
                    let _: & #lifetime_ident i32 = &temp;
                }
            };
            if let Expr::Block(block) = &mut *closure.body {
                block.block.stmts.insert(0, const_block);
            }
        }
        visit_mut::visit_expr_mut(self, expr);
    }
}