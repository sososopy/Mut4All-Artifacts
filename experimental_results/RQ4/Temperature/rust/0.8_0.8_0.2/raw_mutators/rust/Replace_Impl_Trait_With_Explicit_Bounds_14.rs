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
    token, token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Impl_Trait_With_Explicit_Bounds_14;

impl Mutator for Replace_Impl_Trait_With_Explicit_Bounds_14 {
    fn name(&self) -> &str {
        "Replace_Impl_Trait_With_Explicit_Bounds_14"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if let Some(first_bound) = type_impl_trait.bounds.first() {
                            if let TypeParamBound::Trait(trait_bound) = first_bound {
                                let trait_ident = trait_bound.path.segments.last().unwrap().ident.clone();
                                item_fn.sig.generics.params.push(GenericParam::Type(parse_quote!(T: #trait_ident)));
                                *return_type = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(Ident::new("T", Span::call_site())),
                                }));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions with `impl Trait` return types and replaces them with an explicitly bounded generic type. By transforming `impl Trait` to a defined generic type, it tests the compiler's handling of trait bounds and generic type resolution."
    }
}