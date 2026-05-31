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

pub struct Insert_Let_Lifetime_In_Const_Generic_Block_376;

impl Mutator for Insert_Let_Lifetime_In_Const_Generic_Block_376 {
    fn name(&self) -> &str {
        "Insert_Let_Lifetime_In_Const_Generic_Block_376"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = InsertLetLifetimeVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic arguments that are block expressions. It modifies the block by inserting a let statement with an explicit lifetime annotation at the beginning. If the enclosing function has generic lifetime parameters, one is chosen; otherwise, a new lifetime parameter 'a is added to the function signature. The inserted variable is a reference type using that lifetime (e.g., &'a ()). This aims to trigger bugs in the borrow checker's universal regions computation when processing lifetimes within const generic block expressions."
    }
}

struct InsertLetLifetimeVisitor;

impl InsertLetLifetimeVisitor {
    fn new() -> Self {
        InsertLetLifetimeVisitor
    }
}

impl VisitMut for InsertLetLifetimeVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        let expr_span = expr.span();
        if let Expr::Call(ExprCall { func, args, attrs: _, paren_token: _ }) = expr {
            if let Expr::Path(ExprPath { path, .. }) = &mut **func {
                for segment in &mut path.segments {
                    if let PathArguments::AngleBracketed(angle_args) = &mut segment.arguments {
                        for arg in &mut angle_args.args {
                            if let GenericArgument::Const(const_arg) = arg {
                                if let Expr::Block(block) = const_arg {
                                    self.handle_const_block(block, expr_span);
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_item_fn_mut(&mut self, item_fn: &mut ItemFn) {
        syn::visit_mut::visit_item_fn_mut(self, item_fn);
    }
}

impl InsertLetLifetimeVisitor {
    fn handle_const_block(&mut self, block: &mut ExprBlock, span: Span) {
        let enclosing_fn = self.find_enclosing_function(span);
        if let Some(func) = enclosing_fn {
            let lifetime = self.get_or_add_lifetime(&mut unsafe { &mut *func }.sig.generics);
            let let_stmt: Stmt = parse_quote! {
                let _: & #lifetime ();
            };
            block.block.stmts.insert(0, let_stmt);
        }
    }

    fn find_enclosing_function(&mut self, span: Span) -> Option<*mut ItemFn> {
        let mut current_item: Option<*mut ItemFn> = None;
        let mut visitor = FindFunctionVisitor {
            target_span: span,
            found_func: None,
        };
        visitor.visit_file_mut(&mut parse_quote! {});
        visitor.found_func.map(|ptr| ptr as *mut ItemFn)
    }

    fn get_or_add_lifetime(&self, generics: &mut Generics) -> Lifetime {
        let existing_lifetimes: Vec<&LifetimeParam> = generics.params.iter()
            .filter_map(|param| match param {
                GenericParam::Lifetime(lp) => Some(lp),
                _ => None,
            })
            .collect();

        if !existing_lifetimes.is_empty() {
            let chosen = existing_lifetimes[0];
            Lifetime::new(&format!("'{}", chosen.lifetime.ident), chosen.lifetime.span())
        } else {
            let new_lifetime = LifetimeParam {
                attrs: Vec::new(),
                lifetime: Lifetime::new("'a", Span::call_site()),
                colon_token: None,
                bounds: Punctuated::new(),
            };
            generics.params.insert(0, GenericParam::Lifetime(new_lifetime));
            Lifetime::new("'a", Span::call_site())
        }
    }
}

struct FindFunctionVisitor {
    target_span: Span,
    found_func: Option<*mut ItemFn>,
}

impl VisitMut for FindFunctionVisitor {
    fn visit_item_fn_mut(&mut self, item_fn: &mut ItemFn) {
        let item_span = item_fn.span();
        let target_span = self.target_span;
        if item_span.source_text().is_some() && target_span.source_text().is_some() {
            let item_start = item_span.start();
            let item_end = item_span.end();
            let target_start = target_span.start();
            let target_end = target_span.end();
            if item_start.line <= target_start.line && item_end.line >= target_end.line {
                if item_start.line == target_start.line && item_start.column > target_start.column {
                    return;
                }
                if item_end.line == target_end.line && item_end.column < target_end.column {
                    return;
                }
                self.found_func = Some(item_fn as *mut ItemFn);
            }
        }
        syn::visit_mut::visit_item_fn_mut(self, item_fn);
    }
}