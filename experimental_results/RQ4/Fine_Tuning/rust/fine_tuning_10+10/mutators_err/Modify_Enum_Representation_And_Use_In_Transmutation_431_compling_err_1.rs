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

pub struct Modify_Enum_Representation_And_Use_In_Transmutation_431;

impl Mutator for Modify_Enum_Representation_And_Use_In_Transmutation_431 {
    fn name(&self) -> &str {
        "Modify_Enum_Representation_And_Use_In_Transmutation_431"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut repr_packed = false;
                item_enum.attrs.retain(|attr| {
                    if attr.path.is_ident("repr") {
                        if let Ok(meta) = attr.parse_meta() {
                            if let Meta::List(meta_list) = meta {
                                let mut new_tokens = TokenStream::new();
                                for nested_meta in meta_list.nested {
                                    if let NestedMeta::Meta(Meta::Path(path)) = &nested_meta {
                                        if path.is_ident("packed") {
                                            repr_packed = true;
                                            new_tokens.extend(quote! { packed, });
                                        } else if path.is_ident("align") {
                                            continue;
                                        } else {
                                            new_tokens.extend(quote! { #nested_meta, });
                                        }
                                    } else {
                                        new_tokens.extend(quote! { #nested_meta, });
                                    }
                                }
                                attr.tokens = new_tokens;
                            }
                        }
                    }
                    true
                });

                if repr_packed {
                    item_enum.variants.push(parse_quote! {
                        VariantWithFields { a: u16, b: u32 }
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator focuses on modifying enums with specific representation attributes. It identifies enums with `#[repr(packed, align(...))]`, removes the `align(...)` attribute, and retains `repr(packed)`. A new variant with fields of different sizes is added to increase memory layout complexity. This mutation targets potential issues in transmutability checks for enums with complex memory layouts."
    }
}