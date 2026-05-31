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

pub struct Modify_Function_Pointer_Type_639;

impl Mutator for Modify_Function_Pointer_Type_639 {
    fn name(&self) -> &str {
        "Modify_Function_Pointer_Type_639"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                if let Type::Path(type_path) = &*item_type.ty {
                    if let Some(last_segment) = type_path.path.segments.last() {
                        if last_segment.ident == "Box" || last_segment.ident == "Rc" || last_segment.ident == "Arc" {
                            if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                                if let Some(GenericArgument::Type(Type::TraitObject(trait_object))) = args.args.first() {
                                    if trait_object.bounds.iter().any(|bound| {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            trait_bound.path.segments.iter().any(|seg| seg.ident == "Fn")
                                        } else {
                                            false
                                        }
                                    }) {
                                        let new_trait_bound = TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: {
                                                    let mut segments = Punctuated::new();
                                                    segments.push(PathSegment {
                                                        ident: Ident::new("Fn", Span::call_site()),
                                                        arguments: PathArguments::AngleBracketed(parse_quote!(<i32>)),
                                                    });
                                                    segments
                                                },
                                            },
                                        });
                                        let mut new_bounds = Punctuated::new();
                                        new_bounds.push(new_trait_bound);
                                        let new_trait_object = Type::TraitObject(TypeTraitObject {
                                            dyn_token: trait_object.dyn_token.clone(),
                                            bounds: new_bounds,
                                        });
                                        item_type.ty = Box::new(new_trait_object);
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
        "The mutation operator modifies function pointer types encapsulated in smart pointers like Box, Rc, or Arc by changing the parameter type of the function pointer. This transformation aims to test the compiler's handling of function pointers and their type signatures, potentially exposing issues with type inference and lifetime management."
    }
}