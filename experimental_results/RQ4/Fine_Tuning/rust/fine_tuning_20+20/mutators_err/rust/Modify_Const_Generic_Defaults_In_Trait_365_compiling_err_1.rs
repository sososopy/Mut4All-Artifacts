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

pub struct Modify_Const_Generic_Defaults_In_Trait_365;

impl Mutator for Modify_Const_Generic_Defaults_In_Trait_365 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Defaults_In_Trait_365"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_trait: Option<Ident> = None;
        let mut target_index: Option<usize> = None;
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for (i, param) in item_trait.generics.params.iter().enumerate() {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            target_trait = Some(item_trait.ident.clone());
                            target_index = Some(i);
                            break;
                        }
                    }
                }
            }
        }
        if let Some(target_trait) = target_trait {
            for item in &mut file.items {
                if let Item::Trait(item_trait) = item {
                    if item_trait.ident == target_trait {
                        if let Some(target_index) = target_index {
                            if let Some(GenericParam::Const(const_param)) =
                                item_trait.generics.params.get_mut(target_index)
                            {
                                const_param.default = None;
                            }
                        }
                    }
                }
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if path.is_ident(&target_trait) {
                            if let Some(target_index) = target_index {
                                if let Some((_, ref mut args)) = path.segments.last_mut() {
                                    if let PathArguments::AngleBracketed(args) = args {
                                        if args.args.len() == item_impl.generics.params.len() {
                                            if let Some(GenericParam::Const(const_param)) =
                                                item_impl.generics.params.get(target_index)
                                            {
                                                args.args.push(GenericArgument::Const(
                                                    Expr::Path(ExprPath {
                                                        attrs: Vec::new(),
                                                        qself: None,
                                                        path: SynPath::from(const_param.ident.clone()),
                                                    }),
                                                ));
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
        "This mutator targets traits with const generics that have default values. It removes the default value from one const generic in the trait definition and updates the corresponding impl to explicitly specify a value for that const generic. This transformation tests the compiler's handling of const generic defaults and their removal, potentially exposing issues in generic resolution and trait implementation."
    }
}