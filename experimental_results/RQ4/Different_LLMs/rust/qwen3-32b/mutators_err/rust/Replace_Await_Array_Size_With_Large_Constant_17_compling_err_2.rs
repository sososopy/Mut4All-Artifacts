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

pub struct Replace_Await_Array_Size_With_Large_Constant_17;

impl Mutator for Replace_Await_Array_Size_With_Large_Constant_17 {
    fn name(&self) -> &str {
        "Replace_Await_Array_Size_With_Large_Constant_17"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    let mut visitor = ReplaceArraySizeVisitor;
                    visitor.visit_block_mut(&mut func.block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array repeat expressions used as operands in await expressions within async functions. It replaces the array size with a large constant (0xffff_ffff_ffff_ffff) to trigger overflow during generator interior type analysis. This transformation stresses the compiler's handling of large constants in async contexts, potentially exposing bugs in type inference or constant evaluation for async generators."
    }
}

struct ReplaceArraySizeVisitor;

impl VisitMut for ReplaceArraySizeVisitor {
    fn visit_expr_await_mut(&mut self, expr_await: &mut ExprAwait) {
        if let Expr::Repeat(expr_repeat) = &mut *expr_await.base {
            *expr_repeat.len = Box::new(parse_quote! { 0xffff_ffff_ffff_ffff });
        } else {
            self.visit_expr_mut(&mut expr_await.base);
        }
    }

    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}