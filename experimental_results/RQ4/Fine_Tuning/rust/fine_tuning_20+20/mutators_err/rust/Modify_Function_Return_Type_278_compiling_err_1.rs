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

pub struct Modify_Function_Return_Type_278;

impl Mutator for Modify_Function_Return_Type_278 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_278"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let trait_bound = type_impl_trait.bounds.iter().find_map(|bound| {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                Some(trait_bound)
                            } else {
                                None
                            }
                        });
                        if let Some(trait_bound) = trait_bound {
                            let mut trait_name = trait_bound.path.segments.last().unwrap().ident.to_string();
                            trait_name.push_str("Replacement");
                            let struct_ident = Ident::new(&trait_name, Span::call_site());
                            let struct_def = ItemStruct {
                                attrs: Vec::new(),
                                vis: item_fn.vis.clone(),
                                struct_token: token::Struct {
                                    span: Span::call_site(),
                                },
                                ident: struct_ident.clone(),
                                generics: item_fn.sig.generics.clone(),
                                fields: syn::Fields::Unit,
                                semi_token: Some(token::Semi {
                                    spans: [Span::call_site()],
                                }),
                            };
                            let impl_def = Item::Impl(ItemImpl {
                                attrs: Vec::new(),
                                defaultness: None,
                                unsafety: None,
                                impl_token: token::Impl {
                                    span: Span::call_site(),
                                },
                                generics: item_fn.sig.generics.clone(),
                                trait_: Some((
                                    TraitBoundModifier::None,
                                    syn::Path {
                                        leading_colon: trait_bound.path.leading_colon,
                                        segments: trait_bound.path.segments.clone(),
                                    },
                                    token::For {
                                        span: Span::call_site(),
                                    },
                                )),
                                self_ty: Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path::from(struct_ident.clone()),
                                })),
                                brace_token: token::Brace {
                                    span: Span::call_site(),
                                },
                                items: Vec::new(),
                            });
                            let mut new_items = Vec::new();
                            new_items.push(Item::Struct(struct_def));
                            new_items.push(impl_def);
                            for stmt in &item_fn.block.stmts {
                                if let Stmt::Item(item) = stmt {
                                    new_items.push(item.clone());
                                }
                            }
                            item_fn.block.stmts.retain(|stmt| !matches!(stmt, Stmt::Item(_)));
                            file.items.splice(0..0, new_items);
                            item_fn.sig.output = ReturnType::Type(
                                token::RArrow {
                                    spans: [Span::call_site()],
                                },
                                Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path::from(struct_ident),
                                })),
                            );
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with `impl Trait` return types by replacing them with a new struct that implements the same trait. It analyzes the trait bound, generates a uniquely named struct, and injects it into the module. The function's return type is then updated to use this new struct, preserving the original trait contract while altering the concrete return type. This transformation tests the compiler's handling of opaque type resolution and trait implementation in a more explicit form."
    }
}