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

pub struct Modify_Lifetime_Parameter_In_Trait_Implementation_31;

impl Mutator for Modify_Lifetime_Parameter_In_Trait_Implementation_31 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Parameter_In_Trait_Implementation_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_lifetime_map = std::collections::HashMap::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                let trait_ident = &item_trait.ident;
                let mut lifetime_params = Vec::new();
                for param in &item_trait.generics.params {
                    if let GenericParam::Lifetime(lifetime) = param {
                        lifetime_params.push(lifetime.lifetime.clone());
                    }
                }
                trait_lifetime_map.insert(trait_ident.clone(), lifetime_params);
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut target_trait = None;
                if let Some((_, path, _)) = &item_impl.trait_ {
                    target_trait = Some(path);
                }
                if let Some(target_trait) = target_trait {
                    let trait_ident = target_trait.segments.last().unwrap().ident.clone();
                    if let Some(lifetime_params) = trait_lifetime_map.get(&trait_ident) {
                        if !lifetime_params.is_empty() {
                            let mut new_lifetime_params = lifetime_params.clone();
                            new_lifetime_params[0] = Lifetime::new("'other_lt", Span::call_site());
                            let mut new_trait_path = target_trait.clone();
                            new_trait_path.segments.last_mut().unwrap().arguments =
                                PathArguments::AngleBracketed(
                                    AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt::default(),
                                        args: {
                                            let mut args = Punctuated::new();
                                            for lifetime in &new_lifetime_params {
                                                args.push(GenericArgument::Lifetime(
                                                    lifetime.clone(),
                                                ));
                                            }
                                            args
                                        },
                                        gt_token: token::Gt::default(),
                                    },
                                );
                            item_impl.trait_ = Some((
                                None,
                                new_trait_path,
                                token::For::default(),
                            ));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations that involve lifetime parameters. It identifies traits with lifetime parameters and modifies the corresponding lifetime arguments in their implementations to introduce mismatches. By changing the lifetime used in the implementation to a different one than expected by the trait definition, it creates a potential for lifetime resolution failures, aiming to stress-test the compiler's borrow checker and trait system."
    }
}