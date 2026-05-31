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

pub struct Modify_Trait_Bound_In_Return_Type_168;

impl Mutator for Modify_Trait_Bound_In_Return_Type_168 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_In_Return_Type_168"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut new_bounds = Punctuated::new();
                        for bound in &type_impl_trait.bounds {
                            new_bounds.push(bound.clone());
                        }
                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: {
                                    let mut segments = Punctuated::new();
                                    segments.push(PathSegment {
                                        ident: Ident::new("CoerceUnsized", Span::call_site()),
                                        arguments: PathArguments::None,
                                    });
                                    segments
                                },
                            },
                        }));
                        type_impl_trait.bounds = new_bounds;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the trait bound in the return type of functions using `impl Trait`. It adds an additional `CoerceUnsized` trait bound to the existing bounds. This transformation is designed to test the compiler's handling of trait object coercion and the interaction between `impl Trait` and trait bounds, potentially leading to mismatches or ICEs."
    }
}