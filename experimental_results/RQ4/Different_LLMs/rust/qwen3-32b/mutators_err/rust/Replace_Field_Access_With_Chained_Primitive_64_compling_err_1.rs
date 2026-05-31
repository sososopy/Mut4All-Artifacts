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

pub struct Replace_Field_Access_With_Chained_Primitive_64;

struct FieldChainingVisitor;

impl<'ast> VisitMut for FieldChainingVisitor {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_expr_field_mut(&mut self, expr_field: &mut syn::ExprField) {
        let original_expr = expr_field.clone();
        let new_expr = syn::Expr::Field(Box::new(syn::ExprField {
            base: Box::new(original_expr),
            dot_token: syn::token::Dot::default(),
            member: syn::Member::Unnamed(0.into()),
            attrs: Vec::new(),
            span: proc_macro2::Span::call_site().into(),
        }));
        *expr_field = new_expr;
    }
}

impl Mutator for Replace_Field_Access_With_Chained_Primitive_64 {
    fn name(&self) -> &str {
        "Replace_Field_Access_With_Chained_Primitive_64"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = FieldChainingVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator replaces every field access on an ADT with a chained field access to a primitive type. It appends .0 to the original field access, transforming ADT field accesses into attempts to access a non-ADT (primitive) type. This induces invalid field accesses on primitives, testing the compiler's field resolution and type checking mechanisms."
    }
}