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

pub struct Replace_Ref_Var_Type_With_Impl_Trait_212;

impl Mutator for Replace_Ref_Var_Type_With_Impl_Trait_212 {
    fn name(&self) -> &str {
        "Replace_Ref_Var_Type_With_Impl_Trait_212"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Type(pat_type) = &*local.pat {
                            if let Type::Reference(original_ref) = &*pat_type.ty {
                                if let Some(LocalInit { expr: Some(expr), .. }) = &local.init {
                                    if let Expr::AddrOf(addr_of) = &**expr {
                                        let trait_path = parse_quote!(PartialEq);
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            path: trait_path,
                                            modifier: TraitBoundModifier::None,
                                            paren_token: None,
                                            lifetimes: None,
                                        }));
                                        let new_impl_trait = TypeImplTrait {
                                            impl_token: token::Impl::default(),
                                            bounds,
                                        };
                                        let new_ref_type = Type::Reference(TypeReference {
                                            and_token: original_ref.and_token.clone(),
                                            mutability: original_ref.mutability.clone(),
                                            lifetime: original_ref.lifetime.clone(),
                                            elem: Box::new(Type::ImplTrait(new_impl_trait)),
                                        });
                                        pat_type.ty = Box::new(new_ref_type);
                                    }
                                }
                            }
                        }
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for stmt in &mut method.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Pat::Type(pat_type) = &*local.pat {
                                    if let Type::Reference(original_ref) = &*pat_type.ty {
                                        if let Some(LocalInit { expr: Some(expr), .. }) = &local.init {
                                            if let Expr::AddrOf(addr_of) = &**expr {
                                                let trait_path = parse_quote!(PartialEq);
                                                let mut bounds = Punctuated::new();
                                                bounds.push(TypeParamBound::Trait(TraitBound {
                                                    path: trait_path,
                                                    modifier: TraitBoundModifier::None,
                                                    paren_token: None,
                                                    lifetimes: None,
                                                }));
                                                let new_impl_trait = TypeImplTrait {
                                                    impl_token: token::Impl::default(),
                                                    bounds,
                                                };
                                                let new_ref_type = Type::Reference(TypeReference {
                                                    and_token: original_ref.and_token.clone(),
                                                    mutability: original_ref.mutability.clone(),
                                                    lifetime: original_ref.lifetime.clone(),
                                                    elem: Box::new(Type::ImplTrait(new_impl_trait)),
                                                });
                                                pat_type.ty = Box::new(new_ref_type);
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