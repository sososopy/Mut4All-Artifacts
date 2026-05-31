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

pub struct Struct_Default_Update_In_Async_508;

impl Mutator for Struct_Default_Update_In_Async_508 {
    fn name(&self) -> &str {
        "Struct_Default_Update_In_Async_508"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.asyncness.is_some() {
                    let mut visitor = StructInitVisitor::new();
                    visitor.visit_block_mut(&mut item_fn.block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

struct StructInitVisitor;

impl StructInitVisitor {
    fn new() -> Self {
        StructInitVisitor
    }
}

impl VisitMut for StructInitVisitor {
    fn visit_expr_struct_mut(&mut self, expr_struct: &mut ExprStruct) {
        if let Some(rest_expr) = expr_struct.rest.take() {
            if let Expr::Call(ExprCall { func, .. }) = *rest_expr {
                if let Expr::Path(ExprPath { path, .. }) = *func {
                    if path.is_ident("Default::default") {
                        if let Some(field) = expr_struct.fields.first_mut() {
                            let field_ident = field.member.clone();
                            expr_struct.fields.clear();
                            expr_struct.fields.push(parse_quote! { #field_ident: Default::default() });
                            expr_struct.rest = Some(Box::new(parse_quote! { Default::default() }));
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_struct_mut(self, expr_struct);
    }
}