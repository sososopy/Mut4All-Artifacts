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

pub struct Enum_Variant_Padding_Modification_183;

impl Mutator for Enum_Variant_Padding_Modification_183 {
    fn name(&self) -> &str {
        "Enum_Variant_Padding_Modification_183"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if item_enum.variants.len() < 2 {
                    continue;
                }
                let mut has_small_variant = false;
                let mut largest_size = 0;
                for variant in &item_enum.variants {
                    let variant_size = variant
                        .fields
                        .iter()
                        .map(|field| {
                            if let Type::Path(type_path) = &field.ty {
                                match type_path.path.segments.last().unwrap().ident.to_string().as_str()
                                {
                                    "u8" => 1,
                                    "u16" => 2,
                                    "u32" => 4,
                                    "u64" => 8,
                                    _ => 0,
                                }
                            } else {
                                0
                            }
                        })
                        .sum::<usize>();
                    if variant_size < largest_size {
                        has_small_variant = true;
                    }
                    largest_size = largest_size.max(variant_size);
                }
                if !has_small_variant {
                    continue;
                }
                if !item_enum.attrs.iter().any(|attr| {
                    attr.path()
                        .segments
                        .last()
                        .map_or(false, |segment| segment.ident == "repr")
                }) {
                    item_enum.attrs.push(parse_quote!(#[repr(C)]));
                }
                let new_variant: Variant = parse_quote! {
                    PaddingMismatch(#[repr(C)] struct { u8, u16 })
                };
                item_enum.variants.push(new_variant);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify enums with multiple variants and at least one smaller variant. Ensure the enum has a representation attribute, defaulting to `#[repr(C)]`. Add a new variant with a struct that has a mismatched alignment, such as `#[repr(C)] struct { u8, u16 }`, to introduce padding and alignment challenges."
    }
}