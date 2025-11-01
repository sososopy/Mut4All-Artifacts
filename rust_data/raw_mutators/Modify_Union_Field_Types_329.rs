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

pub struct Modify_Union_Field_Types_329;

impl Mutator for Modify_Union_Field_Types_329 {
    fn name(&self) -> &str {
        "Modify_Union_Field_Types_329"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                let mut rng = thread_rng();
                for field in &mut item_union.fields.named {
                    if let Type::Path(TypePath { path, .. }) = &field.ty {
                        if path.segments.len() == 1 {
                            let segment = &path.segments[0];
                            if segment.ident.to_string().contains("Enum") {
                                let new_type = if rng.gen_bool(0.5) { parse_quote!(u32) } else { parse_quote!(i32) };
                                field.ty = new_type;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies union definitions containing fields of different enum types. It selects a field represented by an enum type and changes its type to a primitive integer type (u32 or i32). This ensures the mutation aligns with the intended value range of the enum while potentially triggering alignment and layout calculation issues in the compiler due to differing data layouts."
    }
}