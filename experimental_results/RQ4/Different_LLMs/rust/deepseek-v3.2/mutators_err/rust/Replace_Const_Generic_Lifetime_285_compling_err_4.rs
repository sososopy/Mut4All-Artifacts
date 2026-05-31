use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_Generic_Lifetime_285;

impl Mutator for Replace_Const_Generic_Lifetime_285 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Lifetime_285"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceConstGenericLifetimeVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters that are reference types with explicit lifetimes (e.g., &'a T). It replaces the lifetime with another lifetime that is already in scope within the same item (function, struct, or enum). This mutation tests the compiler's handling of lifetime interactions in const generic contexts, potentially causing lifetime mismatches, substitution errors, or type resolution failures. It only applies when multiple lifetimes are available in the same scope."
    }
}

struct ReplaceConstGenericLifetimeVisitor;

impl ReplaceConstGenericLifetimeVisitor {
    fn new() -> Self {
        Self
    }
}

impl VisitMut for ReplaceConstGenericLifetimeVisitor {
    fn visit_item_fn_mut(&mut self, node: &mut ItemFn) {
        let lifetimes_in_scope: Vec<Lifetime> = node.sig.generics.params.iter()
            .filter_map(|param| {
                if let GenericParam::Lifetime(lifetime_param) = param {
                    Some(lifetime_param.lifetime.clone())
                } else {
                    None
                }
            })
            .collect();

        if lifetimes_in_scope.len() < 2 {
            return;
        }

        for param in &mut node.sig.generics.params {
            if let GenericParam::Const(const_param) = param {
                self.mutate_type_in_const_param(&mut Box::new(const_param.ty.clone()), &lifetimes_in_scope);
            }
        }

        syn::visit_mut::visit_item_fn_mut(self, node);
    }

    fn visit_item_struct_mut(&mut self, node: &mut ItemStruct) {
        let lifetimes_in_scope: Vec<Lifetime> = node.generics.params.iter()
            .filter_map(|param| {
                if let GenericParam::Lifetime(lifetime_param) = param {
                    Some(lifetime_param.lifetime.clone())
                } else {
                    None
                }
            })
            .collect();

        if lifetimes_in_scope.len() < 2 {
            return;
        }

        for param in &mut node.generics.params {
            if let Generic2Param::Const(const_param) = param {
                self.mutate_type_in_const_param(&mut Box::new(const_param.ty.clone()), &lifetimes_in_scope);
            }
        }

        syn::visit_mut::visit_item_struct_mut(self, node);
    }

    fn visit_item_enum_mut(&mut self, node: &mut ItemEnum) {
        let lifetimes_in_scope: Vec<Lifetime> = node.generics.params.iter()
            .filter_map(|param| {
                if let GenericParam::Lifetime(lifetime_param) = param {
                    Some(lifetime_param.lifetime.clone())
                } else {
                    None
                }
            })
            .collect();

        if lifetimes_in_scope.len() < 2 {
            return;
        }

        for param in &mut node.generics.params {
            if let GenericParam::Const(const_param) = param {
                self.mutate_type_in_const_param(&mut Box::new(const_param.ty.clone()), &lifetimes_in_scope);
            }
        }

        syn::visit_mut::visit_item_enum_mut(self, node);
    }
}

impl ReplaceConstGenericLifetimeVisitor {
    fn mutate_type_in_const_param(&self, ty: &mut Box<Type>, lifetimes_in_scope: &[Lifetime]) {
        match &mut **ty {
            Type::Reference(type_ref) => {
                if let Some(lifetime) = &type_ref.lifetime {
                    let mut rng = thread_rng();
                    let candidate_lifetimes: Vec<&Lifetime> = lifetimes_in_scope.iter()
                        .filter(|&lt| lt.ident != lifetime.ident)
                        .collect();
                    if !candidate_lifetimes.is_empty() {
                        let new_lifetime = candidate_lifetimes.choose(&mut rng).unwrap().clone();
                        type_ref.lifetime = Some(new_lifetime.clone());
                    }
                }
            }
            Type::Slice(type_slice) => {
                self.mutate_type_in_const_param(&mut type_slice.elem, lifetimes_in_scope);
            }
            Type::Array(type_array) => {
                self.mutate_type_in_const_param(&mut type_array.elem, lifetimes_in_scope);
            }
            Type::Tuple(type_tuple) => {
                for elem in &mut type_tuple.elems {
                    self.mutate_type_in_const_param(&mut Box::new(elem.clone()), lifetimes_in_scope);
                }
            }
            Type::Path(type_path) => {
                if let Some(qself) = &mut type_path.qself {
                    self.mutate_type_in_const_param(&mut qself.ty, lifetimes_in_scope);
                }
                for segment in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Type(arg_ty) = arg {
                                self.mutate_type_in_const_param(&mut Box::new(arg_2.clone()), lifetimes_in_scope);
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }
    fn mutate_type_in_const2_param(&self, ty: &mut Box<Type>, lifetimes_in_scope: &[Lifetime]) {
        match &mut **ty {
            Type::Reference(type_ref) => {
                if let Some(lifetime) = &type_ref.lifetime {
                    let mut rng = thread_rng();
                    let candidate_lifetimes: Vec<&Lifetime> = lifetimes_in_scope.iter()
                        .filter(|&lt| lt.ident != lifetime.ident)
                        .collect();
                    if !candidate_lifetimes.is_empty() {
                        let new_lifetime = candidate_lifetimes.choose(&mut rng).unwrap().clone();
                        type_ref.lifetime = Some(new_lifetime.clone());
                    }
                }
            }
            Type::Slice(type_slice) => {
                self.mutate_type_in_const2_param(&mut type_slice.elem, lifetimes_in_scope);
            }
            Type::Array(type_array) => {
                self.mutate_type_in_const2_param(&mut type_array.elem, lifetimes_in_scope);
            }
            Type::Tuple(type_tuple) => {
                for elem in &mut type_tuple.elems {
                    self.mutate_type_in_const2_param(&mut Box::new(elem.clone()), lifetimes_in_scope);
                }
            }
            Type::Path(type_path) => {
                if let Some(qself) = &mut type_path.qself {
                    self.mutate_type_in_const2_param(&mut qself.ty, lifetimes_in_scope);
                }
                for segment in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Type(arg_ty) = arg {
                                self.mutate_type_in_const2_param(&mut Box::new(arg_ty.clone()), lifetimes_in_scope);
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }
}