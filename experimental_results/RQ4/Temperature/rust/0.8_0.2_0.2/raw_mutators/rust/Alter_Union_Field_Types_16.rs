use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Alter_Union_Field_Types_16;

impl Mutator for Alter_Union_Field_Types_16 {
    fn name(&self) -> &str {
        "Alter_Union_Field_Types_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                for field in &mut item_union.fields.named {
                    if let Type::Path(type_path) = &field.ty {
                        if let Some(segment) = type_path.path.segments.last() {
                            let new_type = match segment.ident.to_string().as_str() {
                                "u32" => parse_quote!(u8),
                                "u64" => parse_quote!(u16),
                                "i32" => parse_quote!(i8),
                                "i64" => parse_quote!(i16),
                                _ => continue,
                            };
                            field.ty = new_type;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets union definitions, specifically altering the types of union fields to types with different alignment requirements. By changing types like `u32` to `u8`, it aims to expose potential issues related to memory layout and alignment constraints, testing the compiler's handling of unions with varied field alignments."
    }
}