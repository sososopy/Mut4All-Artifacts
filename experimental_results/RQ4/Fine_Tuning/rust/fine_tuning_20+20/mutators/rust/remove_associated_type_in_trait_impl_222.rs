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

pub struct Remove_Associated_Type_In_Trait_Impl_222;

impl Mutator for Remove_Associated_Type_In_Trait_Impl_222 {
    fn name(&self) -> &str {
        "Remove_Associated_Type_In_Trait_Impl_222"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    let trait_ident = path.segments.last().unwrap().ident.to_string();
                    if trait_ident == "TryFrom" || trait_ident == "Into" {
                        let mut has_associated_type = false;
                        item_impl.items.retain(|impl_item| {
                            if let ImplItem::Type(_) = impl_item {
                                has_associated_type = true;
                                false
                            } else {
                                true
                            }
                        });
                        if has_associated_type {
                            for impl_item in &mut item_impl.items {
                                if let ImplItem::Fn(method) = impl_item {
                                    if method.sig.ident == "try_from" {
                                        if let ReturnType::Type(_, ty) = &mut method.sig.output {
                                            if let Type::Path(type_path) = &mut **ty {
                                                if let Some(last) =
                                                    type_path.path.segments.last_mut()
                                                {
                                                    if last.ident == "Error" {
                                                        last.ident = Ident::new(
                                                            "PathBuf",
                                                            Span::call_site(),
                                                        );
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
        "The mutator targets trait implementations of `TryFrom` or `Into` by removing associated types and replacing them with concrete types in method signatures. This transformation creates type mismatches and tests the compiler's handling of associated type resolution and substitution in trait contexts."
    }
}