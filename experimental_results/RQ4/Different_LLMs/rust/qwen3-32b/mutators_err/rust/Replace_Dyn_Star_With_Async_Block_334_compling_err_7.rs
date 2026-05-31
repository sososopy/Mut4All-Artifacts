use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprAsync, ExprClosure, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemStatic, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Dyn_Star_With_Async_Block_334;

impl Mutator for Replace_Dyn_Star_With_Async_Block_334 {
    fn name(&self) -> &str {
        "Replace_Dyn_Star_With_Async_Block_334"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Type::TraitObject(trait_obj) = &*const_item.ty {
                    if trait_obj.dyn_token.is_some() {
                        for bound in &trait_obj.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Fn") {
                                    let return_type = extract_return_type_from_fn_trait(trait_bound);
                                    let new_expr = generate_async_block(return_type);
                                    *const_item.expr = *new_expr;
                                    break;
                                }
                            }
                        }
                    }
                }
            } else if let Item::Static(static_item) = item {
                if let Type::TraitObject(trait_obj) = &*static_item.ty {
                    if trait_obj.dyn_token.is_some() {
                        for bound in &trait_obj.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.is_ident("Fn") {
                                    let return_type = extract_return_type_from_fn_trait(trait_bound);
                                    let new_expr = generate_async_block(return_type);
                                    *static_item.expr = *new_expr;
                                    break;
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

fn extract_return_type_from_fn_trait(trait_bound: &TraitBound) -> Type {
    if let PathArguments::AngleBracketed(args) = &trait_bound.path.segments[0].arguments {
        for arg in &args.args {
            if let GenericArgument::Constraint(constraint) = arg {
                if constraint.ident == "Output" {
                    if let Some(first_bound) = constraint.value.first() {
                        if let TypeParamBound::Trait(trait_bound) = first_bound {
                            let path = &trait_bound.path;
                            let type_path = TypePath {
                                qself: None,
                                path: path.clone(),
                            };
                            return Type::Path(type_path);
                        }
                    }
                }
            }
        }
    }
    parse_quote!(())
}

fn generate_async_block(return_type: Type) -> Box<Expr> {
    match return_type {
        Type::Path(type_path) => {
            if type_path.path.is_ident("i32") {
                parse_quote! { async { 1 } }
            } else if type_path.path.is_ident("bool") {
                parse_quote! { async { true } }
            } else if type_path.path.is_ident("str") && type_path.qself.is_some() {
                parse_quote! { async { "" } }
            } else {
                parse_quote! { async { () } }
            }
        },
        _ => {
            parse_quote! { async { () } }
        }
    }
}