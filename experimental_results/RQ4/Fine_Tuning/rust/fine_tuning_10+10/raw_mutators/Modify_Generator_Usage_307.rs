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

pub struct Modify_Generator_Usage_307;

impl Mutator for Modify_Generator_Usage_307 {
    fn name(&self) -> &str {
        "Modify_Generator_Usage_307"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        if let Some((_, bounds)) = &mut type_item.bounds.split_last_mut() {
                            for bound in bounds {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("Generator") {
                                        for segment in &mut trait_bound.path.segments {
                                            if segment.ident == "Generator" {
                                                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                    for arg in &mut args.args {
                                                        if let GenericArgument::Binding(binding) = arg {
                                                            if binding.ident == "Yield" {
                                                                binding.ty = parse_quote!(u32);
                                                            } else if binding.ident == "Return" {
                                                                binding.ty = parse_quote!(String);
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
            if let Item::Impl(impl_item) = item {
                if let Some((_, path, _)) = &impl_item.trait_ {
                    if path.is_ident("Runnable") {
                        for item in &mut impl_item.items {
                            if let syn::ImplItem::Type(type_item) = item {
                                if type_item.ident == "Gen" {
                                    type_item.ty = parse_quote!(impl Generator<Yield = u32, Return = String>);
                                }
                            }
                            if let syn::ImplItem::Method(method) = item {
                                if method.sig.ident == "run" {
                                    method.block = parse_quote!({
                                        move || {
                                            yield 1;
                                            return "done".to_string();
                                        }
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies the usage of the Generator trait in trait and implementation blocks. It identifies traits with associated types defined as impl Generator<Yield = (), Return = ()> and changes them to use different Yield and Return types, such as u32 and String. The corresponding implementations are updated to match the new types but introduce a runtime logic mismatch, aiming to trigger bugs related to type inference and generator logic handling."
    }
}