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

pub struct Replace_Closure_Arg_With_AssocProj_498;

impl Mutator for Replace_Closure_Arg_With_AssocProj_498 {
    fn name(&self) -> &str {
        "Replace_Closure_Arg_With_AssocProj_498"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut suitable_traits = Vec::new();
        for item in &mut file.items {
            if let Item::Trait(trait_def) = item {
                if !trait_def.generics.lifetimes().collect::<Vec<_>>().is_empty() && has_associated_type(trait_def) {
                    suitable_traits.push(trait_def);
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            for bound in &mut predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(last_segment) = trait_bound.path.segments.last() {
                                        let trait_name = last_segment.ident.to_string();
                                        if ["Fn", "FnMut", "FnOnce"].contains(&trait_name.as_str()) {
                                            if let PathArguments::Parenthesized(parens) = &mut last_segment.arguments {
                                                let args = &mut parens.inputs;
                                                if let Some(first_arg) = args.first_mut() {
                                                    if let Some(trait_info) = suitable_traits.first() {
                                                        let projection = create_projection(trait_info);
                                                        *first_arg = projection;
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

fn has_associated_type(trait_def: &syn::ItemTrait) -> bool {
    for item in &trait_def.items {
        if let syn::TraitItem::Type(_) = item {
            return true;
        }
    }
    false
}

fn create_projection(trait_info: &syn::ItemTrait) -> Type {
    let mut args = Punctuated::new();
    for param in &trait_info.generics.params {
        if let GenericParam::Type(_) = param {
            args.push(parse_quote!(i32));
        }
    }
    let path_args = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
        colon2_token: None,
        lt_token: token::Lt::default(),
        args,
        gt_token: token::Gt::default(),
    });
    let trait_segment = PathSegment {
        ident: trait_info.ident.clone(),
        arguments: path_args,
    };
    let trait_path = SynPath {
        leading_colon: None,
        segments: Punctuated::from_iter(vec![trait_segment]),
    };
    let tokens = quote! { <F as #trait_path>::Type };
    syn::parse2(tokens).unwrap()
}