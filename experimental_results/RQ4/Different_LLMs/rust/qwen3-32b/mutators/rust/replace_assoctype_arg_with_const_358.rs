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

pub struct Replace_AssocType_Arg_With_Const_358;

impl Mutator for Replace_AssocType_Arg_With_Const_358 {
    fn name(&self) -> &str {
        "Replace_AssocType_Arg_With_Const_358"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for bound in &mut func.sig.generics.params {
                    if let GenericParam::Type(ty_param) = bound {
                        for bound in &mut ty_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                for segment in &mut trait_bound.path.segments {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::AssocType(assoc_type) = arg {
                                                if let Type::Path(type_path) = &mut assoc_type.ty {
                                                    for segment in &mut type_path.path.segments {
                                                        if let PathArguments::AngleBracketed(assoc_args) = &mut segment.arguments {
                                                            for assoc_arg in &mut assoc_args.args {
                                                                if let GenericArgument::Type(ty) = assoc_arg {
                                                                    let expr = parse_quote!(42);
                                                                    *assoc_arg = GenericArgument::Const(Expr::Lit(expr));
                                                                    break;
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
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for pred in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(ty_pred) = pred {
                            if let syn::Type::Path(type_path) = &mut ty_pred.bounded_ty {
                                for segment in &mut type_path.path.segments {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::AssocType(assoc_type) = arg {
                                                if let Type::Path(type_path) = &mut assoc_type.ty {
                                                    for segment in &mut type_path.path.segments {
                                                        if let PathArguments::AngleBracketed(assoc_args) = &mut segment.arguments {
                                                            for assoc_arg in &mut assoc_args.args {
                                                                if let GenericArgument::Type(ty) = assoc_arg {
                                                                    let expr = parse_quote!(42);
                                                                    *assoc_arg = GenericArgument::Const(Expr::Lit(expr));
                                                                    break;
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
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for bound in &mut method.sig.generics.params {
                            if let GenericParam::Type(ty_param) = bound {
                                for bound in &mut ty_param.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        for segment in &mut trait_bound.path.segments {
                                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                for arg in &mut args.args {
                                                    if let GenericArgument::AssocType(assoc_type) = arg {
                                                        if let Type::Path(type_path) = &mut assoc_type.ty {
                                                            for segment in &mut type_path.path.segments {
                                                                if let PathArguments::AngleBracketed(assoc_args) = &mut segment.arguments {
                                                                    for assoc_arg in &mut assoc_args.args {
                                                                        if let GenericArgument::Type(ty) = assoc_arg {
                                                                            let expr = parse_quote!(42);
                                                                            *assoc_arg = GenericArgument::Const(Expr::Lit(expr));
                                                                            break;
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
                        if let Some(where_clause) = &mut method.sig.generics.where_clause {
                            for pred in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(ty_pred) = pred {
                                    if let syn::Type::Path(type_path) = &mut ty_pred.bounded_ty {
                                        for segment in &mut type_path.path.segments {
                                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                for arg in &mut args.args {
                                                    if let GenericArgument::AssocType(assoc_type) = arg {
                                                        if let Type::Path(type_path) = &mut assoc_type.ty {
                                                            for segment in &mut type_path.path.segments {
                                                                if let PathArguments::AngleBracketed(assoc_args) = &mut segment.arguments {
                                                                    for assoc_arg in &mut assoc_args.args {
                                                                        if let GenericArgument::Type(ty) = assoc_arg {
                                                                            let expr = parse_quote!(42);
                                                                            *assoc_arg = GenericArgument::Const(Expr::Lit(expr));
                                                                            break;
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}