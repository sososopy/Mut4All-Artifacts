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

pub struct Modify_Union_Field_Types_16;

impl Mutator for Modify_Union_Field_Types_16 {
    fn name(&self) -> &str {
        "Modify_Union_Field_Types_16"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                for field in &mut item_union.fields.named {
                    let new_type = match &field.ty {
                        Type::Path(type_path) => {
                            let ident = &type_path.path.segments.last().unwrap().ident;
                            match ident.to_string().as_str() {
                                "u32" => parse_quote!(u16),
                                "f32" => parse_quote!(f64),
                                _ => continue,
                            }
                        }
                        _ => continue,
                    };
                    field.ty = new_type;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the types of fields within a union to other types with different alignment requirements (e.g., changing `u32` to `u16` and `f32` to `f64`). This transformation is intended to expose potential bugs related to alignment mismatches between ABI and layout."
    }
}