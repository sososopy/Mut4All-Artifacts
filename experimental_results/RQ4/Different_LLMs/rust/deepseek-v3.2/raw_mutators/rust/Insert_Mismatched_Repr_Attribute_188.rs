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

pub struct Insert_Mismatched_Repr_Attribute_188;

impl Mutator for Insert_Mismatched_Repr_Attribute_188 {
    fn name(&self) -> &str {
        "Insert_Mismatched_Repr_Attribute_188"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            match item {
                syn::Item::Enum(item_enum) => {
                    let conflicting_attr = syn::Attribute {
                        style: syn::AttrStyle::Outer,
                        meta: syn::Meta::List(syn::MetaList {
                            path: syn::Path::from(syn::Ident::new("repr", Span::call_site())),
                            delimiter: syn::MacroDelimiter::Paren(syn::token::Paren::default()),
                            tokens: proc_macro2::TokenStream::from_str("packed").unwrap(),
                        }),
                        pound_token: syn::token::Pound::default(),
                        bracket_token: syn::token::Bracket::default(),
                    };
                    item_enum.attrs.insert(0, conflicting_attr);
                }
                syn::Item::Union(item_union) => {
                    let repr_options = ["u8", "u16", "u32", "u64", "usize", "i8", "i16", "i32", "i64", "isize", "C", "transparent"];
                    let selected = repr_options.choose(&mut rng).unwrap();
                    let conflicting_attr = syn::Attribute {
                        style: syn::AttrStyle::Outer,
                        meta: syn::Meta::List(syn::MetaList {
                            path: syn::Path::from(syn::Ident::new("repr", Span::call_site())),
                            delimiter: syn::MacroDelimiter::Paren(syn::token::Paren::default()),
                            tokens: proc_macro2::TokenStream::from_str(selected).unwrap(),
                        }),
                        pound_token: syn::token::Pound::default(),
                        bracket_token: syn::token::Bracket::default(),
                    };
                    item_union.attrs.insert(0, conflicting_attr);
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts conflicting repr attributes on enum and union definitions. For enums, it adds repr(packed), which is invalid for enums and conflicts with any existing repr attribute. For unions, it randomly selects a repr attribute from a set of valid but potentially conflicting options (repr(u8), repr(u16), repr(u32), repr(u64), repr(usize), repr(i8), repr(i16), repr(i32), repr(i64), repr(isize), repr(C), repr(transparent)) and inserts it before any existing repr attributes. This transformation aims to trigger compiler errors related to ABI conflicts, alignment mismatches, and invalid repr combinations, testing the compiler's attribute validation and repr handling logic."
    }
}