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

pub struct Replace_Const_Generic_Bound_With_Non_Const_Generic_Bound_133;

impl Mutator for Replace_Const_Generic_Bound_With_Non_Const_Generic_Bound_133 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Bound_With_Non_Const_Generic_Bound_133"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for (i, bound) in trait_item.supertraits.iter_mut().enumerate() {
                    if let TypeParamBound::Trait(TraitBound {
                        paren_token: _,
                        lifetimes: _,
                        path: syn_path,
                        modifier: _,
                    }) = bound {
                        if let Some(segment) = syn_path.segments.last() {
                            if let PathArguments::AngleBracketed(angle_bracketed) = &segment.arguments {
                                let mut new_args = angle_bracketed.args.clone();
                                for (i, a) in new_args.iter_mut().enumerate() {
                                    if let GenericArgument::Const(_) = a {
                                        let new_arg = GenericArgument::Type(Type::Path(TypePath {
                                            qself: None,
                                            path: syn_path.clone(),
                                        }));
                                        *a = new_arg.clone();
                                    }
                                }
                                let new_path_arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                    colon2_token: angle_bracketed.colon2_token,
                                    lt_token: angle_bracketed.lt_token,
                                    args: new_args,
                                    gt_token: angle_bracketed.gt_token,
                                });
                                if let Some(segment_mut) = syn_path.segments.last_mut() {
                                    segment_mut.arguments = new_path_arguments;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic bounds with non-const generic bounds in trait definitions. This transformation can help expose bugs related to the handling of const generics and associated types."
    }
}