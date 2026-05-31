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

pub struct Modify_Trait_Bound_With_Generic_Const_Expressions_363;

impl Mutator for Modify_Trait_Bound_With_Generic_Const_Expressions_363 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_With_Generic_Const_Expressions_363"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let syn::Type::Reference(type_reference) = &**ty {
                        if let syn::Type::TraitObject(type_trait_object) = &*type_reference.elem {
                            for bound in &type_trait_object.bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    let mut new_segments = Punctuated::new();
                                    for segment in &trait_bound.path.segments {
                                        let new_segment = if segment.ident == "MyTrait" {
                                            let mut new_args = Punctuated::new();
                                            if let syn::PathArguments::AngleBracketed(
                                                angle_bracketed_generic_arguments,
                                            ) = &segment.arguments
                                            {
                                                for arg in
                                                    &angle_bracketed_generic_arguments.args
                                                {
                                                    let new_arg = if let syn::GenericArgument::Const(
                                                        expr,
                                                    ) = arg
                                                    {
                                                        if let syn::Expr::Block(expr_block) = expr {
                                                            let new_block = syn::ExprBlock {
                                                                attrs: expr_block.attrs.clone(),
                                                                label: expr_block.label.clone(),
                                                                block: syn::Block {
                                                                    brace_token: expr_block
                                                                        .block
                                                                        .brace_token,
                                                                    stmts: expr_block
                                                                        .block
                                                                        .stmts
                                                                        .clone(),
                                                                },
                                                            };
                                                            let new_expr =
                                                                syn::Expr::Block(new_block);
                                                            syn::GenericArgument::Const(new_expr)
                                                        } else {
                                                            arg.clone()
                                                        }
                                                    } else {
                                                        arg.clone()
                                                    };
                                                    new_args.push(new_arg);
                                                }
                                            }
                                            syn::PathSegment {
                                                ident: segment.ident.clone(),
                                                arguments: syn::PathArguments::AngleBracketed(
                                                    syn::AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: new_args,
                                                        gt_token: token::Gt::default(),
                                                    },
                                                ),
                                            }
                                        } else {
                                            segment.clone()
                                        };
                                        new_segments.push(new_segment);
                                    }
                                    let new_bound = syn::TypeParamBound::Trait(syn::TraitBound {
                                        paren_token: trait_bound.paren_token,
                                        modifier: trait_bound.modifier.clone(),
                                        lifetimes: trait_bound.lifetimes.clone(),
                                        path: syn::Path {
                                            leading_colon: trait_bound.path.leading_colon,
                                            segments: new_segments,
                                        },
                                    });
                                    let mut new_bounds = Punctuated::new();
                                    for bound in &type_trait_object.bounds {
                                        new_bounds.push(bound.clone());
                                    }
                                    let new_trait_object = syn::TypeTraitObject {
                                        dyn_token: type_trait_object.dyn_token,
                                        bounds: new_bounds,
                                    };
                                    let new_type = syn::Type::TraitObject(new_trait_object);
                                    let new_ref = syn::TypeReference {
                                        and_token: type_reference.and_token,
                                        lifetime: type_reference.lifetime.clone(),
                                        mutability: type_reference.mutability,
                                        elem: Box::new(new_type),
                                    };
                                    *ty = Box::new(syn::Type::Reference(new_ref));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions returning references to trait objects with generic const expressions. It identifies the trait bound involving a generic const expression and modifies the const expression to use different arithmetic or logical operations, such as changing a literal to an arithmetic expression. This transformation stresses the compiler's const expression evaluation within trait bounds, potentially revealing bugs in const evaluation or trait resolution."
    }
}