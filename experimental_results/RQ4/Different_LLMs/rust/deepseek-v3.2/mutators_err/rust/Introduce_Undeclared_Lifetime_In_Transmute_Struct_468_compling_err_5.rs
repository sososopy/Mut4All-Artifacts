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

pub struct Introduce_Undeclared_Lifetime_In_Transmute_Struct_468;

impl Mutator for Introduce_Undeclared_Lifetime_In_Transmute_Struct_468 {
    fn name(&self) -> &str {
        "Introduce_Undeclared_Lifetime_In_Transmute_Struct_468"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut transmute_structs = Vec::new();
        let mut visitor = TransmuteStructVisitor {
            structs: &mut transmute_structs,
        };
        visitor.visit_file(file);
        let mut rng = thread_rng();
        let struct_item = transmute_structs.choose_mut(&mut rng);
        if let Some(struct_item) = struct_item {
            let mut candidate_fields = Vec::new();
            for (i, field) in struct_item.fields.iter_mut().enumerate() {
                if let Type::Reference(type_ref) = &mut field.ty {
                    candidate_fields.push(i);
                }
            }
            if let Some(field_idx) = candidate_fields.choose(&mut rng) {
                if let Type::Reference(type_ref) = &mut struct_item.fields.iter_mut().nth(*field_idx).unwrap().ty {
                    type_ref.lifetime = Some(Lifetime::new("'undeclared", Span::call_site()));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions that are used in transmutability checks (e.g., within calls to `BikeshedIntrinsicFrom` or `is_transmutable`). It identifies a reference type field within such a struct and replaces its lifetime with an undeclared lifetime identifier 'undeclared. This mimics a bug scenario where a struct contains a reference with a lifetime that is not declared in the struct's generics, potentially causing lifetime resolution errors during transmutability analysis."
    }
}

struct TransmuteStructVisitor<'a> {
    structs: &'a mut Vec<&'a mut ItemStruct>,
}

impl<'a> Visit<'a> for TransmuteStructVisitor<'a> {
    fn visit_expr_call(&mut self, node: &'a ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.func {
            if let Some(segment) = path.segments.last() {
                if segment.ident == "BikeshedIntrinsicFrom" || segment.ident == "is_transmutable" {
                    for arg in &node.args {
                        if let Expr::Path(expr_path) = arg {
                            syn::visit::visit_expr_path(self, expr_path);
                        }
                    }
                }
            }
        }
        syn::visit::visit_expr_call(self, node);
    }
    fn visit_item_struct(&mut self, node: &'a ItemStruct) {
        unsafe {
            let node_mut = &mut *(node as *const ItemStruct as *mut ItemStruct);
            self.structs.push(node_mut);
        }
    }
}