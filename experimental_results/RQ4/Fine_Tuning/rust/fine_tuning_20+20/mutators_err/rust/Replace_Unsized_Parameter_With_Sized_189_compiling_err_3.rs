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

pub struct Replace_Unsized_Parameter_With_Sized_189;

impl Mutator for Replace_Unsized_Parameter_With_Sized_189 {
    fn name(&self) -> &str {
        "Replace_Unsized_Parameter_With_Sized_189"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &*pat_type.ty {
                            let mut new_bounds = Punctuated::new();
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(assoc_type) = trait_bound
                                        .path
                                        .segments
                                        .last()
                                        .and_then(|seg| match &seg.arguments {
                                            PathArguments::AngleBracketed(args) => {
                                                args.args.iter().find_map(|arg| {
                                                    if let GenericArgument::Constraint(
                                                        constraint,
                                                    ) = arg
                                                    {
                                                        Some(constraint)
                                                    } else {
                                                        None
                                                    }
                                                })
                                            }
                                            _ => None,
                                        })
                                    {
                                        if assoc_type.ident == "Item" {
                                            let mut new_constraint = assoc_type.clone();
                                            new_constraint.colon_token = Some(Default::default());
                                            new_constraint.bounds = Punctuated::new();
                                            new_constraint.bounds.push(TypeParamBound::Type(
                                                Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath::from(Ident::new(
                                                        "String",
                                                        Span::call_site(),
                                                    )),
                                                }),
                                            ));
                                            let mut new_segment = trait_bound
                                                .path
                                                .segments
                                                .last()
                                                .unwrap()
                                                .clone();
                                            new_segment.arguments =
                                                PathArguments::AngleBracketed(
                                                    parse_quote!(<#new_constraint>),
                                                );
                                            let mut new_trait_bound = trait_bound.clone();
                                            new_trait_bound.path.segments.pop();
                                            new_trait_bound.path.segments.push(new_segment);
                                            new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                        } else {
                                            new_bounds.push(bound.clone());
                                        }
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                } else {
                                    new_bounds.push(bound.clone());
                                }
                            }
                            pat_type.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: Default::default(),
                                bounds: new_bounds,
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets function parameters using `impl Trait` with unsized associated types, such as `impl Iterator<Item = str>`. It replaces the unsized type (e.g., `str`) with a sized equivalent (e.g., `String`). This transformation forces a shift from unsized to sized type constraints, potentially revealing issues in type layout, trait resolution, and compiler handling of size assumptions."
    }
}