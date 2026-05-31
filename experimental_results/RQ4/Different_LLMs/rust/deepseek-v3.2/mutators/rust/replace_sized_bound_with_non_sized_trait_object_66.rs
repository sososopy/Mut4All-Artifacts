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

pub struct Replace_Sized_Bound_With_Non_Sized_Trait_Object_66;

impl Mutator for Replace_Sized_Bound_With_Non_Sized_Trait_Object_66 {
    fn name(&self) -> &str {
        "Replace_Sized_Bound_With_Non_Sized_Trait_Object_66"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if struct_item.generics.params.len() > 0 {
                    let mut found_tuple_field = false;
                    for field in &mut struct_item.fields {
                        if let Type::Tuple(tuple_type) = &mut field.ty {
                            if tuple_type.elems.len() >= 1 {
                                found_tuple_field = true;
                                break;
                            }
                        }
                    }
                    if found_tuple_field {
                        for generic_param in &mut struct_item.generics.params {
                            if let GenericParam::Type(type_param) = generic_param {
                                if type_param.bounds.len() == 0 {
                                    type_param.bounds.push(parse_quote!(?Sized));
                                }
                            }
                        }
                        for field in &mut struct_item.fields {
                            if let Type::Tuple(tuple_type) = &mut field.ty {
                                if tuple_type.elems.len() >= 1 {
                                    let first_elem = &mut tuple_type.elems[0];
                                    let mut should_replace = false;
                                    let mut trait_object = None;
                                    if let Type::Path(type_path) = first_elem {
                                        if let Some(segment) = type_path.path.segments.last() {
                                            for generic_param in &struct_item.generics.params {
                                                if let GenericParam::Type(type_param) = generic_param {
                                                    if segment.ident.to_string() == type_param.ident.to_string() {
                                                        let trait_bound = TraitBound {
                                                            paren_token: None,
                                                            modifier: TraitBoundModifier::None,
                                                            lifetimes: None,
                                                            path: syn::Path::from(Ident::new("Copy", Span::call_site())),
                                                        };
                                                        trait_object = Some(TypeTraitObject {
                                                            dyn_token: Some(token::Dyn::default()),
                                                            bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(trait_bound)]),
                                                        });
                                                        should_replace = true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if should_replace && trait_object.is_some() {
                                        *first_elem = Type::TraitObject(trait_object.unwrap());
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
        "The mutation operator targets struct definitions with generic parameters used in tuple fields. It adds a `?Sized` bound to unconstrained type parameters and replaces the first element of tuple fields with a `dyn Copy` trait object when that element matches the generic type. This introduces a dynamically-sized type in a non-final tuple position, creating layout computation issues during const-eval for static/const contexts. The transformation specifically uses a trait that requires `Self: Sized` (like Copy) as a trait object, violating object safety and triggering compiler errors."
    }
}