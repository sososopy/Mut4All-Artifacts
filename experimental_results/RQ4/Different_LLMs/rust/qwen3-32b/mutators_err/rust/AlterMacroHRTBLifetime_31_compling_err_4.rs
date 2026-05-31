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

pub struct AlterMacroHRTBLifetime_31;

impl Mutator for AlterMacroHRTBLifetime_31 {
    fn name(&self) -> &str {
        "AlterMacroHRTBLifetime_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::TraitObject(trait_object) = &mut **return_type {
                        for bound in &mut trait_object.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.modifier == syn::TraitBoundModifier::For {
                                    if let Some(lifetimes) = &trait_bound.lifetimes {
                                        if let Some(original_lt) = lifetimes.lifetimes.iter().next() {
                                            let new_lt = Lifetime::new("'lt2", Span::call_site());
                                            let new_lt_param = parse_quote!(#new_lt);
                                            let new_lifetimes = Punctuated::from_iter(vec![new_lt_param]);
                                            trait_bound.lifetimes = Some(parse_quote!(for < 'lt2 >));
                                            for segment in &mut trait_bound.path.segments {
                                                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                    for arg in &mut args.args {
                                                        if let GenericArgument::AssocType(assoc_type) = arg {
                                                            if assoc_type.ident == "T" {
                                                                let original_type = assoc_type.ty.clone();
                                                                let new_type_tokens = quote! { <#original_type as SomeTrait<#original_lt>>::NestedType };
                                                                let new_type = syn::parse2(new_type_tokens).unwrap();
                                                                assoc_type.ty = new_type;
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro-generated higher-ranked trait objects (HRTB) with nested associated types. It modifies the macro expansion to use a different lifetime variable in the HRTB than in the associated type, creating a mismatch between the higher-ranked lifetime and the actual parameter. This forces the borrow checker to resolve inconsistent lifetime relationships, exposing potential issues in the compiler's lifetime equate logic."
    }
}