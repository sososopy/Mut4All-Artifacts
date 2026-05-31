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

pub struct Replace_Bool_Const_With_Integer_Literal_404;

impl Mutator for Replace_Bool_Const_With_Integer_Literal_404 {
    fn name(&self) -> &str {
        "Replace_Bool_Const_With_Integer_Literal_404"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstBoolReplacer::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function calls with explicit const generic arguments where boolean literals (true/false) are used. It replaces one boolean literal with an integer literal (e.g., 0u8, 1u32) to create a type mismatch. This transformation aims to trigger type-checking errors or internal compiler inconsistencies related to const generic parameter mismatches, especially in features like generic_const_exprs where const evaluation and type checking interact."
    }
}

struct ConstBoolReplacer {
    rng: rand::rngs::ThreadRng,
}

impl ConstBoolReplacer {
    fn new() -> Self {
        Self {
            rng: rand::thread_rng(),
        }
    }

    fn replace_bool_with_int(&mut self, expr: &mut Expr) {
        if let Expr::Call(ExprCall { func, args, .. }) = expr {
            if let Expr::Path(ExprPath { path, .. }) = &mut **func {
                if let Some(last_segment) = path.segments.last_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                        let mut bool_indices = Vec::new();
                        for (i, arg) in args.args.iter_mut().enumerate() {
                            if let GenericArgument::Const(const_arg) = arg {
                                if let Expr::Lit(expr_lit) = const_arg {
                                    if let syn::Lit::Bool(lit_bool) = &expr_lit.lit {
                                        bool_indices.push(i);
                                    }
                                }
                            }
                        }
                        if !bool_indices.is_empty() {
                            let idx = *bool_indices.choose(&mut self.rng).unwrap();
                            if let GenericArgument::Const(const_arg) = &mut args.args[idx] {
                                let int_lit = match self.rng.gen_range(0..2) {
                                    0 => parse_quote!(0u8),
                                    1 => parse_quote!(1u32),
                                    _ => parse_quote!(0u8),
                                };
                                *const_arg = int_lit;
                            }
                        }
                    }
                }
            }
        }
    }
}

impl VisitMut for ConstBoolReplacer {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        self.replace_bool_with_int(expr);
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}