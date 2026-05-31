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

pub struct Modify_Type_Param_Usage_In_Return_31;

impl Mutator for Modify_Type_Param_Usage_In_Return_31 {
    fn name(&self) -> &str {
        "Modify_Type_Param_Usage_In_Return_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if let Some(TypeParamBound::Trait(trait_bound)) = type_impl_trait.bounds.first() {
                            if let Some(segment) = trait_bound.path.segments.last() {
                                let generic_args = &segment.arguments;
                                if let PathArguments::AngleBracketed(angle_bracketed_args) = generic_args {
                                    if let Some(GenericArgument::Type(Type::Path(type_path))) = angle_bracketed_args.args.first() {
                                        if let Some(last_segment) = type_path.path.segments.last() {
                                            let original_type = &last_segment.ident;
                                            let new_type_ident = Ident::new("U", Span::call_site());
                                            let new_type: ItemStruct = parse_quote! {
                                                struct U;
                                            };
                                            file.items.push(Item::Struct(new_type));
                                            let new_generic_argument = GenericArgument::Type(Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath::from(new_type_ident),
                                            }));
                                            let mut new_args = angle_bracketed_args.clone();
                                            new_args.args = Punctuated::from_iter(vec![new_generic_argument]);
                                            let new_segment = PathSegment {
                                                ident: segment.ident.clone(),
                                                arguments: PathArguments::AngleBracketed(new_args),
                                            };
                                            let mut new_path = trait_bound.path.clone();
                                            new_path.segments.pop();
                                            new_path.segments.push(new_segment);
                                            let new_trait_bound = TraitBound {
                                                paren_token: trait_bound.paren_token,
                                                modifier: trait_bound.modifier,
                                                lifetimes: trait_bound.lifetimes.clone(),
                                                path: new_path,
                                            };
                                            let new_type_impl_trait = TypeImplTrait {
                                                impl_token: type_impl_trait.impl_token,
                                                bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(new_trait_bound)]),
                                            };
                                            *return_type = Box::new(Type::ImplTrait(new_type_impl_trait));
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
        "This mutator targets functions returning `impl Trait` where the trait is implemented by a data structure with a generic type parameter. It replaces the generic type parameter with a new type `U`, declared as a simple struct. This transformation explores type mismatches and tests the compiler's handling of type parameter substitutions and trait implementations."
    }
}