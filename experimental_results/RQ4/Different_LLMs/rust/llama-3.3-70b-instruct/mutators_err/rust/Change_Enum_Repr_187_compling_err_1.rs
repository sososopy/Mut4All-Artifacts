use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Change_Enum_Repr_187;

impl Mutator for Change_Enum_Repr_187 {
    fn name(&self) -> &str {
        "Change_Enum_Repr_187"
    }

    fn mutate(&self, file: &mut syn::File) {
        let repr_attributes = vec!["u8", "u16", "u32", "u64", "i8", "i16", "i32", "i64", "usize", "isize"];
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                for attribute in &mut enum_item.attrs {
                    if attribute.path.is_ident("repr") {
                        let mut rng = thread_rng();
                        let new_repr = repr_attributes.choose(&mut rng).unwrap();
                        attribute.tokens = TokenStream::from(quote!(repr(#new_repr));
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The ChangeEnumRepr mutator changes the representation attribute of enum declarations. It randomly selects a different representation attribute from a predefined list, such as changing #[repr(u32)] to #[repr(i32)] or #[repr(C)]. This transformation tests the compiler's handling of different enum representations and their effects on layout and alignment."
    }
}