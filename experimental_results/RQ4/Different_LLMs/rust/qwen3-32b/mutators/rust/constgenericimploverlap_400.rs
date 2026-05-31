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

pub struct ConstGenericImplOverlap_400;

impl Mutator for ConstGenericImplOverlap_400 {
    fn name(&self) -> &str {
        "ConstGenericImplOverlap_400"
    }
    fn mutate(&self, file: &mut syn::File) {
        let feature_attr = parse_quote! { #![feature(generic_const_exprs)] };
        file.attrs.push(feature_attr);

        let mut new_impls = Vec::new();
        
        for item in &mut file.items {
            if let Item::Struct(struct_def) = item {
                let const_params: Vec<_> = struct_def.generics.params.iter()
                    .filter_map(|param| match param {
                        GenericParam::Const(c) => Some(c),
                        _ => None,
                    })
                    .collect();
                if const_params.len() >= 2 {
                    let struct_name = &struct_def.ident;

                    let mut generic_args_tokens = TokenStream::new();
                    for (i, _) in const_params.iter().enumerate() {
                        if i > 0 {
                            generic_args_tokens.extend(quote! { , });
                        }
                        if i == 0 {
                            generic_args_tokens.extend(quote! { undefined_const });
                        } else if i == 1 {
                            generic_args_tokens.extend(quote! { [[usize; 4]; 4] });
                        } else {
                            generic_args_tokens.extend(quote! { 0 });
                        }
                    }

                    let impl_tokens = quote! {
                        impl #struct_name<#generic_args_tokens> { }
                    };

                    let new_impl = parse_quote! { #impl_tokens };
                    new_impls.push(Item::Impl(new_impl));
                }
            }
        }
        
        file.items.extend(new_impls);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}