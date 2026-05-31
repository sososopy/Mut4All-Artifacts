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

pub struct Replace_ImplTrait_Return_With_Impl_Yield_38;

impl Mutator for Replace_ImplTrait_Return_With_Impl_Yield_38 {
    fn name(&self) -> &str {
        "Replace_ImplTrait_Return_With_Impl_Yield_38"
    }
    fn mutate(&self, file: &mut syn::File) {
        let has_generators = file.attrs.iter().any(|attr| {
            if let Some(segment) = attr.path().segments.last() {
                if segment.ident == "feature" {
                    let tokens_str = attr.to_token_stream().to_string();
                    tokens_str.contains("generators")
                } else {
                    false
                }
            } else {
                false
            }
        });

        if !has_generators {
            let new_attr = parse_quote! { #![feature(generators)] };
            file.attrs.insert(0, new_attr);
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut new_bounds = Punctuated::new();
                        let yield_bound = parse_quote! { Yield };
                        new_bounds.push_value(yield_bound);
                        *return_type = Box::new(Type::ImplTrait(TypeImplTrait {
                            impl_token: type_impl_trait.impl_token.clone(),
                            bounds: new_bounds,
                        }));

                        let mut new_block = func.block.clone();
                        let yield_expr = parse_quote! { yield };
                        let yield_stmt = Stmt::Expr(yield_expr, None);
                        new_block.stmts.insert(0, yield_stmt);
                        func.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}