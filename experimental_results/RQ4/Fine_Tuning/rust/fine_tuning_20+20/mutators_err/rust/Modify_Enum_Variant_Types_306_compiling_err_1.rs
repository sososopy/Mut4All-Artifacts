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

pub struct Modify_Enum_Variant_Types_306;

impl Mutator for Modify_Enum_Variant_Types_306 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_Types_306"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if item_enum.variants.len() < 2 {
                    continue;
                }
                let mut rng = thread_rng();
                let target_variant_index = rng.gen_range(0..item_enum.variants.len());
                let mut target_variant_data_types = Vec::new();
                for (i, variant) in item_enum.variants.iter().enumerate() {
                    if i == target_variant_index {
                        if let syn::Fields::Unnamed(fields) = &variant.fields {
                            for field in &fields.unnamed {
                                target_variant_data_types.push(field.ty.clone());
                            }
                        }
                    }
                }
                if target_variant_data_types.is_empty() {
                    continue;
                }
                let mut available_data_types = Vec::new();
                for (i, variant) in item_enum.variants.iter().enumerate() {
                    if i != target_variant_index {
                        if let syn::Fields::Unnamed(fields) = &variant.fields {
                            for field in &fields.unnamed {
                                available_data_types.push(field.ty.clone());
                            }
                        }
                    }
                }
                if available_data_types.is_empty() {
                    continue;
                }
                let mut new_data_types = Vec::new();
                for _ in &target_variant_data_types {
                    if let Some(new_type) = available_data_types.choose(&mut rng) {
                        new_data_types.push(new_type.clone());
                    }
                }
                for (i, variant) in item_enum.variants.iter_mut().enumerate() {
                    if i == target_variant_index {
                        if let syn::Fields::Unnamed(fields) = &mut variant.fields {
                            for (j, field) in fields.unnamed.iter_mut().enumerate() {
                                if j < new_data_types.len() {
                                    field.ty = new_data_types[j].clone();
                                }
                            }
                        }
                    }
                }
                let enum_ident = &item_enum.ident;
                let mut variant_idents = Vec::new();
                for variant in &item_enum.variants {
                    variant_idents.push(variant.ident.clone());
                }
                let mut new_variant_idents = Vec::new();
                for (i, variant_ident) in variant_idents.iter().enumerate() {
                    let new_variant_ident = if i == target_variant_index {
                        Ident::new(
                            &format!("{}_Modified", variant_ident.to_string()),
                            variant_ident.span(),
                        )
                    } else {
                        variant_ident.clone()
                    };
                    new_variant_idents.push(new_variant_ident);
                }
                for item in &mut file.items {
                    if let Item::Fn(item_fn) = item {
                        let mut visitor = EnumVariantUsageVisitor {
                            enum_ident,
                            variant_idents: &variant_idents,
                            new_variant_idents: &new_variant_idents,
                            replacements: Vec::new(),
                        };
                        visitor.visit_item_fn(item_fn);
                        for (old_expr, new_expr) in visitor.replacements {
                            replace_expr(item_fn, &old_expr, new_expr);
                        }
                    }
                }
                for (i, variant) in item_enum.variants.iter_mut().enumerate() {
                    variant.ident = new_variant_idents[i].clone();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify an enum with multiple variants, each having associated data types. Randomly select one variant and change its data types to those used in other variants of the same enum. Update all code that constructs or matches on the modified variant to use the new data types, potentially introducing type mismatches to stress-test the compiler's type resolution and variant handling."
    }
}

struct EnumVariantUsageVisitor<'a> {
    enum_ident: &'a Ident,
    variant_idents: &'a [Ident],
    new_variant_idents: &'a [Ident],
    replacements: Vec<(Expr, Expr)>,
}

impl<'a> Visit<'a> for EnumVariantUsageVisitor<'a> {
    fn visit_expr(&mut self, expr: &'a Expr) {
        if let Expr::Path(expr_path) = expr {
            if expr_path.path.segments.len() == 2 {
                let enum_segment = &expr_path.path.segments[0];
                let variant_segment = &expr_path.path.segments[1];
                if enum_segment.ident == *self.enum_ident {
                    for (i, variant_ident) in self.variant_idents.iter().enumerate() {
                        if variant_segment.ident == *variant_ident {
                            let new_variant_ident = &self.new_variant_idents[i];
                            let new_expr: Expr = parse_quote! {
                                #enum_segment :: #new_variant_ident
                            };
                            self.replacements.push((expr.clone(), new_expr));
                        }
                    }
                }
            }
        }
        syn::visit::visit_expr(self, expr);
    }
}

fn replace_expr(item_fn: &mut ItemFn, old_expr: &Expr, new_expr: Expr) {
    let mut replacer = ExprReplacer {
        old_expr,
        new_expr,
    };
    replacer.visit_item_fn_mut(item_fn);
}

struct ExprReplacer<'a> {
    old_expr: &'a Expr,
    new_expr: Expr,
}

impl VisitMut for ExprReplacer<'_> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if expr == self.old_expr {
            *expr = self.new_expr.clone();
        } else {
            syn::visit_mut::visit_expr_mut(self, expr);
        }
    }
}