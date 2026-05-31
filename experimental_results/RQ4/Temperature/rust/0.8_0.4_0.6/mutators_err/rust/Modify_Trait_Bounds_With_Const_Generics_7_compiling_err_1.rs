use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Trait_Bounds_With_Const_Generics_7;

impl Mutator for Modify_Trait_Bounds_With_Const_Generics_7 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_With_Const_Generics_7"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let syn::TraitItem::Type(trait_item_type) = item {
                        for bound in &mut trait_item_type.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                for segment in &mut trait_bound.path.segments {
                                    if let syn::PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                        for arg in &mut args.args {
                                            if let syn::GenericArgument::Const(expr) = arg {
                                                if let syn::Expr::Lit(expr_lit) = expr {
                                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                        let new_value = lit_int.base10_parse::<i32>().unwrap() + 1;
                                                        *expr = syn::Expr::Lit(syn::ExprLit {
                                                            attrs: vec![],
                                                            lit: syn::Lit::Int(syn::LitInt::new(&new_value.to_string(), Span::call_site())),
                                                        });
                                                    }
                                                }
                                            }
                                            if let syn::GenericArgument::Type(ref mut ty) = arg {
                                                if let syn::Type::Path(type_path) = ty {
                                                    if type_path.path.is_ident("()") {
                                                        *ty = parse_quote!(bool);
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