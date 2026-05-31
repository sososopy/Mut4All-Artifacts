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

pub struct Replace_Extern_With_Impl_201;

impl Mutator for Replace_Extern_With_Impl_201 {
    fn name(&self) -> &str {
        "Replace_Extern_With_Impl_201"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for method in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = method {
                        if let Some(extern_token) = method.sig.extern_token {
                            // Create a new trait bound for the method
                            let trait_bound = TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: method.sig.ident.clone().into(),
                            };

                            // Create a new type parameter bound for the method
                            let type_param_bound = TypeParamBound::Trait(trait_bound);

                            // Create a new where clause predicate for the method
                            let where_predicate = WherePredicate::Type(PredicateType {
                                lifetimes: None,
                                bounded_ty: method.sig.ident.clone().into(),
                                colon_token: token::Colon::default(),
                                bounds: punctuated::Punctuated::from_iter(vec![type_param_bound]),
                            });

                            // Create a new where clause for the method
                            let where_clause = WhereClause {
                                where_token: token::Where::default(),
                                predicates: punctuated::Punctuated::from_iter(vec![where_predicate]),
                            };

                            // Replace the extern method with a new method that has the where clause
                            *method = syn::TraitItem::Method(syn::MethodSig {
                                constness: method.sig.constness,
                                unsafety: method.sig.unsafety,
                                abi: method.sig.abi,
                                ident: method.sig.ident,
                                generics: method.sig.generics,
                                inputs: method.sig.inputs,
                                variadic: method.sig.variadic,
                                output: method.sig.output,
                                where_clause: Some(where_clause),
                            });

                            // Create a new impl block for the trait
                            let impl_item = syn::Item::Impl(syn::Impl {
                                attrs: vec![],
                                defaultness: None,
                                unsafety: method.sig.unsafety,
                                impl_token: token::Impl::default(),
                                of_token: token::Of::default(),
                                for_token: token::For::default(),
                                generics: method.sig.generics,
                                trait_: Some((method.sig.ident.clone().into(), None, None)),
                                self_ty: method.sig.ident.clone().into(),
                                bracket_token: token::Brace::default(),
                                items: punctuated::Punctuated::from_iter(vec![syn::ImplItem::Method(syn::Method {
                                    sig: syn::MethodSig {
                                        constness: method.sig.constness,
                                        unsafety: method.sig.unsafety,
                                        abi: method.sig.abi,
                                        ident: method.sig.ident,
                                        generics: method.sig.generics,
                                        inputs: method.sig.inputs,
                                        variadic: method.sig.variadic,
                                        output: method.sig.output,
                                        where_clause: Some(where_clause),
                                    },
                                    block: syn::Block {
                                        brace_token: token::Brace::default(),
                                        stmts: punctuated::Punctuated::new(),
                                    },
                                })]),
                            });

                            // Add the new impl block to the file
                            file.items.push(impl_item);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces extern methods in traits with impl blocks. It creates a new where clause for the method and adds a new impl block to the file. This transformation aims to test the compiler's ability to handle trait implementations and where clauses."
    }
}