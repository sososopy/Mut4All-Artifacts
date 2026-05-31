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

pub struct Change_TypeAlias_ImplTrait_To_Concrete_482;

impl Mutator for Change_TypeAlias_ImplTrait_To_Concrete_482 {
    fn name(&self) -> &str {
        "Change_TypeAlias_ImplTrait_To_Concrete_482"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AliasReplacer<'a> {
            alias_ident: &'a Ident,
            concrete_type: Type,
        }

        impl<'a> VisitMut for AliasReplacer<'a> {
            fn visit_type_mut(&mut self, node: &mut Type) {
                if let Type::Path(type_path) = node {
                    if type_path.qself.is_none()
                        && type_path.path.segments.len() == 1
                        && type_path.path.segments[0].ident == *self.alias_ident
                    {
                        *node = self.concrete_type.clone();
                    }
                }
                syn::visit_mut::visit_type_mut(self, node);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut alias_candidates = Vec::new();
                for (index, item) in item_fn.block.stmts.iter().enumerate() {
                    if let Stmt::Item(Item::Type(item_type)) = item {
                        if let Type::ImplTrait(_) = &*item_type.ty {
                            alias_candidates.push(index);
                        }
                    }
                }

                let mut new_stmts = item_fn.block.stmts.clone();
                for index in alias_candidates {
                    if let Stmt::Item(Item::Type(item_type)) = &mut new_stmts[index] {
                        let alias_ident = &item_type.ident;
                        let concrete_type: Type = if let Type::ImplTrait(type_impl_trait) =
                            &*item_type.ty
                        {
                            if type_impl_trait.bounds.iter().any(|bound| {
                                matches!(
                                    bound,
                                    TypeParamBound::Trait(TraitBound {
                                        modifier: TraitBoundModifier::None,
                                        ..
                                    })
                                )
                            }) {
                                parse_quote! { (T,) }
                            } else {
                                parse_quote! { u32 }
                            }
                        } else {
                            continue;
                        };
                        item_type.ty = Box::new(concrete_type.clone());
                        let mut replacer = AliasReplacer {
                            alias_ident,
                            concrete_type,
                        };
                        replacer.visit_block_mut(&mut item_fn.block);
                    }
                }
                item_fn.block.stmts = new_stmts;
            } else if let Item::Mod(item_mod) = item {
                if let Some((_, items)) = &mut item_mod.content {
                    let mut alias_candidates = Vec::new();
                    for (index, item) in items.iter().enumerate() {
                        if let Item::Type(item_type) = item {
                            if let Type::ImplTrait(_) = &*item_type.ty {
                                alias_candidates.push(index);
                            }
                        }
                    }

                    let mut new_items = items.clone();
                    for index in alias_candidates {
                        if let Item::Type(item_type) = &mut new_items[index] {
                            let alias_ident = &item_type.ident;
                            let concrete_type: Type = if let Type::ImplTrait(type_impl_trait) =
                                &*item_type.ty
                            {
                                if type_impl_trait.bounds.iter().any(|bound| {
                                    matches!(
                                        bound,
                                        TypeParamBound::Trait(TraitBound {
                                            modifier: TraitBoundModifier::None,
                                            ..
                                        })
                                    )
                                }) {
                                    parse_quote! { (i32,) }
                                } else {
                                    parse_quote! { u32 }
                                }
                            } else {
                                continue;
                            };
                            item_type.ty = Box::new(concrete_type.clone());
                            let mut replacer = AliasReplacer {
                                alias_ident,
                                concrete_type,
                            };
                            for item in &mut *items {
                                replacer.visit_item_mut(item);
                            }
                        }
                    }
                    *items = new_items;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets type aliases using `impl Trait` within functions and modules, replacing them with concrete types like tuples or primitives. It ensures that all usages of the alias are updated to match the new concrete type, aiming to expose weaknesses in type inference and trait resolution by enforcing explicit type constraints."
    }
}