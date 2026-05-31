use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct GATConcreteTypeSubstitution_131;

impl Mutator for GATConcreteTypeSubstitution_131 {
    fn name(&self) -> &str {
        "GATConcreteTypeSubstitution_131"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            for bound in &mut type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("FnMut") {
                                        if let Some(BoundLifetimes { .. }) = trait_bound.lifetimes {
                                            if let Some(segment) = trait_bound.path.segments.last() {
                                                if let PathArguments::AngleBracketed(args) = &mut *segment.arguments {
                                                    for arg in args.args.iter_mut() {
                                                        if let GenericArgument::Type(ty) = arg {
                                                            if let Type::Path(type_path) = &*ty {
                                                                let segments = &type_path.path.segments;
                                                                if segments.len() >= 2 && segments[0].ident == "Self" {
                                                                    let assoc_type_name = &segments[1].ident;
                                                                    let struct_name = assoc_type_name.to_string().replace("Ref", "");
                                                                    let mut lifetime = None;
                                                                    if let PathArguments::AngleBracketed(generic_args) = &segments[1].arguments {
                                                                        for generic_arg in &generic_args.args {
                                                                            if let GenericArgument::Lifetime(lt) = generic_arg {
                                                                                lifetime = Some(lt.clone());
                                                                                break;
                                                                            }
                                                                        }
                                                                    }
                                                                    if let Some(lt) = lifetime {
                                                                        let new_type = parse_quote! { &'lt struct_name };
                                                                        *ty = new_type;
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
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function parameters using GATs with higher-ranked lifetimes in FnMut bounds. It substitutes the GAT reference (Self::AssociatedType) with a concrete reference type (e.g., &'b Foo), preserving the for<'b> quantifier. This transformation challenges the compiler's handling of GATs, trait bounds, and lifetime elision, potentially exposing bugs in type inference and trait resolution."
    }
}