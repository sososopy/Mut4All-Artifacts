use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Const_Generic_Parameter_With_Another_Constant_99;

impl Mutator for Replace_Const_Generic_Parameter_With_Another_Constant_99 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Parameter_With_Another_Constant_99"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for generic_param in &mut trait_item.generics.params {
                    if let GenericParam::Const(const_param) = generic_param {
                        let new_const_value = match &const_param.default {
                            Some(default_value) => {
                                let mut new_value = default_value.clone();
                                // Replace the constant value with another constant value
                                // For example, replace a constant integer with another integer
                                if let Expr::Lit(lit_expr) = &new_value {
                                    if let Lit::Int(int_lit) = &lit_expr.lit {
                                        let new_int_value = thread_rng().gen_range(1..100);
                                        Expr::Lit(Lit::Int(IntLit::new(new_int_value.to_string(), int_lit.span())))
                                    } else {
                                        new_value.clone()
                                    }
                                } else {
                                    new_value.clone()
                                }
                            }
                            None => {
                                // If there is no default value, generate a new constant value
                                // For example, generate a random integer
                                let new_int_value = thread_rng().gen_range(1..100);
                                Expr::Lit(Lit::Int(IntLit::new(new_int_value.to_string(), Span::call_site())))
                            }
                        };
                        const_param.default = Some(new_const_value);
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for generic_param in &mut func.sig.generics.params {
                            if let GenericParam::Const(const_param) = generic_param {
                                let new_const_value = match &const_param.default {
                                    Some(default_value) => {
                                        let mut new_value = default_value.clone();
                                        // Replace the constant value with another constant value
                                        // For example, replace a constant integer with another integer
                                        if let Expr::Lit(lit_expr) = &new_value {
                                            if let Lit::Int(int_lit) = &lit_expr.lit {
                                                let new_int_value = thread_rng().gen_range(1..100);
                                                Expr::Lit(Lit::Int(IntLit::new(new_int_value.to_string(), int_lit.span())))
                                            } else {
                                                new_value.clone()
                                            }
                                        } else {
                                            new_value.clone()
                                        }
                                    }
                                    None => {
                                        // If there is no default value, generate a new constant value
                                        // For example, generate a random integer
                                        let new_int_value = thread_rng().gen_range(1..100);
                                        Expr::Lit(Lit::Int(IntLit::new(new_int_value.to_string(), Span::call_site())))
                                    }
                                };
                                const_param.default = Some(new_const_value);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant generic parameters with another constant value. This transformation aims to test the compiler's handling of constant generics and its ability to resolve type constraints under different constant values."
    }
}