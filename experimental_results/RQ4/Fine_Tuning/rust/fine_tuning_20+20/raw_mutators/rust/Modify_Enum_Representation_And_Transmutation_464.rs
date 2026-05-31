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

pub struct Modify_Enum_Representation_And_Transmutation_464;

impl Mutator for Modify_Enum_Representation_And_Transmutation_464 {
    fn name(&self) -> &str {
        "Modify_Enum_Representation_And_Transmutation_464"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_packed = false;
                let mut has_align = false;
                item_enum.attrs.retain(|attr| {
                    if attr.path.is_ident("repr") {
                        if let Ok(meta) = attr.parse_meta() {
                            if let Meta::List(meta_list) = meta {
                                let mut new_nested = Punctuated::new();
                                for nested in meta_list.nested {
                                    match nested {
                                        NestedMeta::Meta(Meta::Path(path))
                                            if path.is_ident("packed") =>
                                        {
                                            has_packed = true;
                                            new_nested.push(nested);
                                        }
                                        NestedMeta::Meta(Meta::List(meta_list))
                                            if meta_list
                                                .path
                                                .is_ident("align") =>
                                        {
                                            has_align = true;
                                        }
                                        _ => {
                                            new_nested.push(nested);
                                        }
                                    }
                                }
                                if !new_nested.is_empty() {
                                    let new_meta_list = Meta::List(MetaList {
                                        path: meta_list.path,
                                        paren_token: meta_list.paren_token,
                                        nested: new_nested,
                                    });
                                    let new_attr = Attribute {
                                        pound_token: attr.pound_token,
                                        style: attr.style,
                                        bracket_token: attr.bracket_token,
                                        path: attr.path.clone(),
                                        tokens: new_meta_list.to_token_stream(),
                                    };
                                    *attr = new_attr;
                                    return true;
                                } else {
                                    return false;
                                }
                            }
                        }
                    }
                    true
                });
                if has_packed && has_align {
                    item_enum.variants.push(parse_quote! {
                        VariantWithFields { a: u16, b: u32 }
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enums with both `repr(packed)` and `repr(align(...))` attributes. It removes the `align` attribute while preserving `packed` and adds a new variant with fields of different sizes. This transformation stresses the compiler's handling of enum representation and transmutability by altering memory layout assumptions and increasing variant complexity."
    }
}