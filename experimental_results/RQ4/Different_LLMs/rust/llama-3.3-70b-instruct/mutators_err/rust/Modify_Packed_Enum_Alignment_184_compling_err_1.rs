use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote, punctuated::Punctuated, spanned::Spanned,
    token, token::Comma, token::{Paren, Plus}, visit::Visit, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Modify_Packed_Enum_Alignment_184;

impl Mutator for Modify_Packed_Enum_Alignment_184 {
    fn name(&self) -> &str {
        "Modify_Packed_Enum_Alignment_184"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                for attr in &enum_item.attrs {
                    if let Attribute { path, .. } = attr {
                        if path.is_ident("repr") {
                            if let Some(repr) = path.get_ident() {
                                if repr == "packed" {
                                    let mut rng = thread_rng();
                                    let mut variants = enum_item.variants.clone();
                                    if rng.gen_bool(0.5) {
                                        // Add a padding variant
                                        let padding_variant = parse_quote! {
                                            _padding: u8,
                                        };
                                        variants.push(padding_variant);
                                    } else {
                                        // Remove a variant
                                        if !variants.is_empty() {
                                            let index = rng.gen_range(0..variants.len());
                                            variants.remove(index);
                                        }
                                    }
                                    enum_item.variants = variants;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums with the #[repr(packed)] attribute and modifies their alignment by adding or removing padding bytes. This transformation aims to test the compiler's handling of enum alignment and size, potentially exposing bugs related to the alignment and size of enums with the #[repr(packed)] attribute."
    }
}