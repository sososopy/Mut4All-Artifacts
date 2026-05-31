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

pub struct Replace_Box_Dyn_With_HRTB_557;

impl Mutator for Replace_Box_Dyn_With_HRTB_557 {
    fn name(&self) -> &str {
        "Replace_Box_Dyn_With_HRTB_557"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    for field in &mut item_struct.fields {
                        if let Type::Path(type_path) = &mut field.ty {
                            if let Some(last_segment) = type_path.path.segments.last_mut() {
                                if last_segment.ident == "Box" {
                                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                        if let Some(GenericArgument::Type(Type::TraitObject(trait_object))) = args.args.first_mut() {
                                            if trait_object.dyn_token.is_some() {
                                                trait_object.bounds.push(TypeParamBound::Lifetime(Lifetime::new("'a", Span::call_site())));
                                                trait_object.dyn_token = Some(token::Dyn { span: Span::call_site() });
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Item::Fn(item_fn) => {
                    for input in &mut item_fn.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &mut *pat_type.ty {
                                if let Some(last_segment) = type_path.path.segments.last_mut() {
                                    if last_segment.ident == "Box" {
                                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                            if let Some(GenericArgument::Type(Type::TraitObject(trait_object))) = args.args.first_mut() {
                                                if trait_object.dyn_token.is_some() {
                                                    trait_object.bounds.push(TypeParamBound::Lifetime(Lifetime::new("'a", Span::call_site())));
                                                    trait_object.dyn_token = Some(token::Dyn { span: Span::call_site() });
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets `Box<dyn Trait>` instances in structs and function parameters, replacing them with `Box<dyn for<'a> Trait + 'a>`. This transformation introduces explicit higher-rank trait bounds, challenging the compiler's lifetime resolution and testing its handling of trait objects with specified lifetimes."
    }
}