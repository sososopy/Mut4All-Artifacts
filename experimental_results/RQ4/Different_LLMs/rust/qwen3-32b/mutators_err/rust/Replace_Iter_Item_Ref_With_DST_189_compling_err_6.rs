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

pub struct Replace_Iter_Item_Ref_With_DST_189;

impl Mutator for Replace_Iter_Item_Ref_With_DST_189 {
    fn name(&self) -> &str {
        "Replace_Iter_Item_Ref_With_DST_189"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(reference) = pat_type.ty.as_ref() {
                            if reference.mutability.is_some() {
                                if let Type::ImplTrait(impl_trait) = reference.elem.as_ref() {
                                    for bound in &mut impl_trait.bounds {
                                        if let TypeParamBound::Trait(trait_bound) = bound {
                                            if let Some(last) = trait_bound.path.segments.last() {
                                                if last.ident == "Iterator" {
                                                    if let PathArguments::AngleBracketed(angle_args) = &mut last.arguments {
                                                        for arg in &mut angle_args.args {
                                                            if let GenericArgument::AssocType(assoc_type) = arg {
                                                                if assoc_type.ident == "Item" {
                                                                    if let Type::Reference(ref_ty) = &*assoc_type.ty {
                                                                        assoc_type.ty = parse_quote!(str);
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
        ""
    }
}