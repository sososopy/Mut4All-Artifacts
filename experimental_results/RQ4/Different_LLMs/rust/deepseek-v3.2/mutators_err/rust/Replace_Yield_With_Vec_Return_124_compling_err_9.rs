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

pub struct Replace_Yield_With_Vec_Return_124;

impl Mutator for Replace_Yield_With_Vec_Return_124 {
    fn name(&self) -> &str {
        "Replace_Yield_With_Vec_Return_124"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut visitor = YieldVisitor {
                            found_yield: false,
                            yield_expr: None,
                        };
                        visitor.visit_block_mut(&mut item_fn.block);
                        if visitor.found_yield && visitor.yield_expr.is_some() {
                            let yield_expr = visitor.yield_expr.unwrap();
                            let vec_expr: Expr = parse_quote! { vec![#yield_expr] };
                            let return_stmt: Stmt = parse_quote! { return #vec_expr; };
                            let mut new_block = item_fn.block.clone();
                            new_block.stmts.clear();
                            new_block.stmts.push(return_stmt);
                            item_fn.block = new_block;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait` that contain yield expressions. It replaces the yield statement with a return statement that returns a Vec containing the yielded value. This transformation removes the generator state machine and replaces it with a direct return of a collection that implements IntoIterator, testing the compiler's handling of trait resolution and return type inference in contexts where generators are expected."
    }
}

struct YieldVisitor {
    found_yield: bool,
    yield_expr: Option<Expr>,
}

impl VisitMut for YieldVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Yield(yield_expr) = expr {
            self.found_yield = true;
            if let Some(yield_expr_box) = &yield_expr.expr {
                self.yield_expr = Some(*yield_expr_box.clone());
            }
            *expr = parse_quote! { () };
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}