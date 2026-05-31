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

pub struct Alter_Async_Closure_Return_Type_655;

impl Mutator for Alter_Async_Closure_Return_Type_655 {
    fn name(&self) -> &str {
        "Alter_Async_Closure_Return_Type_655"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if type_impl_trait.bounds.iter().any(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound.path.segments.last().map_or(false, |segment| {
                                    segment.ident == "Future"
                                })
                            } else {
                                false
                            }
                        }) {
                            *return_type = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: parse_quote!(Box<dyn Future<Output = u8>>),
                            }));

                            let new_block: syn::Block = parse_quote!({
                                Box::pin(async move || -> u8 { 42 })
                            });
                            item_fn.block = Box::new(new_block);
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