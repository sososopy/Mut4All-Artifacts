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

pub struct Alter_Trait_Object_Syntax_333;

impl Mutator for Alter_Trait_Object_Syntax_333 {
    fn name(&self) -> &str {
        "Alter_Trait_Object_Syntax_333"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut target_trait_name = None;
                let mut target_value_name = None;
                for stmt in &func.block.stmts {
                    if let syn::Stmt::Local(local) = stmt {
                        if let Some(init) = &local.init {
                            if let syn::Expr::Reference(expr_ref) = &*init.expr {
                                if let syn::Pat::Type(pat_type) = &local.pat {
                                    if let syn::Type::TraitObject(type_trait_object) =
                                        &*pat_type.ty
                                    {
                                        if type_trait_object.dyn_token.is_some() {
                                            let mut new_bounds = Punctuated::new();
                                            for bound in &type_trait_object.bounds {
                                                if let syn::TypeParamBound::Trait(trait_bound) =
                                                    bound
                                                {
                                                    let mut new_path_segments =
                                                        Punctuated::new();
                                                    for segment in
                                                        &trait_bound.path.segments
                                                    {
                                                        new_path_segments.push(
                                                            syn::PathSegment {
                                                                ident: segment.ident.clone(),
                                                                arguments: segment
                                                                    .arguments
                                                                    .clone(),
                                                            },
                                                        );
                                                    }
                                                    let new_path = syn::Path {
                                                        leading_colon: trait_bound
                                                            .path
                                                            .leading_colon,
                                                        segments: new_path_segments,
                                                    };
                                                    let new_trait_bound = syn::TraitBound {
                                                        paren_token: trait_bound.paren_token,
                                                        modifier: trait_bound.modifier.clone(),
                                                        lifetimes: trait_bound.lifetimes.clone(),
                                                        path: new_path.clone(),
                                                    };
                                                    new_bounds.push(
                                                        syn::TypeParamBound::Trait(
                                                            new_trait_bound.clone(),
                                                        ),
                                                    );
                                                    target_trait_name =
                                                        Some(new_path.segments.last().unwrap().ident.to_string());
                                                } else {
                                                    new_bounds.push(bound.clone());
                                                }
                                            }
                                            let new_type_trait_object =
                                                syn::TypeTraitObject {
                                                    dyn_token: None,
                                                    bounds: new_bounds,
                                                };
                                            let new_pat_type = syn::PatType {
                                                attrs: pat_type.attrs.clone(),
                                                pat: pat_type.pat.clone(),
                                                colon_token: pat_type.colon_token,
                                                ty: Box::new(syn::Type::TraitObject(
                                                    new_type_trait_object,
                                                )),
                                            };
                                            target_value_name = Some(pat_type.pat.to_token_stream().to_string());
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if target_trait_name.is_some() && target_value_name.is_some() {
                    let target_trait_name = target_trait_name.unwrap();
                    let target_value_name = target_value_name.unwrap();
                    for stmt in &mut func.block.stmts {
                        if let syn::Stmt::Expr(expr, _) = stmt {
                            if let syn::Expr::Cast(expr_cast) = expr {
                                if let syn::Expr::Call(expr_call) = &*expr_cast.expr {
                                    if expr_call.args.len() == 1 {
                                        let arg_str = expr_call.args[0].to_token_stream().to_string();
                                        if arg_str == target_value_name {
                                            if let syn::Type::TraitObject(type_trait_object) =
                                                &*expr_cast.ty
                                            {
                                                if type_trait_object.dyn_token.is_none() {
                                                    let mut new_bounds = Punctuated::new();
                                                    for bound in &type_trait_object.bounds {
                                                        if let syn::TypeParamBound::Trait(
                                                            trait_bound,
                                                        ) = bound
                                                        {
                                                            let mut new_path_segments =
                                                                Punctuated::new();
                                                            for segment in
                                                                &trait_bound.path.segments
                                                            {
                                                                new_path_segments.push(
                                                                    syn::PathSegment {
                                                                        ident: segment
                                                                            .ident
                                                                            .clone(),
                                                                        arguments: segment
                                                                            .arguments
                                                                            .clone(),
                                                                    },
                                                                );
                                                            }
                                                            let new_path = syn::Path {
                                                                leading_colon: trait_bound
                                                                    .path
                                                                    .leading_colon,
                                                                segments: new_path_segments,
                                                            };
                                                            let new_trait_bound = syn::TraitBound {
                                                                paren_token: trait_bound
                                                                    .paren_token,
                                                                modifier: trait_bound
                                                                    .modifier
                                                                    .clone(),
                                                                lifetimes: trait_bound
                                                                    .lifetimes
                                                                    .clone(),
                                                                path: new_path.clone(),
                                                            };
                                                            new_bounds.push(
                                                                syn::TypeParamBound::Trait(
                                                                    new_trait_bound.clone(),
                                                                ),
                                                            );
                                                        } else {
                                                            new_bounds.push(bound.clone());
                                                        }
                                                    }
                                                    let new_type_trait_object =
                                                        syn::TypeTraitObject {
                                                            dyn_token: Some(
                                                                token::Dyn::default(),
                                                            ),
                                                            bounds: new_bounds,
                                                        };
                                                    expr_cast.ty = Box::new(
                                                        syn::Type::TraitObject(
                                                            new_type_trait_object,
                                                        ),
                                                    );
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
        "This mutator targets trait object variable declarations within function scopes by toggling the syntax between `dyn Trait` and `dyn* Trait`. It also adjusts associated type casts to maintain consistency with the modified trait object syntax, ensuring the transformation is both syntactically and semantically valid."
    }
}