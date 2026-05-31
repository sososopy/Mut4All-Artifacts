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

pub struct Insert_Derive_Trait_Struct_255;

impl Mutator for Insert_Derive_Trait_Struct_255 {
    fn name(&self) -> &str {
        "Insert_Derive_Trait_Struct_255"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    for impl_item in &mut impl_item.items {
                        if let ImplItem::Fn(func) = impl_item {
                            if let Some(ref mut block) = func.block {
                                if block.stmts.is_empty() {
                                    continue;
                                }
                                let has_struct = block.stmts.iter().any(|stmt| {
                                    if let Stmt::Item(item) = stmt {
                                        matches!(item, Item::Struct(_))
                                    } else {
                                        false
                                    }
                                });
                                if has_struct {
                                    continue;
                                }
                                let trait_ident = trait_path.segments.last().unwrap().ident.clone();
                                let struct_item = parse_quote! {
                                    #[derive(#trait_ident)]
                                    struct LocalStruct {}
                                };
                                block.stmts.insert(0, Stmt::Item(struct_item));
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
}.