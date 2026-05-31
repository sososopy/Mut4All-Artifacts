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

pub struct Modify_Opaque_Return_Types_156;

impl Mutator for Modify_Opaque_Return_Types_156 {
    fn name(&self) -> &str {
        "Modify_Opaque_Return_Types_156"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref return_type) = func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &**return_type {
                        let type_alias_name = Ident::new("MyIter", Span::call_site());
                        let type_alias = syn::Item::Type(syn::ItemType {
                            attrs: vec![],
                            vis: syn::Visibility::Inherited,
                            type_token: token::Type { span: Span::call_site() },
                            ident: type_alias_name.clone(),
                            generics: Default::default(),
                            eq_token: token::Eq { spans: [Span::call_site(); 1] },
                            ty: Box::new(syn::Type::ImplTrait(type_impl_trait.clone())),
                            semi_token: token::Semi { spans: [Span::call_site(); 1] },
                        });
                        new_items.push(type_alias);
                        func.sig.output = syn::ReturnType::Type(
                            token::RArrow { spans: [Span::call_site(); 1] },
                            Box::new(syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(type_alias_name),
                            })),
                        );
                    }
                }
            }
        }
        file.items.splice(0..0, new_items);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}