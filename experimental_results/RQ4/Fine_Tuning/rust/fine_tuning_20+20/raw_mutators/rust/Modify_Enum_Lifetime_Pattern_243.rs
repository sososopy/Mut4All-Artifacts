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

pub struct Modify_Enum_Lifetime_Pattern_243;

impl Mutator for Modify_Enum_Lifetime_Pattern_243 {
    fn name(&self) -> &str {
        "Modify_Enum_Lifetime_Pattern_243"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut enum_names = HashSet::new();
        for item in &file.items {
            if let Item::Enum(item_enum) = item {
                enum_names.insert(item_enum.ident.to_string());
            }
        }
        let mut target_enum_name = None;
        let mut target_variant_name = None;
        let mut target_lifetime = None;
        for item in &file.items {
            if let Item::Enum(item_enum) = item {
                for variant in &item_enum.variants {
                    if let syn::Fields::Unnamed(fields) = &variant.fields {
                        for field in &fields.unnamed {
                            if let Type::Ptr(ptr) = &field.ty {
                                if let Type::Reference(reference) = &*ptr.elem {
                                    if let Some(lifetime) = &reference.lifetime {
                                        if target_enum_name.is_none() {
                                            target_enum_name = Some(item_enum.ident.clone());
                                            target_variant_name = Some(variant.ident.clone());
                                            target_lifetime = Some(lifetime.clone());
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if target_enum_name.is_none() {
            return;
        }
        let target_enum_name = target_enum_name.unwrap();
        let target_variant_name = target_variant_name.unwrap();
        let target_lifetime = target_lifetime.unwrap();
        let mut visitor = PatternVisitor {
            target_enum_name: target_enum_name.clone(),
            target_variant_name: target_variant_name.clone(),
            target_lifetime: target_lifetime.clone(),
            enum_names,
            replacement: None,
        };
        visitor.visit_file(file);
        let replacement = match visitor.replacement {
            Some(replacement) => replacement,
            None => return,
        };
        let mut replacer = PatternReplacer {
            target_enum_name,
            target_variant_name,
            replacement,
        };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enums with variants involving lifetimes, specifically those using pointer types with lifetime annotations. It identifies such patterns and modifies the pattern matching to introduce a type or lifetime mismatch, such as matching against a non-existent variant or using an incorrect nested enum member. This mutation aims to exploit potential weaknesses in the compiler's pattern matching and lifetime management systems, potentially triggering internal compiler errors or unexpected behavior."
    }
}

struct PatternVisitor {
    target_enum_name: Ident,
    target_variant_name: Ident,
    target_lifetime: Lifetime,
    enum_names: HashSet<String>,
    replacement: Option<Expr>,
}

impl<'ast> Visit<'ast> for PatternVisitor {
    fn visit_expr(&mut self, node: &'ast Expr) {
        if let Expr::Path(expr_path) = node {
            if expr_path
                .path
                .segments
                .last()
                .map_or(false, |seg| seg.ident == self.target_variant_name)
            {
                if expr_path
                    .path
                    .segments
                    .first()
                    .map_or(false, |seg| seg.ident == self.target_enum_name)
                {
                    let mut new_path = expr_path.path.clone();
                    let last_segment = new_path.segments.last_mut().unwrap();
                    last_segment.ident = Ident::new("NonExistent", last_segment.ident.span());
                    self.replacement = Some(Expr::Path(ExprPath {
                        attrs: expr_path.attrs.clone(),
                        qself: expr_path.qself.clone(),
                        path: new_path,
                    }));
                    return;
                }
                if expr_path
                    .path
                    .segments
                    .first()
                    .map_or(false, |seg| self.enum_names.contains(&seg.ident.to_string()))
                {
                    let mut new_path = expr_path.path.clone();
                    let last_segment = new_path.segments.last_mut().unwrap();
                    last_segment.ident = Ident::new("NonExistent", last_segment.ident.span());
                    self.replacement = Some(Expr::Path(ExprPath {
                        attrs: expr_path.attrs.clone(),
                        qself: expr_path.qself.clone(),
                        path: new_path,
                    }));
                    return;
                }
            }
        }
        syn::visit::visit_expr(self, node);
    }
}

struct PatternReplacer {
    target_enum_name: Ident,
    target_variant_name: Ident,
    replacement: Expr,
}

impl VisitMut for PatternReplacer {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Path(expr_path) = node {
            if expr_path
                .path
                .segments
                .last()
                .map_or(false, |seg| seg.ident == self.target_variant_name)
            {
                if expr_path
                    .path
                    .segments
                    .first()
                    .map_or(false, |seg| seg.ident == self.target_enum_name)
                {
                    *node = self.replacement.clone();
                    return;
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}