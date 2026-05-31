use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitItem, TraitItemType, ImplItem, ImplItemType, ImplItemFn, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_ImplTrait_Generator_With_Concrete_122;

impl Mutator for Replace_ImplTrait_Generator_With_Concrete_122 {
    fn name(&self) -> &str {
        "Replace_ImplTrait_Generator_With_Concrete_122"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item_item in &mut trait_item.items {
                    if let TraitItem::Type(type_item) = trait_item_item {
                        if let Some((_, default_type)) = &type_item.default {
                            if let Type::ImplTrait(type_impl_trait) = default_type {
                                if let Some(generator_bound) = type_impl_trait.bounds.iter().find(|bound| {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        trait_bound.path.segments.last().map(|seg| seg.ident.to_string()) == Some("Generator".to_string())
                                    } else {
                                        false
                                    }
                                }) {
                                    let generator_bound = generator_bound.clone();
                                    let mut impl_blocks = Vec::new();
                                    for item in &mut file.items {
                                        if let Item::Impl(impl_item) = item {
                                            if let Some(trait_path) = &impl_item.trait_ {
                                                if trait_path.1.segments.last().map(|seg| seg.ident.to_string()) == Some(trait_item.ident.to_string()) {
                                                    for impl_item_item in &mut impl_item.items {
                                                        if let ImplItem::Type(type_item) = impl_item_item {
                                                            if type_item.ident == type_item.ident {
                                                                if let Some((_, default_type)) = &type_item.default {
                                                                if let Type::ImplTrait(_) = default_type {
                                                                for impl_item_item2 in &mut impl_item.items {
                                                    if let ImplItem::Fn(func_item) = impl_item_item2 {
                                                        if let ReturnType::Type(_, return_type) = &func_item.sig.output {
                                                            if let Type::Path(type_path) = &**return_type {
                                                                if type_path.path.segments.last().map(|seg| seg.ident.to_string()) == Some(type_item.ident.to_string()) {
                                                                let mut closure_found = false;
                                                                for stmt in &func_item.block.stmts {
                                                                if let Stmt::Expr(expr, _) = stmt {
                                                                if let Expr::Closure(closure) = expr {
                                                                closure_found = true;
                                                                }
                                                                }
                                                                }
                                                                if closure_found {
                                                                impl_blocks.push(impl_item.clone());
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
                    }
                }
            }
        }
        for impl_block in impl_blocks {
            let concrete_gen_ident = Ident::new("ConcreteGen", Span::call_site());
            let new_type_alias: Item = parse_quote! {
                type #concrete_gen_ident = impl Generator;
            };
            new_items.push(new_type_alias);
            for impl_item_item in &mut impl_block.items {
                if let ImplItem::Type(type_item) = impl_item_item {
                    if type_item.ident == type_item.ident {
                        type_item.default = Some((parse_quote! { = }, parse_quote! { #concrete_gen_ident }));
                    }
                }
                if let ImplItem::Fn(func_item) = impl_item_item {
                    if let ReturnType::Type(_, return_type) = &mut func_item.sig.output {
                        if let Type::Path(type_path) = &**return_type {
                            if type_path.path.segments.last().map(|seg| seg.ident.to_string()) == Some(type_item.ident.to_string()) {
                                *return_type = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(concrete_gen_ident),
                                }));
                            }
                        }
                    }
                }
            }
            file.items.push(Item::Impl(impl_block));
        }
        for new_item in new_items {
            file.items.push(new_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait associated types defined as `impl Generator`. It identifies trait implementations where the associated type is used as a return type of a function that returns a generator closure. It then introduces a new type alias `ConcreteGen = impl Generator` at module scope and replaces the associated type definition and the function return type with this alias. This transformation stresses the compiler's handling of generator type resolution and debuginfo generation when impl Trait is aliased through multiple levels, potentially exposing ICEs related to type forwarding and metadata registration."
    }
}