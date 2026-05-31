use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Apply_Conflicting_Repr_Attributes_187;

impl Mutator for Apply_Conflicting_Repr_Attributes_187 {
    fn name(&self) -> &str {
        "Apply_Conflicting_Repr_Attributes_187"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut union_field_types = HashSet::new();
        for item in &file.items {
            if let Item::Union(union_item) = item {
                if union_item.fields.named.len() >= 2 {
                    for field in &union_item.fields.named {
                        let ty = &field.ty;
                        if let Type::Path(type_path) = ty {
                            let segments = &type_path.path.segments;
                            if let Some(last_segment) = segments.last() {
                                union_field_types.insert(last_segment.ident.clone());
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                if union_field_types.contains(&enum_item.ident) {
                    let mut repr_attrs = Vec::new();
                    for attr in &enum_item.attrs {
                        if attr.path().is_ident("repr") {
                            repr_attrs.push(attr.clone());
                        }
                    }
                    if repr_attrs.is_empty() {
                        enum_item.attrs.push(parse_quote!(#[repr(u32)]));
                    } else {
                        let mut rng = thread_rng();
                        let repr_choices = ["#[repr(u8)]", "#[repr(u16)]", "#[repr(u32)]", "#[repr(u64)]", "#[repr(C)]", "#[repr(C, packed)]"];
                        let new_repr = repr_choices.choose(&mut rng).unwrap();
                        enum_item.attrs.push(parse_quote!(#new_repr));
                    }
                }
            }
            if let Item::Struct(struct_item) = item {
                if union_field_types.contains(&struct_item.ident) {
                    let mut repr_attrs = Vec::new();
                    for attr in &struct_item.attrs {
                        if attr.path().is_ident("repr") {
                            repr_attrs.push(attr.clone());
                        }
                    }
                    if repr_attrs.is_empty() {
                        struct_item.attrs.push(parse_quote!(#[repr(C, packed(2))]));
                    } else {
                        let mut rng = thread_rng();
                        let repr_choices = ["#[repr(C)]", "#[repr(C, packed)]", "#[repr(C, packed(2))]", "#[repr(C, packed(4))]", "#[repr(C, align(8))]", "#[repr(transparent)]"];
                        let new_repr = repr_choices.choose(&mut rng).unwrap();
                        struct_item.attrs.push(parse_quote!(#new_repr));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unions with at least two fields. It collects the type names of union fields and modifies their corresponding enum or struct definitions by adding or altering #[repr] attributes. For enums, if no repr exists, it adds #[repr(u32)]; otherwise, it adds a conflicting repr like #[repr(u8)]. For structs, if no repr exists, it adds #[repr(C, packed(2))]; otherwise, it adds a conflicting repr like #[repr(C, align(8))]. This creates alignment mismatches within unions, potentially triggering layout computation assertions or ABI conflicts."
    }
}