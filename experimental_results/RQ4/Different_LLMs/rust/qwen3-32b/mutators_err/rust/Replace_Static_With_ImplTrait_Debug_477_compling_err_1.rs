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

pub struct Replace_Static_With_ImplTrait_Debug_477;

impl Mutator for Replace_Static_With_ImplTrait_Debug_477 {
    fn name(&self) -> &str {
        "Replace_Static_With_ImplTrait_Debug_477"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                if static_item.mutability.is_some() {
                    let alias_name = Ident::new("Alias_Debug_477", Span::call_site());
                    let trait_path = parse_quote! { core::fmt::Debug };
                    let alias_type = TypeImplTrait {
                        impl_token: token::Impl { span: Span::call_site() },
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: trait_path,
                            }));
                            bounds
                        },
                    };
                    let alias_item = Item::Type(syn::ItemType {
                        attrs: vec![],
                        vis: parse_quote! { pub },
                        ident: alias_name.clone(),
                        generics: parse_quote! {},
                        eq_token: parse_quote! { = },
                        ty: Box::new(Type::ImplTrait(alias_type)),
                    });
                    let index = file.items.iter().position(|i| i == item).unwrap();
                    file.items.insert(index, Box::new(alias_item));
                    let function_name = Ident::new("dummy_477", Span::call_site());
                    let function_item = Item::Fn(syn::ItemFn {
                        attrs: vec![],
                        vis: parse_quote! { pub },
                        sig: syn::Signature {
                            constness: None,
                            asyncness: None,
                            unsafety: None,
                            abi: None,
                            fn_token: token::Fn::default(),
                            ident: function_name.clone(),
                            generics: parse_quote! {},
                            paren_token: Default::default(),
                            inputs: Punctuated::new(),
                            output: parse_quote! { -> #alias_name },
                            variadic: None,
                        },
                        block: parse_quote! {
                            {
                                Default::default()
                            }
                        },
                    });
                    file.items.insert(index + 1, Box::new(function_item));
                    let path = syn::Path {
                        leading_colon: None,
                        segments: Punctuated::from_iter(vec![syn::PathSegment {
                            ident: alias_name,
                            arguments: syn::PathArguments::None,
                        }]),
                    };
                    static_item.ty = Box::new(Type::Path(syn::TypePath {
                        qself: None,
                        path,
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}