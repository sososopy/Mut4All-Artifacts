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

pub struct Modify_Yield_Expressions_In_Generators_662;

impl Mutator for Modify_Yield_Expressions_In_Generators_662 {
    fn name(&self) -> &str {
        "Modify_Yield_Expressions_In_Generators_662"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if type_impl_trait.bounds.iter().any(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.path.segments.last().map_or(false, |segment| segment.ident == "Generator")
                            } else {
                                false
                            }
                        }) {
                            let mut visitor = YieldExprVisitor;
                            visitor.visit_block_mut(&mut item_fn.block);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct YieldExprVisitor;

impl VisitMut for YieldExprVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Yield(expr_yield) = expr {
            if let Some(expr) = &expr_yield.expr {
                if let Expr::Reference(expr_ref) = &**expr {
                    expr_yield.expr = Some(expr_ref.expr.clone());
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}