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

pub struct ReplaceConstGenericParameterWithNonConstGenericParameter_400;

impl Mutator for ReplaceConstGenericParameterWithNonConstGenericParameter_400 {
    fn name(&self) -> &str {
        "ReplaceConstGenericParameterWithNonConstGenericParameter_400"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_def) = item {
                if let Some(generics) = &mut struct_def.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Const(param) = param {
                            *param = GenericParam::Type(TypeParam {
                                attrs: param.attrs.clone(),
                                ident: param.ident.clone(),
                                colon_token: None,
                                bounds: Default::default(),
                                default: None,
                                eq_token: None,
                                default_eq_token: None,
                            });
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters with non-const generic parameters in struct definitions. This transformation tests the handling of non-const generic parameters in the Rust compiler and may help expose bugs related to the generic_const_exprs feature."
    }
}