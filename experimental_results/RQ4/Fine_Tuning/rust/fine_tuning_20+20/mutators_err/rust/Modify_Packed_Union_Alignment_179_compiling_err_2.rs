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

pub struct Modify_Packed_Union_Alignment_179;

impl Mutator for Modify_Packed_Union_Alignment_179 {
    fn name(&self) -> &str {
        "Modify_Packed_Union_Alignment_179"
    }
    fn mutate(&self, file: &mut syn::File) {
        fn natural_alignment(ty: &Type) -> usize {
            match ty {
                Type::Path(type_path) => {
                    if let Some(segment) = type_path.path.segments.last() {
                        match segment.ident.to_string().as_str() {
                            "u8" | "i8" | "bool" => 1,
                            "u16" | "i16" => 2,
                            "u32" | "i32" | "f32" => 4,
                            "u64" | "i64" | "f64" => 8,
                            "u128" | "i128" => 16,
                            "usize" | "isize" => std::mem::size_of::<usize>(),
                            _ => std::mem::size_of::<usize>(),
                        }
                    } else {
                        std::mem::size_of::<usize>()
                    }
                }
                Type::Array(type_array) => natural_alignment(&type_array.elem),
                Type::Tuple(type_tuple) => type_tuple
                    .elems
                    .iter()
                    .map(natural_alignment)
                    .max()
                    .unwrap_or(std::mem::size_of::<usize>()),
                _ => std::mem::size_of::<usize>(),
            }
        }

        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                let mut max_alignment = 1;
                for field in &item_union.fields.named {
                    let field_alignment = natural_alignment(&field.ty);
                    if field_alignment > max_alignment {
                        max_alignment = field_alignment;
                    }
                }

                if let Some((_, attrs)) = item_union.attrs.split_first_mut() {
                    for attr in attrs {
                        if attr.path().is_ident("repr") {
                            if let Ok(meta) = attr.parse_meta() {
                                if let Meta::List(meta_list) = meta {
                                    if meta_list
                                        .nested
                                        .iter()
                                        .any(|nested| matches!(nested, NestedMeta::Meta(Meta::Path(path)) if path.is_ident("packed")))
                                    {
                                        let new_alignment = max_alignment + 1;
                                        let new_meta_list = Meta::List(MetaList {
                                            path: meta_list.path.clone(),
                                            delimiter: meta_list.delimiter.clone(),
                                            tokens: meta_list
                                                .nested
                                                .iter()
                                                .map(|nested| match nested {
                                                    NestedMeta::Meta(Meta::Path(path))
                                                        if path.is_ident("packed") =>
                                                    {
                                                        NestedMeta::Meta(Meta::List(
                                                            MetaList {
                                                                path: path.clone(),
                                                                delimiter: syn::MacroDelimiter::Paren(
                                                                    Paren::default(),
                                                                ),
                                                                tokens: {
                                                                    let lit: LitInt = parse_quote!(
                                                                        #new_alignment
                                                                    );
                                                                    lit.into_token_stream()
                                                                },
                                                            },
                                                        ))
                                                    }
                                                    _ => nested.clone(),
                                                })
                                                .collect(),
                                        });
                                        *attr = parse_quote!(#[#new_meta_list]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets union declarations with a `repr(packed)` attribute. It modifies the packing alignment to a value larger than the natural alignment of any field within the union, potentially creating misalignment issues that test the compiler's handling of packed unions."
    }
}