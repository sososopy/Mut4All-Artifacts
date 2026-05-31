use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct IncreaseConstArraySizeWithBitshift_77;

impl Mutator for IncreaseConstArraySizeWithBitshift_77 {
    fn name(&self) -> &str {
        "IncreaseConstArraySizeWithBitshift_77"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Some(expr) = &mut item_const.expr {
                    let mut visitor = ArraySizeMutator;
                    visitor.visit_expr_mut(expr);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator increases the size of array initializers in const declarations by replacing their size expressions with 'usize::MAX >> 16'. This transformation stresses the compiler's const evaluation system by requesting a large array allocation during compilation, potentially triggering ICEs in the const evaluator due to excessive size."
    }
}

struct ArraySizeMutator;

impl VisitMut for ArraySizeMutator {
    fn visit_expr(&mut self, expr: &mut Expr) {
        if let Expr::Array(expr_array) = expr {
            if expr_array.suffix.is_some() {
                let new_suffix: Expr = parse_quote! { usize::MAX >> 16 };
                expr_array.suffix = Some(Box::new(new_suffix));
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}