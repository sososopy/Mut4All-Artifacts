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

pub struct Modify_Lifetime_Variance_638;

impl Mutator for Modify_Lifetime_Variance_638 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Variance_638"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_lifetime = false;
                let mut lifetime_variance = None;

                if let Some(generics) = &mut item_struct.generics.params {
                    for param in generics {
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            has_lifetime = true;
                            if thread_rng().gen_bool(0.5) {
                                lifetime_variance = Some(TraitBoundModifier::Plus);
                            } else {
                                lifetime_variance = Some(TraitBoundModifier::Minus);
                            }
                        }
                    }
                }

                if has_lifetime {
                    if let Some(fields) = &mut item_struct.fields {
                        for field in fields.iter_mut() {
                            if let Type::Reference(type_reference) = &mut field.ty {
                                if let Some(lifetime) = &type_reference.lifetime {
                                    if let Some(variance) = lifetime_variance {
                                        *lifetime = Lifetime {
                                            apostrophe: lifetime.apostrophe,
                                            ident: Ident::new(
                                                &format!("{}{}", lifetime.ident, variance),
                                                lifetime.ident.span(),
                                            ),
                                        };
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
        "The mutation operator targets structs with lifetime parameters and modifies their lifetime variance. By introducing explicit covariance or contravariance, it aims to explore edge cases in Rust's type-checking and lifetime resolution, potentially exposing subtle issues in the compiler's handling of lifetime variance."
    }
}