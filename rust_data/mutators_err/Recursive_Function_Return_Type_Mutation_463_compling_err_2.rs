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

pub struct Recursive_Function_Return_Type_Mutation_463;

impl Mutator for Recursive_Function_Return_Type_Mutation_463 {
    fn name(&self) -> &str {
        "Recursive_Function_Return_Type_Mutation_463"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        if item_fn.block.stmts.iter().any(|stmt| {
                            if let Stmt::Expr(Expr::Call(ExprCall { func, .. }), _) = stmt {
                                if let Expr::Path(ExprPath { path, .. }) = &**func {
                                    path.is_ident(&item_fn.sig.ident)
                                } else {
                                    false
                                }
                            } else {
                                false
                            }
                        }) {
                            let new_trait_ident = Ident::new("NewTrait", Span::call_site());
                            let new_trait = parse_quote! {
                                trait #new_trait_ident {}
                            };
                            let impl_trait = parse_quote! {
                                impl #new_trait_ident for u32 {}
                            };
                            file.items.push(Item::Trait(new_trait));
                            file.items.push(Item::Impl(impl_trait));
                            item_fn.sig.output = ReturnType::Type(
                                token::RArrow { spans: [Span::call_site(), Span::call_site()] },
                                Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(new_trait_ident),
                                })),
                            );
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with recursive calls that return `impl Trait`. It replaces the `impl Trait` return type with a concrete trait `NewTrait`, implemented by a type such as `u32`. This transformation forces the compiler to handle specific trait implementations, potentially exposing bugs in type resolution and trait handling. By converting the return type to a concrete trait, it challenges the compiler's ability to manage recursive functions and opaque return types."
    }
}