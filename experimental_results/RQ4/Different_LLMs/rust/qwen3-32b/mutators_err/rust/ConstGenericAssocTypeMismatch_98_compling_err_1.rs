use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprArray, ExprCall, ExprLit, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct ConstGenericAssocTypeMismatch_98;

impl Mutator for ConstGenericAssocTypeMismatch_98 {
    fn name(&self) -> &str {
        "ConstGenericAssocTypeMismatch_98"
    }
    fn mutate(&self, file: &mut File) {
        let mut traits_with_assoc_consts = Vec::new();
        
        // First collect all traits with associated constants
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                let trait_name = &trait_item.ident;
                let mut assoc_consts = Vec::new();
                for item in &trait_item.items {
                    if let TraitItem::Const(trait_const) = item {
                        assoc_consts.push((trait_const.ident.clone(), trait_const.ty.clone()));
                    }
                }
                if !assoc_consts.is_empty() {
                    traits_with_assoc_consts.push((trait_name.clone(), assoc_consts));
                }
            }
        }

        // Process each impl block
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                // Find if this impl is for a trait with associated constants
                let mut trait_info = None;
                for (trait_name, assoc_consts) in &traits_with_assoc_consts {
                    if let Some(trait_path) = &impl_item.trait_ {
                        if let TraitPath { path, .. } = trait_path {
                            if path.ident == trait_name {
                                trait_info = Some((trait_name, assoc_consts));
                                break;
                            }
                        }
                    }
                }

                if let Some((trait_name, assoc_consts)) = trait_info {
                    // Find the first generic const parameter in the impl
                    let mut generic_const_type = None;
                    let mut self_ty = None;
                    for param in &impl_item.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            generic_const_type = Some(const_param.ty.clone());
                            break;
                        }
                    }
                    self_ty = Some(impl_item.self_ty.clone());

                    if let Some(u_type) = generic_const_type {
                        let self_ty = self_ty.unwrap();
                        // Check each associated const in the trait
                        for (const_name, t_type) in assoc_consts {
                            // Compare T and U types
                            let t_type_str = quote! { #t_type }.to_string();
                            let u_type_str = quote! { #u_type }.to_string();
                            if t_type_str != u_type_str {
                                // Find the first array expression with a literal length
                                for item in &mut file.items {
                                    if let Item::Fn(func) = item {
                                        for stmt in &mut func.block.stmts {
                                            if let Stmt::Expr(expr) = stmt {
                                                if let Expr::Array(expr_array) = &mut **expr {
                                                    if let Some(len_expr) = &mut expr_array.len {
                                                        if let Expr::Lit(len_lit) = &mut **len_expr {
                                                            // Replace the literal with the associated const
                                                            let new_expr = parse_quote!(<#self_ty as #trait_name>::#const_name);
                                                            *len_expr = Box::new(new_expr);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if let Item::Impl(impl_item2) = item {
                                        for impl_item in &mut impl_item2.items {
                                            if let ImplItem::Fn(func) = impl_item {
                                                for stmt in &mut func.block.stmts {
                                                    if let Stmt::Expr(expr) = stmt {
                                                        if let Expr::Array(expr_array) = &mut **expr {
                                                            if let Some(len_expr) = &mut expr_array.len {
                                                                if let Expr::Lit(len_lit) = &mut **len_expr {
                                                                    let new_expr = parse_quote!(<#self_ty as #trait_name>::#const_name);
                                                                    *len_expr = Box::new(new_expr);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies trait implementations with a generic const parameter of type U and an associated const of type T. It then finds array expressions with literal lengths and replaces the literal with the associated const, creating a type mismatch between T and U. This forces the compiler to relate constants of different types during array length validation, exposing potential ICE scenarios in const evaluation and trait resolution when type relationships are incorrectly inferred."
    }
}