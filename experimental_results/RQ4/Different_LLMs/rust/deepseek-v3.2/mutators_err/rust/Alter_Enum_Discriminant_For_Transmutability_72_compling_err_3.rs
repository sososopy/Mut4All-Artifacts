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

pub struct Alter_Enum_Discriminant_For_Transmutability_72;

impl Mutator for Alter_Enum_Discriminant_For_Transmutability_72 {
    fn name(&self) -> &str {
        "Alter_Enum_Discriminant_For_Transmutability_72"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut transmutability_enums = HashSet::new();
        let mut visitor = TransmutabilityVisitor {
            enums: &mut transmutability_enums,
        };
        visitor.visit_file(file);
        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                if transmutability_enums.contains(&enum_item.ident) {
                    if let Some(first_variant) = enum_item.variants.first_mut() {
                        match &first_variant.discriminant {
                            Some(_) => {
                                let new_expr: syn::Expr = parse_quote!(1);
                                first_variant.discriminant = Some((syn::token::Eq::default(), new_expr));
                            }
                            None => {
                                let new_expr: syn::Expr = parse_quote!(1);
                                first_variant.discriminant = Some((syn::token::Eq::default(), new_expr));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum definitions used in transmutability checks (BikeshedIntrinsicFrom). It alters the discriminant of the first variant: if the variant has an explicit discriminant, it replaces it with 1; if it lacks an explicit discriminant, it adds an explicit discriminant of 1. This creates potential mismatches between compiler layout calculations and discriminant handling during const-eval, aiming to trigger discriminant-handling bugs in transmutability analysis."
    }
}

struct TransmutabilityVisitor<'a> {
    enums: &'a mut HashSet<syn::Ident>,
}

impl<'a> syn::visit::Visit<'a> for TransmutabilityVisitor<'a> {
    fn visit_expr_call(&mut self, node: &'a syn::ExprCall) {
        if let syn::Expr::Path(path_expr) = &*node.func {
            if path_expr.path.segments.last().map(|seg| seg.ident == "BikeshedIntrinsicFrom").unwrap_or(false) {
                for arg in &node.args {
                    self.visit_expr(arg);
                }
            }
        }
        syn::visit::visit_expr_call(self, node);
    }
    fn visit_expr(&mut self, expr: &'a syn::Expr) {
        if let syn::Expr::Cast(expr_cast) = expr {
            self.visit_type(&expr_cast.ty);
        }
        syn::visit::visit_expr(self, expr);
    }
    fn visit_type(&mut self, ty: &'a syn::Type) {
        if let syn::Type::Path(type_path) = ty {
            if let Some(last_segment) = type_path.path.segments.last() {
                if let syn::PathArguments::AngleBracketed(args) = &last_segment.arguments {
                    for arg in &args.args {
                        if let syn::GenericArgument::Type(inner_ty) = arg {
                            self.visit_type(inner_ty);
                        }
                    }
                }
                self.enums.insert(last_segment.ident.clone());
            }
        } else if let syn::Type::Array(type_array) = ty {
            self.visit_type(&type_array.elem);
        } else if let syn::Type::Tuple(type_tuple) = ty {
            for elem in &type_tuple.elems {
                self.visit_type(elem);
            }
        }
        syn::visit::visit_type(self, ty);
    }
}