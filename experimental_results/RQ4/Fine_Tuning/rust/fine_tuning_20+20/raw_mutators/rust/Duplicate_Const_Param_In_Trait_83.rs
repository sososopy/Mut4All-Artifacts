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

pub struct Duplicate_Const_Param_In_Trait_83;

impl Mutator for Duplicate_Const_Param_In_Trait_83 {
    fn name(&self) -> &str {
        "Duplicate_Const_Param_In_Trait_83"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let const_params: Vec<_> = trait_item
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let GenericParam::Const(const_param) = param {
                            Some(const_param.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if const_params.is_empty() {
                    continue;
                }
                let mut rng = thread_rng();
                let chosen_param = const_params.choose(&mut rng).unwrap();
                let new_const_param = GenericParam::Const(syn::ConstParam {
                    attrs: vec![],
                    const_token: token::Const {
                        span: Span::call_site(),
                    },
                    ident: chosen_param.ident.clone(),
                    colon_token: token::Colon {
                        spans: [Span::call_site()],
                    },
                    ty: chosen_param.ty.clone(),
                    eq_token: Some(token::Eq {
                        spans: [Span::call_site()],
                    }),
                    default: Some(Box::new(Type::Path(TypePath {
                        qself: None,
                        path: syn::Path::from(chosen_param.ident.clone()),
                    }))),
                });
                trait_item.generics.params.push(new_const_param);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies traits with const generic parameters and duplicates one of them, reusing its name and type while setting the default value to the original parameter. This transformation exploits a known compiler bug related to const parameter name reuse, potentially causing compilation errors or ICEs."
    }
}