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

pub struct UnionWithAssociatedItemTypeReference_13;

impl Mutator for UnionWithAssociatedItemTypeReference_13 {
    fn name(&self) -> &str {
        "UnionWithAssociatedItemTypeReference_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, ref trait_path, _)) = impl_item.trait_ {
                    let trait_name = trait_path.segments.last().unwrap().ident.clone();
                    let self_ty = &impl_item.self_ty;
                    let union = syn::ItemUnion {
                        attrs: vec![],
                        vis: syn::Visibility::Inherited,
                        ident: Ident::new("U", Span::call_site()),
                        union_token: Token![union]::default(),
                        generics: parse_quote!(()),
                        fields: syn::Fields::Named(syn::FieldsNamed {
                            brace_token: syn::token::Brace::default(),
                            named: {
                                let mut fields = Punctuated::new();
                                fields.push(syn::Field {
                                    attrs: vec![],
                                    vis: syn::Visibility::Inherited,
                                    mutability: false,
                                    ident: Some(Ident::new("field", Span::call_site())),
                                    colon_token: Some(Default::default()),
                                    ty: parse_quote!(<#self_ty as #trait_name>::Item),
                                });
                                fields
                            },
                        }),
                    };
                    file.items.push(syn::Item::Union(union));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}