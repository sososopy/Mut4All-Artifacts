use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, 
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    parse_quote,
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

pub struct Add_Tuple_Field_And_Pattern_Match_240;

impl Mutator for Add_Tuple_Field_And_Pattern_Match_240 {
    fn name(&self) -> &str {
        "Add_Tuple_Field_And_Pattern_Match_240"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                item_struct.fields = syn::Fields::Unnamed(syn::FieldsUnnamed {
                    paren_token: Default::default(),
                    unnamed: {
                        let mut fields = syn::punctuated::Punctuated::new();
                        fields.push(parse_quote!(()));
                        fields.push(parse_quote!(()));
                        fields
                    },
                });

                let struct_name = &item_struct.ident;
                let mut found_impl = false;
                for item in &mut file.items {
                    if let syn::Item::Impl(impl_item) = item {
                        if impl_item.trait_.is_none()
                            && impl_item
                                .self_ty
                                .as_ref()
                                .map(|ty| {
                                    if let syn::Type::Path(type_path) = ty {
                                        if let Some(segment) = type_path.path.segments.last() {
                                            segment.ident == *struct_name
                                        } else {
                                            false
                                        }
                                    } else {
                                        false
                                    }
                                })
                                .unwrap_or(false)
                        {
                            found_impl = true;
                            break;
                        }
                    }
                }

                if !found_impl {
                    let new_impl: syn::ItemImpl = parse_quote! {
                        impl #struct_name {
                            fn check() {
                                let a;
                                (Self, a) = (Self::new(), ());
                            }
                        }
                    };
                    file.items.push(syn::Item::Impl(new_impl));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}