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

pub struct Modify_Const_Generics_With_Default_Values_166;

impl Mutator for Modify_Const_Generics_With_Default_Values_166 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_With_Default_Values_166"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                let mut modified = false;
                for param in &mut trait_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            const_param.default = None;
                            modified = true;
                            break;
                        }
                    }
                }

                if modified {
                    for impl_item in &mut file.items {
                        if let Item::Impl(impl_block) = impl_item {
                            if let Type::Path(TypePath { path, .. }) = &*impl_block.self_ty {
                                if path.segments.last().unwrap().ident == trait_item.ident {
                                    for param in &mut impl_block.generics.params {
                                        if let GenericParam::Const(const_param) = param {
                                            if const_param.default.is_none() {
                                                let default_value = parse_quote!({ 0 }); // Placeholder for default value
                                                impl_block.generics.params.push(GenericParam::Const(syn::ConstParam {
                                                    attrs: Vec::new(),
                                                    const_token: token::Const::default(),
                                                    ident: const_param.ident.clone(),
                                                    colon_token: token::Colon::default(),
                                                    ty: const_param.ty.clone(),
                                                    eq_token: Some(token::Eq::default()),
                                                    default: Some(default_value),
                                                }));
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
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies trait definitions with const generics having default values, removes the default from one of the const generics, and updates the trait implementations to explicitly specify all const generics, including the one that previously had a default."
    }
}