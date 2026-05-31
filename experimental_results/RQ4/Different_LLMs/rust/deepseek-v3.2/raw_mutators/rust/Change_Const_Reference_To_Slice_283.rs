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

pub struct Change_Const_Reference_To_Slice_283;

impl Mutator for Change_Const_Reference_To_Slice_283 {
    fn name(&self) -> &str {
        "Change_Const_Reference_To_Slice_283"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstRefToSliceVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters with reference types (&'static str or &'static [T]) in functions, structs, and impls. It changes the const parameter type from a reference to a slice of the same base type (e.g., &'static str to [&'static str; 1]). The operator updates all uses of that const parameter within the relevant scope to match the new type by replacing A with A[0] in expressions. It also modifies const generic arguments at call/instantiation sites from FOO to [FOO; 1]. This transformation stresses the compiler's handling of const generic parameters with complex nested types, potentially exposing bugs in type checking, monomorphization, or MIR generation for ADT const params."
    }
}

struct ConstRefToSliceVisitor;

impl ConstRefToSliceVisitor {
    fn new() -> Self {
        Self
    }

    fn is_reference_type(&self, ty: &Type) -> bool {
        if let Type::Reference(type_ref) = ty {
            if let Some(lifetime) = &type_ref.lifetime {
                if lifetime.ident == "static" {
                    return true;
                }
            }
        }
        false
    }

    fn transform_reference_to_slice(&self, ty: &Type) -> Option<Type> {
        if let Type::Reference(type_ref) = ty {
            if let Some(lifetime) = &type_ref.lifetime {
                if lifetime.ident == "static" {
                    let base_type = *type_ref.elem.clone();
                    return Some(parse_quote!([#base_type; 1]));
                }
            }
        }
        None
    }
}

impl VisitMut for ConstRefToSliceVisitor {
    fn visit_generic_param_mut(&mut self, param: &mut GenericParam) {
        if let GenericParam::Const(const_param) = param {
            if let Some(new_ty) = self.transform_reference_to_slice(&const_param.ty) {
                const_param.ty = new_ty;
            }
        }
        visit_mut::visit_generic_param_mut(self, param);
    }

    fn visit_expr_path_mut(&mut self, expr: &mut ExprPath) {
        if expr.path.segments.len() == 1 {
            let segment = &expr.path.segments[0];
            if segment.arguments.is_none() {
                let ident = &segment.ident;
                let new_path: SynPath = parse_quote!(#ident[0]);
                *expr = ExprPath { path: new_path, ..expr.clone() };
            }
        }
        visit_mut::visit_expr_path_mut(self, expr);
    }

    fn visit_path_mut(&mut self, path: &mut SynPath) {
        if path.segments.len() == 1 && path.segments[0].arguments.is_none() {
            let segment = &path.segments[0];
            let ident = &segment.ident;
            let new_segment: PathSegment = parse_quote!([#ident; 1]);
            path.segments = Punctuated::from_iter(vec![new_segment]);
        }
        visit_mut::visit_path_mut(self, path);
    }

    fn visit_item_fn_mut(&mut self, item: &mut ItemFn) {
        for param in &mut item.sig.generics.params {
            self.visit_generic_param_mut(param);
        }
        visit_mut::visit_block_mut(self, &mut item.block);
    }

    fn visit_item_struct_mut(&mut self, item: &mut ItemStruct) {
        for param in &mut item.generics.params {
            self.visit_generic_param_mut(param);
        }
        visit_mut::visit_item_struct_mut(self, item);
    }

    fn visit_item_impl_mut(&mut self, item: &mut ItemImpl) {
        for param in &mut item.generics.params {
            self.visit_generic_param_mut(param);
        }
        for impl_item in &mut item.items {
            if let ImplItem::Fn(method) = impl_item {
                self.visit_item_fn_mut(method);
            }
        }
    }
}