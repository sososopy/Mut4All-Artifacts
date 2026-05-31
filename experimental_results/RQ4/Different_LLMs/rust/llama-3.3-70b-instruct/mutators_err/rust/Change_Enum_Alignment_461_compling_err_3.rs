use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Change_Enum_Alignment_461;

impl Mutator for Change_Enum_Alignment_461 {
    fn name(&self) -> &str {
        "Change_Enum_Alignment_461"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let alignment = 2; // Change alignment to 2 bytes
                let attribute = Attribute {
                    pound_token: token::Pound {
                        spans: [Span::call_site()],
                    },
                    style: syn::AttrStyle::Inner(Default::default()),
                    bracket_token: Default::default(),
                    meta: Meta::List(MetaList {
                        path: parse_quote!(repr),
                        paren_token: Default::default(),
                        elements: Punctuated::from_iter(vec![
                            Meta::NameValue(MetaNameValue {
                                path: parse_quote!(C),
                                equals_token: Default::default(),
                                value: Lit::Str(LitStr {
                                    value: "packed".to_string(),
                                    span: Span::call_site(),
                                    style: LitStrStyle::Cooked,
                                }),
                            }),
                            Meta::NameValue(MetaNameValue {
                                path: parse_quote!(packed),
                                equals_token: Default::default(),
                                value: Lit::Int(LitInt {
                                    base: 10,
                                    suffix: None,
                                    span: Span::call_site(),
                                    value: alignment,
                                }),
                            }),
                        ]),
                    }),
                };
                if let Item::Enum(item_enum) = item {
                    item_enum.attrs.push(attribute);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the alignment of enums by adding or modifying the `#[repr(C, packed(n))]` attribute. This transformation aims to trigger bugs related to alignment mismatches and test the compiler's handling of enum layout and ABI compatibility."
    }
}