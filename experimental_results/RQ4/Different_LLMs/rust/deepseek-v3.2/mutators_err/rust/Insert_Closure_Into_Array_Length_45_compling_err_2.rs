use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use rand::prelude::IteratorRandom;
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

pub struct Insert_Closure_Into_Array_Length_45;

impl Mutator for Insert_Closure_Into_Array_Length_45 {
    fn name(&self) -> &str {
        "Insert_Closure_Into_Array_Length_45"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut integer_literals = HashSet::new();
        let mut visitor = CollectIntegerLiterals { literals: &mut integer_literals };
        visitor.visit_file(&file);

        let mut rng = thread_rng();
        let mut visitor = MutateArrayLengths { literals: &integer_literals, rng: &mut rng };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array type definitions, specifically the length expression within the brackets. It replaces the length expression with a closure that returns a constant integer. The closure has no parameters and its body is a constant integer literal. This transformation aims to stress the compiler's handling of closures in const contexts, array length evaluation, and type inference for arrays with non-trivial length expressions. It may trigger bugs in const evaluation, closure type resolution, or array type validation."
    }
}

struct CollectIntegerLiterals<'a> {
    literals: &'a mut HashSet<i64>,
}

impl<'a> Visit<'a> for CollectIntegerLiterals<'a> {
    fn visit_expr(&mut self, node: &'a Expr) {
        if let Expr::Lit(expr_lit) = node {
            if let Lit::Int(lit_int) = &expr_lit.lit {
                self.literals.insert(lit_int.base10_parse().unwrap());
            }
        }
        syn::visit::visit_expr(self, node);
    }
}

struct MutateArrayLengths<'a> {
    literals: &'a HashSet<i64>,
    rng: &'a mut rand::rngs::ThreadRng,
}

impl<'a> VisitMut for MutateArrayLengths<'a> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Array(type_array) = node {
            let constant_value = if self.literals.is_empty() {
                0
            } else {
                *self.literals.iter().choose(self.rng).unwrap()
            };
            let closure_expr: Expr = parse_quote! { || #constant_value };
            type_array.len = closure_expr;
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}