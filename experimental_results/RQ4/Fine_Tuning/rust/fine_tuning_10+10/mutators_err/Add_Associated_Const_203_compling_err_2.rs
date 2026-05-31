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

pub struct Add_Associated_Const_203;

impl Mutator for Add_Associated_Const_203 {
    fn name(&self) -> &str {
        "Add_Associated_Const_203"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    if !item_struct.generics.params.is_empty() {
                        let new_const = syn::Field {
                            attrs: vec![],
                            vis: syn::Visibility::Public(syn::Visibility::Public {
                                pub_token: syn::token::Pub(Span::call_site()),
                            }),
                            ident: Some(Ident::new("DIFF", Span::call_site())),
                            colon_token: Some(token::Colon(Span::call_site())),
                            ty: parse_quote!(usize),
                            mutability: None,
                        };
                        item_struct.fields = syn::Fields::Named(syn::FieldsNamed {
                            brace_token: token::Brace(Span::call_site()),
                            named: {
                                let mut punctuated = Punctuated::new();
                                punctuated.push(new_const);
                                punctuated
                            },
                        });
                    }
                }
                Item::Trait(item_trait) => {
                    if !item_trait.generics.params.is_empty() {
                        let new_const: syn::TraitItemConst = parse_quote! {
                            const SIZE_DIFF: usize = Self::Tokenized::SIZE - Self::Untokenized::SIZE;
                        };
                        item_trait.items.push(syn::TraitItem::Const(new_const));
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs and traits using generic const expressions. It adds a new associated constant that involves existing constants or parameters in a non-trivial way, potentially introducing cyclic dependencies or conflicting constraints. This mutation aims to provoke unexpected compiler behavior by introducing new relationships and constraints via generic const expressions."
    }
}