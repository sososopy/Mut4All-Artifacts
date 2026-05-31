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

pub struct Adjust_Lifetime_Parameters_In_Trait_Implementations_301;

impl Mutator for Adjust_Lifetime_Parameters_In_Trait_Implementations_301 {
    fn name(&self) -> &str {
        "Adjust_Lifetime_Parameters_In_Trait_Implementations_301"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_methods: Vec<(Ident, Vec<Lifetime>)> = Vec::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Fn(trait_fn) = item {
                        let mut lifetimes = Vec::new();
                        for generic in &trait_fn.sig.generics.params {
                            if let GenericParam::Lifetime(lt) = generic {
                                lifetimes.push(lt.lifetime.clone());
                            }
                        }
                        if !lifetimes.is_empty() {
                            trait_methods.push((trait_fn.sig.ident.clone(), lifetimes));
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(impl_fn) = impl_item {
                        if let Some((_, trait_path, _)) = &item_impl.trait_ {
                            let trait_ident = trait_path.segments.last().unwrap().ident.clone();
                            let mut target_method_lifetimes = Vec::new();
                            for (method_ident, lifetimes) in &trait_methods {
                                if *method_ident == impl_fn.sig.ident {
                                    target_method_lifetimes = lifetimes.clone();
                                    break;
                                }
                            }
                            if !target_method_lifetimes.is_empty() {
                                let mut impl_lifetimes = Vec::new();
                                for generic in &impl_fn.sig.generics.params {
                                    if let GenericParam::Lifetime(lt) = generic {
                                        impl_lifetimes.push(lt.lifetime.clone());
                                    }
                                }
                                if impl_lifetimes == target_method_lifetimes {
                                    impl_fn.sig.generics.params = Punctuated::new();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait methods with lifetime parameters and their corresponding implementations. It identifies methods in traits that use lifetimes and ensures their implementations in types do not match these lifetimes, creating a mismatch. This stresses the compiler's lifetime resolution and substitution mechanisms, potentially uncovering bugs related to lifetime management in trait implementations."
    }
}