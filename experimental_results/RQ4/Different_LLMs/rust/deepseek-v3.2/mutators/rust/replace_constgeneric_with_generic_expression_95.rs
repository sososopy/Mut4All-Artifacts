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

pub struct Replace_ConstGeneric_With_Generic_Expression_95;

impl Mutator for Replace_ConstGeneric_With_Generic_Expression_95 {
    fn name(&self) -> &str {
        "Replace_ConstGeneric_With_Generic_Expression_95"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for bound in &mut func.sig.generics.params {
                    if let syn::GenericParam::Type(type_param) = bound {
                        for bound in &mut type_param.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(segment) = trait_bound.path.segments.last_mut() {
                                    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let syn::GenericArgument::Const(const_arg) = arg {
                                                if let syn::Expr::Lit(lit) = &*const_arg {
                                                    if let syn::Lit::Int(_) = lit.lit {
                                                        let new_expr: syn::Expr = parse_quote! { std::mem::size_of::<T>() };
                                                        *const_arg = syn::Expr::Lit(syn::ExprLit {
                                                            attrs: Vec::new(),
                                                            lit: syn::Lit::Int(syn::LitInt::new("0", lit.lit.span())),
                                                        });
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
            if let syn::Item::Impl(impl_item) = item {
                for impl_item_fn in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item_fn {
                        for bound in &mut func.sig.generics.params {
                            if let syn::GenericParam::Type(type_param) = bound {
                                for bound in &mut type_param.bounds {
                                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(segment) = trait_bound.path.segments.last_mut() {
                                            if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                for arg in &mut args.args {
                                                    if let syn::GenericArgument::Const(const_arg) = arg {
                                                        if let syn::Expr::Lit(lit) = &*const_arg {
                                                            if let syn::Lit::Int(_) = lit.lit {
                                                                let new_expr: syn::Expr = parse_quote! { std::mem::size_of::<T>() };
                                                                *const_arg = syn::Expr::Lit(syn::ExprLit {
                                                                attrs: Vec::new(),
                                                                lit: syn::Lit::Int(syn::LitInt::new("0", lit.lit.span())),
                                                            });
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
        "The mutation operator targets trait bounds with const generic arguments of integer literal type. It replaces the const argument with a const expression that references a generic type parameter, such as `std::mem::size_of::<T>()`. This transformation stresses the compiler's ability to handle const generics that depend on generic types, potentially triggering bugs in const evaluation, type inference, or trait resolution when generic parameters are involved."
    }
}