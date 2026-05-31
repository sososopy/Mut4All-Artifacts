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

pub struct Toggle_Const_Generic_Braces_293;

impl Mutator for Toggle_Const_Generic_Braces_293 {
    fn name(&self) -> &str {
        "Toggle_Const_Generic_Braces_293"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = item_impl.trait_ {
                    continue;
                }
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    for segment in &mut type_path.path.segments {
                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    let mut visitor = ConstExprVisitor::new(&mut rng);
                                    visitor.visit_expr_mut(expr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant generic arguments in impl blocks, specifically when they involve function calls or complex expressions. It toggles braces around constant expressions: if the expression is already braced, it removes the braces; if not, it adds braces. For function calls, it may also place braces around individual arguments to create mismatched brace patterns. This transformation creates syntactically valid but semantically odd constant arguments that may trigger internal compiler errors during constant evaluation or valtree construction, especially with incomplete features like generic_const_exprs and adt_const_params."
    }
}

struct ConstExprVisitor<'a, R: Rng> {
    rng: &'a mut R,
}

impl<'a, R: Rng> ConstExprVisitor<'a, R> {
    fn new(rng: &'a mut R) -> Self {
        Self { rng }
    }
}

impl<'a, R: Rng> VisitMut for ConstExprVisitor<'a, R> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        match expr {
            Expr::Call(call_expr) => {
                let choice = self.rng.gen_range(0..3);
                match choice {
                    0 => {
                        let inner_expr = Box::new(call_expr.clone());
                        *expr = parse_quote!({ #inner_expr });
                    }
                    1 => {
                        if !call_expr.args.is_empty() {
                            let idx = self.rng.gen_range(0..call_expr.args.len());
                            let arg = &mut call_expr.args[idx];
                            let inner_arg = Box::new(arg.clone());
                            *arg = parse_quote!({ #inner_arg });
                        }
                    }
                    _ => {}
                }
            }
            Expr::Array(array_expr) => {
                if self.rng.gen_bool(0.5) {
                    let inner_expr = Box::new(array_expr.clone());
                    *expr = parse_quote!({ #inner_expr });
                }
            }
            Expr::Tuple(tuple_expr) => {
                if self.rng.gen_bool(0.5) {
                    let inner_expr = Box::new(tuple_expr.clone());
                    *expr = parse_quote!({ #inner_expr });
                }
            }
            Expr::Block(block_expr) => {
                if block_expr.block.stmts.len() == 1 {
                    if let Some(stmt) = block_expr.block.stmts.first() {
                        if let Stmt::Expr(inner_expr, _) = stmt {
                            *expr = inner_expr.clone();
                        }
                    }
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}