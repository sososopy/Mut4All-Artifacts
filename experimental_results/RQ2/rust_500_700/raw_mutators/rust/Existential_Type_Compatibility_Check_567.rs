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

pub struct Existential_Type_Compatibility_Check_567;

impl Mutator for Existential_Type_Compatibility_Check_567 {
    fn name(&self) -> &str {
        "Existential_Type_Compatibility_Check_567"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        // Mutation 1: Change Closure Return Type
                        let new_return_type: Type = parse_quote!(u32);
                        let new_expr: Expr = parse_quote!(|| 0u32);
                        let mut new_block = item_fn.block.clone();
                        new_block.stmts.clear();
                        new_block.stmts.push(Stmt::Expr(new_expr, None));
                        item_fn.block = new_block;

                        // Mutation 2: Alter Logic of Closure
                        let altered_expr: Expr = parse_quote!(|| 1usize + 1);
                        let mut altered_block = item_fn.block.clone();
                        altered_block.stmts.clear();
                        altered_block.stmts.push(Stmt::Expr(altered_expr, None));
                        item_fn.block = altered_block;

                        // Mutation 3: Add Parameter to const fn
                        item_fn.sig.inputs.push(parse_quote!(x: usize));
                        let param_expr: Expr = parse_quote!(move || x);
                        let mut param_block = item_fn.block.clone();
                        param_block.stmts.clear();
                        param_block.stmts.push(Stmt::Expr(param_expr, None));
                        item_fn.block = param_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}