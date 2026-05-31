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

pub struct Add_Associated_Const_Expression_402;

impl Mutator for Add_Associated_Const_Expression_402 {
    fn name(&self) -> &str {
        "Add_Associated_Const_Expression_402"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    if let Some(generics) = &item_struct.generics.params.first() {
                        if let GenericParam::Const(_) = generics {
                            let new_const: syn::ImplItemConst = parse_quote! {
                                const DIFF: usize = T - R;
                            };
                            item_struct.fields.push(syn::Field {
                                attrs: Vec::new(),
                                vis: syn::Visibility::Inherited,
                                ident: Some(Ident::new("DIFF", Span::call_site())),
                                colon_token: Some(token::Colon::default()),
                                ty: syn::Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path::from(Ident::new("usize", Span::call_site())),
                                }),
                            });
                        }
                    }
                }
                Item::Trait(item_trait) => {
                    if item_trait.generics.params.iter().any(|param| matches!(param, GenericParam::Const(_))) {
                        item_trait.items.push(syn::TraitItem::Const(parse_quote! {
                            const SIZE_DIFF: usize = Self::Tokenized::SIZE - Self::Untokenized::SIZE;
                        }));
                    }
                }
                _ => {}
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs and traits with generic const expressions. It adds a new associated constant to these items, which is defined as a generic const expression involving existing constants or parameters. This transformation introduces new relationships and constraints, potentially leading to unexpected compiler behavior due to cyclic dependencies or conflicting constraints."
    }
}