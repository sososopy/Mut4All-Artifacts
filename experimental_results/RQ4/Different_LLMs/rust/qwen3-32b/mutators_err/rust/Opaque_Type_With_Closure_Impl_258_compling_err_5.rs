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

pub struct Opaque_Type_With_Closure_Impl_258;

impl Mutator for Opaque_Type_With_Closure_Impl_258 {
    fn name(&self) -> &str {
        "Opaque_Type_With_Closure_Impl_258"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_opaque_trait = false;
        let mut has_another_trait = false;
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.ident == "OpaqueTrait" {
                    has_opaque_trait = true;
                } else if trait_item.ident == "AnotherTrait" {
                    has_another_trait = true;
                }
            }
        }

        if !has_opaque_trait {
            let opaque_trait = parse_quote! {
                trait OpaqueTrait {}
            };
            file.items.push(Item::Trait(opaque_trait));
        }

        if !has_another_trait {
            let another_trait = parse_quote! {
                trait AnotherTrait {}
            };
            file.items.push(Item::Trait(another_trait));
        }

        let mut has_opaque_impl = false;
        for item in &file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(trait_path) = &impl_item.trait_ {
                    if trait_path.1.is_ident("OpaqueTrait") {
                        has_opaque_impl = true;
                    }
                }
            }
        }
        if !has_opaque_impl {
            let opaque_impl = parse_quote! {
                impl<T> OpaqueTrait for T {}
            };
            file.items.push(Item::Impl(opaque_impl));
        }

        let mut has_another_send_impl = false;
        for item in &file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(trait_path) = &impl_item.trait_ {
                    if trait_path.1.is_ident("AnotherTrait") {
                        if let Some(ty) = impl_item.generics.params.first() {
                            if let GenericParam::Type(ty_param) = ty {
                                if ty_param.bounds.iter().any(|bound| {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        trait_bound.path.is_ident("Send")
                                    } else {
                                        false
                                    }
                                }) {
                                    has_another_send_impl = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        if !has_another_send_impl {
            let another_send_impl = parse_quote! {
                impl<T: Send> AnotherTrait for T {}
            };
            file.items.push(Item::Impl(another_send_impl));
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(_) = **return_type {
                        continue;
                    }
                } else {
                    continue;
                }

                let block = &mut func.block;
                if block.stmts.len() != 1 {
                    continue;
                }
                if let Stmt::Expr(expr, _) = &block.stmts[0] {
                    if let Expr::Return(ret_expr) = expr {
                        if let Some(expr) = &ret_expr.expr {
                            if let Expr::Call(_) = &**expr {
                            } else {
                                continue;
                            }
                        } else {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } else {
                    continue;
                }

                let opaque_type_name = Ident::new(&format!("Opaque_{}", func.sig.ident), Span::call_site());
                let type_alias = parse_quote! {
                    type #opaque_type_name = impl OpaqueTrait;
                };
                let index = file.items.iter().position(|i| i == item).unwrap();
                file.items.insert(index, Item::Type(type_alias));

                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    *return_type = Box::new(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(opaque_type_name.clone()),
                    }));
                }

                if let Stmt::Expr(expr, _) = &mut block.stmts[0] {
                    if let Expr::Return(ret_expr) = &mut **expr {
                        if let Some(expr) = &mut ret_expr.expr {
                            if let Expr::Call(call_expr) = &mut **expr {
                                let closure = parse_quote! {
                                    || #call_expr
                                };
                                *expr = Box::new(closure);
                            }
                        }
                    }
                }

                let impl_another_trait = parse_quote! {
                    impl AnotherTrait for #opaque_type_name {}
                };
                file.items.push(Item::Impl(impl_another_trait));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an opaque type with a closure returning an existing function's output and adds specialized trait implementations for the opaque type. This transformation leverages `impl Trait` and trait specialization to create complex interactions between closure types, opaque return types, and trait coherence, potentially exposing compiler bugs in type inference and specialization logic."
    }
}